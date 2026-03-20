#include "yrich-persist.h"
#include <msgpack.hpp>
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>

namespace yetty::yrich {

//=============================================================================
// Helper functions
//=============================================================================

static std::string colorToHex(uint32_t color) {
    char buf[14];
    snprintf(buf, sizeof(buf), "\"#%08X\"", color);
    return buf;
}

static uint32_t hexToColor(const std::string& hex) {
    if (hex.empty()) return 0xFF000000;
    std::string h = hex;
    if (h[0] == '#') h = h.substr(1);
    return static_cast<uint32_t>(std::stoul(h, nullptr, 16));
}

static std::string indent(int level) {
    return std::string(level * 2, ' ');
}

static std::string escapeYaml(const std::string& s) {
    // Simple escape for YAML strings
    bool needQuotes = false;
    for (char c : s) {
        if (c == ':' || c == '#' || c == '\n' || c == '"' || c == '\'' || c == '[' || c == ']') {
            needQuotes = true;
            break;
        }
    }
    if (!needQuotes && !s.empty() && (s[0] == ' ' || s.back() == ' ')) {
        needQuotes = true;
    }

    if (!needQuotes) return s;

    std::string result = "\"";
    for (char c : s) {
        if (c == '"') result += "\\\"";
        else if (c == '\\') result += "\\\\";
        else if (c == '\n') result += "\\n";
        else if (c == '\t') result += "\\t";
        else result += c;
    }
    result += "\"";
    return result;
}

static uint64_t currentTimestamp() {
    return static_cast<uint64_t>(std::time(nullptr));
}

//=============================================================================
// YDoc extraction
//=============================================================================

YDocData DocumentPersist::extract(const YDoc& doc) {
    YDocData data;
    data.version = 1;
    data.type = "ydoc";
    data.modified = currentTimestamp();
    data.pageWidth = doc.pageWidth();
    data.margin = doc.margin();

    // Paragraphs
    for (int i = 0; i < doc.paragraphCount(); ++i) {
        auto para = doc.paragraphAt(i);
        if (!para) continue;

        ParagraphData pd;
        pd.text = para->text();
        pd.fontSize = para->textStyle().fontSize;
        pd.color = para->textStyle().color.toPacked();

        for (const auto& run : para->runs()) {
            TextRunData rd;
            rd.start = run.start;
            rd.end = run.end;
            rd.fontSize = run.style.fontSize;
            rd.color = run.style.color.toPacked();
            rd.bgColor = run.style.bgColor.toPacked();
            rd.format = static_cast<uint32_t>(run.style.format);
            rd.fontId = run.style.fontId;
            pd.runs.push_back(rd);
        }

        data.paragraphs.push_back(pd);
    }

    // Tables
    for (int i = 0; i < doc.tableCount(); ++i) {
        auto table = doc.tableAt(i);
        if (!table) continue;

        TableData td;
        td.rows = table->rows();
        td.cols = table->cols();

        for (int c = 0; c < td.cols; ++c) {
            td.columnWidths.push_back(table->columnWidth(c));
        }
        for (int r = 0; r < td.rows; ++r) {
            td.rowHeights.push_back(table->rowHeight(r));
        }

        for (int r = 0; r < td.rows; ++r) {
            for (int c = 0; c < td.cols; ++c) {
                TableCellData cd;
                cd.text = table->cell(r, c);
                td.cells.push_back(cd);
            }
        }

        data.tables.push_back(td);
    }

    // Images
    for (int i = 0; i < doc.imageCount(); ++i) {
        auto img = doc.imageAt(i);
        if (!img) continue;

        ImageData id;
        id.x = img->bounds().x;
        id.y = img->bounds().y;
        id.width = img->bounds().w;
        id.height = img->bounds().h;
        id.source = img->source();
        id.altText = img->altText();
        id.caption = img->caption();
        id.align = static_cast<int>(img->align());

        data.images.push_back(id);
    }

    // Comments
    for (const auto& comment : doc.allComments()) {
        CommentData cd;
        cd.id = comment->id;
        cd.paragraphIndex = comment->paragraphIndex;
        cd.startPos = comment->startPos;
        cd.endPos = comment->endPos;
        cd.text = comment->text;
        cd.author = comment->author;
        cd.timestamp = comment->timestamp;
        cd.resolved = comment->resolved;

        for (const auto& reply : comment->replies) {
            CommentData rd;
            rd.id = reply.id;
            rd.text = reply.text;
            rd.author = reply.author;
            rd.timestamp = reply.timestamp;
            cd.replies.push_back(rd);
        }

        data.comments.push_back(cd);
    }

    // Versions
    for (const auto& ver : doc.versions()) {
        VersionData vd;
        vd.timestamp = ver.timestamp;
        vd.author = ver.author;
        vd.description = ver.description;
        vd.operationIndex = ver.operationIndex;
        data.versions.push_back(vd);
    }

    return data;
}

void DocumentPersist::populate(YDoc& doc, const YDocData& data) {
    doc.setPageWidth(data.pageWidth);
    doc.setPageMargin(data.margin);

    // Clear existing content
    while (doc.paragraphCount() > 0) {
        doc.removeParagraph(0);
    }

    // Add paragraphs
    for (const auto& pd : data.paragraphs) {
        auto para = doc.addParagraph(pd.text);
        if (!para) continue;

        TextStyle style;
        style.fontSize = pd.fontSize;
        style.color = Color::fromPacked(pd.color);
        para->setTextStyle(style);

        // Apply runs
        for (const auto& rd : pd.runs) {
            if (rd.format != 0) {
                para->applyFormat(rd.start, rd.end, static_cast<TextFormat>(rd.format));
            }
            if (rd.fontSize != pd.fontSize) {
                para->setFontSize(rd.start, rd.end, rd.fontSize);
            }
            if (rd.color != pd.color) {
                para->setColor(rd.start, rd.end, Color::fromPacked(rd.color));
            }
        }
    }

    // Add tables
    for (const auto& td : data.tables) {
        auto table = doc.insertTable(doc.paragraphCount(), td.rows, td.cols);
        if (!table) continue;

        for (size_t c = 0; c < td.columnWidths.size(); ++c) {
            table->setColumnWidth(static_cast<int>(c), td.columnWidths[c]);
        }
        for (size_t r = 0; r < td.rowHeights.size(); ++r) {
            table->setRowHeight(static_cast<int>(r), td.rowHeights[r]);
        }

        for (int r = 0; r < td.rows; ++r) {
            for (int c = 0; c < td.cols; ++c) {
                int idx = r * td.cols + c;
                if (idx < static_cast<int>(td.cells.size())) {
                    table->setCell(r, c, td.cells[idx].text);
                }
            }
        }
    }

    // Add images
    for (const auto& id : data.images) {
        auto img = doc.insertImage(doc.paragraphCount(), id.width, id.height);
        if (!img) continue;

        img->setSource(id.source);
        img->setAltText(id.altText);
        img->setCaption(id.caption);
        img->setAlign(static_cast<InlineImage::Align>(id.align));
    }

    // Add comments
    for (const auto& cd : data.comments) {
        auto comment = doc.addComment(cd.paragraphIndex, cd.startPos, cd.endPos, cd.text, cd.author);
        if (cd.resolved) {
            doc.resolveComment(comment->id);
        }
        for (const auto& rd : cd.replies) {
            doc.replyToComment(comment->id, rd.text, rd.author);
        }
    }
}

//=============================================================================
// YDoc YAML serialization
//=============================================================================

std::string DocumentPersist::toYaml(const YDoc& doc) {
    auto data = extract(doc);
    std::ostringstream os;

    os << "# YDoc Document\n";
    os << "version: " << data.version << "\n";
    os << "type: " << data.type << "\n";
    os << "\n";

    os << "metadata:\n";
    os << "  modified: " << data.modified << "\n";
    if (!data.author.empty()) {
        os << "  author: " << escapeYaml(data.author) << "\n";
    }
    os << "\n";

    os << "document:\n";
    os << "  pageWidth: " << data.pageWidth << "\n";
    os << "  margin: " << data.margin << "\n";
    os << "\n";

    // Paragraphs
    if (!data.paragraphs.empty()) {
        os << "  paragraphs:\n";
        for (const auto& para : data.paragraphs) {
            os << "    - text: " << escapeYaml(para.text) << "\n";
            if (para.fontSize != 14.0f) {
                os << "      fontSize: " << para.fontSize << "\n";
            }
            if (para.color != 0xFF000000) {
                os << "      color: " << colorToHex(para.color) << "\n";
            }
            if (!para.runs.empty()) {
                os << "      runs:\n";
                for (const auto& run : para.runs) {
                    os << "        - start: " << run.start << "\n";
                    os << "          end: " << run.end << "\n";
                    if (run.format != 0) {
                        os << "          format: " << run.format << "\n";
                    }
                    if (run.fontSize != para.fontSize) {
                        os << "          fontSize: " << run.fontSize << "\n";
                    }
                    if (run.color != para.color) {
                        os << "          color: " << colorToHex(run.color) << "\n";
                    }
                }
            }
        }
        os << "\n";
    }

    // Tables
    if (!data.tables.empty()) {
        os << "  tables:\n";
        for (const auto& table : data.tables) {
            os << "    - rows: " << table.rows << "\n";
            os << "      cols: " << table.cols << "\n";
            os << "      columnWidths: [";
            for (size_t i = 0; i < table.columnWidths.size(); ++i) {
                if (i > 0) os << ", ";
                os << table.columnWidths[i];
            }
            os << "]\n";
            os << "      cells:\n";
            for (int r = 0; r < table.rows; ++r) {
                os << "        - [";
                for (int c = 0; c < table.cols; ++c) {
                    if (c > 0) os << ", ";
                    int idx = r * table.cols + c;
                    if (idx < static_cast<int>(table.cells.size())) {
                        os << escapeYaml(table.cells[idx].text);
                    }
                }
                os << "]\n";
            }
        }
        os << "\n";
    }

    // Images
    if (!data.images.empty()) {
        os << "  images:\n";
        for (const auto& img : data.images) {
            os << "    - width: " << img.width << "\n";
            os << "      height: " << img.height << "\n";
            if (!img.source.empty()) {
                os << "      source: " << escapeYaml(img.source) << "\n";
            }
            if (!img.altText.empty()) {
                os << "      altText: " << escapeYaml(img.altText) << "\n";
            }
            if (!img.caption.empty()) {
                os << "      caption: " << escapeYaml(img.caption) << "\n";
            }
        }
        os << "\n";
    }

    // Comments
    if (!data.comments.empty()) {
        os << "  comments:\n";
        for (const auto& comment : data.comments) {
            os << "    - paragraph: " << comment.paragraphIndex << "\n";
            os << "      start: " << comment.startPos << "\n";
            os << "      end: " << comment.endPos << "\n";
            os << "      text: " << escapeYaml(comment.text) << "\n";
            os << "      author: " << escapeYaml(comment.author) << "\n";
            if (comment.resolved) {
                os << "      resolved: true\n";
            }
            if (!comment.replies.empty()) {
                os << "      replies:\n";
                for (const auto& reply : comment.replies) {
                    os << "        - text: " << escapeYaml(reply.text) << "\n";
                    os << "          author: " << escapeYaml(reply.author) << "\n";
                }
            }
        }
        os << "\n";
    }

    return os.str();
}

Result<YDoc::Ptr> DocumentPersist::fromYaml(std::string_view yaml) {
    try {
        YAML::Node root = YAML::Load(std::string(yaml));

        if (root["type"].as<std::string>("") != "ydoc") {
            return Err<YDoc::Ptr>("Not a YDoc document");
        }

        YDocData data;
        data.version = root["version"].as<int>(1);
        data.pageWidth = root["document"]["pageWidth"].as<float>(600.0f);
        data.margin = root["document"]["margin"].as<float>(20.0f);

        if (root["metadata"]) {
            data.author = root["metadata"]["author"].as<std::string>("");
            data.modified = root["metadata"]["modified"].as<uint64_t>(0);
        }

        // Paragraphs
        if (root["document"]["paragraphs"]) {
            for (const auto& pnode : root["document"]["paragraphs"]) {
                ParagraphData pd;
                pd.text = pnode["text"].as<std::string>("");
                pd.fontSize = pnode["fontSize"].as<float>(14.0f);
                pd.color = hexToColor(pnode["color"].as<std::string>("#FF000000"));

                if (pnode["runs"]) {
                    for (const auto& rnode : pnode["runs"]) {
                        TextRunData rd;
                        rd.start = rnode["start"].as<int>(0);
                        rd.end = rnode["end"].as<int>(0);
                        rd.format = rnode["format"].as<uint32_t>(0);
                        rd.fontSize = rnode["fontSize"].as<float>(pd.fontSize);
                        rd.color = hexToColor(rnode["color"].as<std::string>(colorToHex(pd.color)));
                        pd.runs.push_back(rd);
                    }
                }
                data.paragraphs.push_back(pd);
            }
        }

        // Tables
        if (root["document"]["tables"]) {
            for (const auto& tnode : root["document"]["tables"]) {
                TableData td;
                td.rows = tnode["rows"].as<int>(0);
                td.cols = tnode["cols"].as<int>(0);

                if (tnode["columnWidths"]) {
                    for (const auto& w : tnode["columnWidths"]) {
                        td.columnWidths.push_back(w.as<float>(100.0f));
                    }
                }

                if (tnode["cells"]) {
                    for (const auto& row : tnode["cells"]) {
                        for (const auto& cell : row) {
                            TableCellData cd;
                            cd.text = cell.as<std::string>("");
                            td.cells.push_back(cd);
                        }
                    }
                }
                data.tables.push_back(td);
            }
        }

        // Images
        if (root["document"]["images"]) {
            for (const auto& inode : root["document"]["images"]) {
                ImageData id;
                id.width = inode["width"].as<float>(100.0f);
                id.height = inode["height"].as<float>(100.0f);
                id.source = inode["source"].as<std::string>("");
                id.altText = inode["altText"].as<std::string>("");
                id.caption = inode["caption"].as<std::string>("");
                data.images.push_back(id);
            }
        }

        // Comments
        if (root["document"]["comments"]) {
            for (const auto& cnode : root["document"]["comments"]) {
                CommentData cd;
                cd.paragraphIndex = cnode["paragraph"].as<int>(0);
                cd.startPos = cnode["start"].as<int>(0);
                cd.endPos = cnode["end"].as<int>(0);
                cd.text = cnode["text"].as<std::string>("");
                cd.author = cnode["author"].as<std::string>("");
                cd.resolved = cnode["resolved"].as<bool>(false);

                if (cnode["replies"]) {
                    for (const auto& rnode : cnode["replies"]) {
                        CommentData rd;
                        rd.text = rnode["text"].as<std::string>("");
                        rd.author = rnode["author"].as<std::string>("");
                        cd.replies.push_back(rd);
                    }
                }
                data.comments.push_back(cd);
            }
        }

        auto docResult = YDoc::create();
        if (!docResult) return Err<YDoc::Ptr>("Failed to create YDoc");

        populate(**docResult, data);
        return docResult;

    } catch (const std::exception& e) {
        return Err<YDoc::Ptr>(std::string("YAML parse error: ") + e.what());
    }
}

//=============================================================================
// YDoc binary (msgpack) serialization
//=============================================================================

std::vector<uint8_t> DocumentPersist::toBinary(const YDoc& doc) {
    auto data = extract(doc);
    std::stringstream ss;
    msgpack::packer<std::stringstream> pk(ss);

    // Pack document structure
    pk.pack_map(6);

    pk.pack("version");
    pk.pack(data.version);

    pk.pack("type");
    pk.pack(data.type);

    pk.pack("pageWidth");
    pk.pack(data.pageWidth);

    pk.pack("margin");
    pk.pack(data.margin);

    pk.pack("modified");
    pk.pack(data.modified);

    // Paragraphs
    pk.pack("paragraphs");
    pk.pack_array(data.paragraphs.size());
    for (const auto& para : data.paragraphs) {
        pk.pack_map(4);
        pk.pack("text");
        pk.pack(para.text);
        pk.pack("fontSize");
        pk.pack(para.fontSize);
        pk.pack("color");
        pk.pack(para.color);
        pk.pack("runs");
        pk.pack_array(para.runs.size());
        for (const auto& run : para.runs) {
            pk.pack_array(5);
            pk.pack(run.start);
            pk.pack(run.end);
            pk.pack(run.format);
            pk.pack(run.fontSize);
            pk.pack(run.color);
        }
    }

    std::string str = ss.str();
    return std::vector<uint8_t>(str.begin(), str.end());
}

Result<YDoc::Ptr> DocumentPersist::fromBinary(const uint8_t* data, size_t len) {
    try {
        msgpack::object_handle oh = msgpack::unpack(reinterpret_cast<const char*>(data), len);
        msgpack::object obj = oh.get();

        if (obj.type != msgpack::type::MAP) {
            return Err<YDoc::Ptr>("Invalid binary format");
        }

        YDocData docData;
        docData.version = 1;

        auto map = obj.as<std::map<std::string, msgpack::object>>();

        if (map.count("pageWidth")) docData.pageWidth = map["pageWidth"].as<float>();
        if (map.count("margin")) docData.margin = map["margin"].as<float>();

        if (map.count("paragraphs")) {
            auto& arr = map["paragraphs"];
            if (arr.type == msgpack::type::ARRAY) {
                for (size_t i = 0; i < arr.via.array.size; ++i) {
                    auto& pobj = arr.via.array.ptr[i];
                    ParagraphData pd;

                    auto pmap = pobj.as<std::map<std::string, msgpack::object>>();
                    if (pmap.count("text")) pd.text = pmap["text"].as<std::string>();
                    if (pmap.count("fontSize")) pd.fontSize = pmap["fontSize"].as<float>();
                    if (pmap.count("color")) pd.color = pmap["color"].as<uint32_t>();

                    if (pmap.count("runs")) {
                        auto& runs = pmap["runs"];
                        if (runs.type == msgpack::type::ARRAY) {
                            for (size_t j = 0; j < runs.via.array.size; ++j) {
                                auto& robj = runs.via.array.ptr[j];
                                if (robj.type == msgpack::type::ARRAY && robj.via.array.size >= 5) {
                                    TextRunData rd;
                                    rd.start = robj.via.array.ptr[0].as<int>();
                                    rd.end = robj.via.array.ptr[1].as<int>();
                                    rd.format = robj.via.array.ptr[2].as<uint32_t>();
                                    rd.fontSize = robj.via.array.ptr[3].as<float>();
                                    rd.color = robj.via.array.ptr[4].as<uint32_t>();
                                    pd.runs.push_back(rd);
                                }
                            }
                        }
                    }
                    docData.paragraphs.push_back(pd);
                }
            }
        }

        auto docResult = YDoc::create();
        if (!docResult) return Err<YDoc::Ptr>("Failed to create YDoc");

        populate(**docResult, docData);
        return docResult;

    } catch (const std::exception& e) {
        return Err<YDoc::Ptr>(std::string("Binary parse error: ") + e.what());
    }
}

//=============================================================================
// YDoc file I/O
//=============================================================================

Result<void> DocumentPersist::saveYDoc(const YDoc& doc, const std::string& path) {
    Format fmt = detectFormat(path);

    std::ofstream file(path, std::ios::binary);
    if (!file) {
        return Err("Failed to open file for writing: " + path);
    }

    if (fmt == Format::Yaml) {
        std::string yaml = toYaml(doc);
        file.write(yaml.data(), yaml.size());
    } else {
        auto binary = toBinary(doc);
        file.write(reinterpret_cast<const char*>(binary.data()), binary.size());
    }

    return Ok();
}

Result<YDoc::Ptr> DocumentPersist::loadYDoc(const std::string& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) {
        return Err<YDoc::Ptr>("Failed to open file: " + path);
    }

    auto size = file.tellg();
    file.seekg(0);

    std::vector<char> buffer(size);
    file.read(buffer.data(), size);

    Format fmt = detectFormat(path);

    if (fmt == Format::Yaml) {
        return fromYaml(std::string_view(buffer.data(), buffer.size()));
    } else {
        return fromBinary(reinterpret_cast<const uint8_t*>(buffer.data()), buffer.size());
    }
}

//=============================================================================
// YSpreadsheet serialization (simplified)
//=============================================================================

YSpreadsheetData DocumentPersist::extract(const YSpreadsheet& doc) {
    YSpreadsheetData data;
    data.version = 1;
    data.type = "yspreadsheet";
    data.modified = currentTimestamp();
    data.rows = doc.rowCount();
    data.cols = doc.colCount();

    for (int c = 0; c < data.cols; ++c) {
        data.columnWidths.push_back(doc.colWidth(c));
    }
    for (int r = 0; r < data.rows; ++r) {
        data.rowHeights.push_back(doc.rowHeight(r));
    }

    // Only save non-empty cells (sparse)
    for (int r = 0; r < data.rows; ++r) {
        for (int c = 0; c < data.cols; ++c) {
            std::string val = doc.cellValue({r, c});
            if (!val.empty()) {
                SpreadsheetCellData cd;
                cd.row = r;
                cd.col = c;
                cd.value = val;
                data.cells.push_back(cd);
            }
        }
    }

    return data;
}

void DocumentPersist::populate(YSpreadsheet& doc, const YSpreadsheetData& data) {
    for (size_t c = 0; c < data.columnWidths.size(); ++c) {
        doc.setColWidth(static_cast<int>(c), data.columnWidths[c]);
    }
    for (size_t r = 0; r < data.rowHeights.size(); ++r) {
        doc.setRowHeight(static_cast<int>(r), data.rowHeights[r]);
    }

    for (const auto& cell : data.cells) {
        doc.setCellValue({cell.row, cell.col}, cell.value);
    }
}

std::string DocumentPersist::toYaml(const YSpreadsheet& doc) {
    auto data = extract(doc);
    std::ostringstream os;

    os << "# YSpreadsheet Document\n";
    os << "version: " << data.version << "\n";
    os << "type: " << data.type << "\n";
    os << "\n";

    os << "metadata:\n";
    os << "  modified: " << data.modified << "\n";
    os << "\n";

    os << "spreadsheet:\n";
    os << "  rows: " << data.rows << "\n";
    os << "  cols: " << data.cols << "\n";
    os << "\n";

    os << "  columnWidths: [";
    for (size_t i = 0; i < data.columnWidths.size(); ++i) {
        if (i > 0) os << ", ";
        os << data.columnWidths[i];
    }
    os << "]\n\n";

    os << "  cells:\n";
    for (const auto& cell : data.cells) {
        char colLetter = 'A' + cell.col;
        os << "    " << colLetter << (cell.row + 1) << ": " << escapeYaml(cell.value) << "\n";
    }

    return os.str();
}

Result<YSpreadsheet::Ptr> DocumentPersist::fromYaml(std::string_view yaml, int) {
    try {
        YAML::Node root = YAML::Load(std::string(yaml));

        if (root["type"].as<std::string>("") != "yspreadsheet") {
            return Err<YSpreadsheet::Ptr>("Not a YSpreadsheet document");
        }

        YSpreadsheetData data;

        if (root["spreadsheet"]["columnWidths"]) {
            for (const auto& w : root["spreadsheet"]["columnWidths"]) {
                data.columnWidths.push_back(w.as<float>(80.0f));
            }
        }

        if (root["spreadsheet"]["cells"]) {
            for (const auto& it : root["spreadsheet"]["cells"]) {
                std::string key = it.first.as<std::string>();
                std::string val = it.second.as<std::string>();

                // Parse cell address like "A1", "B10"
                if (!key.empty() && key[0] >= 'A' && key[0] <= 'Z') {
                    SpreadsheetCellData cd;
                    cd.col = key[0] - 'A';
                    cd.row = std::stoi(key.substr(1)) - 1;
                    cd.value = val;
                    data.cells.push_back(cd);
                }
            }
        }

        auto docResult = YSpreadsheet::create();
        if (!docResult) return Err<YSpreadsheet::Ptr>("Failed to create YSpreadsheet");

        populate(**docResult, data);
        return docResult;

    } catch (const std::exception& e) {
        return Err<YSpreadsheet::Ptr>(std::string("YAML parse error: ") + e.what());
    }
}

std::vector<uint8_t> DocumentPersist::toBinary(const YSpreadsheet& doc) {
    auto data = extract(doc);
    std::stringstream ss;
    msgpack::packer<std::stringstream> pk(ss);

    pk.pack_map(4);
    pk.pack("type");
    pk.pack(data.type);
    pk.pack("rows");
    pk.pack(data.rows);
    pk.pack("cols");
    pk.pack(data.cols);

    pk.pack("cells");
    pk.pack_array(data.cells.size());
    for (const auto& cell : data.cells) {
        pk.pack_array(3);
        pk.pack(cell.row);
        pk.pack(cell.col);
        pk.pack(cell.value);
    }

    std::string str = ss.str();
    return std::vector<uint8_t>(str.begin(), str.end());
}

Result<YSpreadsheet::Ptr> DocumentPersist::fromBinary(const uint8_t* data, size_t len, int) {
    try {
        msgpack::object_handle oh = msgpack::unpack(reinterpret_cast<const char*>(data), len);
        msgpack::object obj = oh.get();

        YSpreadsheetData docData;
        auto map = obj.as<std::map<std::string, msgpack::object>>();

        if (map.count("cells")) {
            auto& arr = map["cells"];
            if (arr.type == msgpack::type::ARRAY) {
                for (size_t i = 0; i < arr.via.array.size; ++i) {
                    auto& cobj = arr.via.array.ptr[i];
                    if (cobj.type == msgpack::type::ARRAY && cobj.via.array.size >= 3) {
                        SpreadsheetCellData cd;
                        cd.row = cobj.via.array.ptr[0].as<int>();
                        cd.col = cobj.via.array.ptr[1].as<int>();
                        cd.value = cobj.via.array.ptr[2].as<std::string>();
                        docData.cells.push_back(cd);
                    }
                }
            }
        }

        auto docResult = YSpreadsheet::create();
        if (!docResult) return Err<YSpreadsheet::Ptr>("Failed to create YSpreadsheet");

        populate(**docResult, docData);
        return docResult;

    } catch (const std::exception& e) {
        return Err<YSpreadsheet::Ptr>(std::string("Binary parse error: ") + e.what());
    }
}

Result<void> DocumentPersist::saveYSpreadsheet(const YSpreadsheet& doc, const std::string& path) {
    Format fmt = detectFormat(path);

    std::ofstream file(path, std::ios::binary);
    if (!file) return Err("Failed to open file for writing: " + path);

    if (fmt == Format::Yaml) {
        std::string yaml = toYaml(doc);
        file.write(yaml.data(), yaml.size());
    } else {
        auto binary = toBinary(doc);
        file.write(reinterpret_cast<const char*>(binary.data()), binary.size());
    }

    return Ok();
}

Result<YSpreadsheet::Ptr> DocumentPersist::loadYSpreadsheet(const std::string& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) return Err<YSpreadsheet::Ptr>("Failed to open file: " + path);

    auto size = file.tellg();
    file.seekg(0);

    std::vector<char> buffer(size);
    file.read(buffer.data(), size);

    Format fmt = detectFormat(path);

    if (fmt == Format::Yaml) {
        return fromYaml(std::string_view(buffer.data(), buffer.size()), 0);
    } else {
        return fromBinary(reinterpret_cast<const uint8_t*>(buffer.data()), buffer.size(), 0);
    }
}

//=============================================================================
// YSlides serialization
//=============================================================================

YSlidesData DocumentPersist::extract(const YSlides& doc) {
    YSlidesData data;
    data.version = 1;
    data.type = "yslides";
    data.modified = currentTimestamp();
    data.slideWidth = doc.slideWidth();
    data.slideHeight = doc.slideHeight();

    for (int i = 0; i < doc.slideCount(); ++i) {
        auto slide = doc.slideAt(i);
        if (!slide) continue;

        SlideData sd;
        sd.index = i;
        sd.bgColor = slide->bgColor().toPacked();

        for (const auto& shape : slide->shapes()) {
            ShapeData shd;
            shd.id = shape->id();
            shd.shapeType = static_cast<int>(shape->shapeType());
            shd.x = shape->bounds().x;
            shd.y = shape->bounds().y;
            shd.width = shape->bounds().w;
            shd.height = shape->bounds().h;
            shd.rotation = shape->rotation();
            shd.cornerRadius = shape->cornerRadius();
            shd.fillColor = shape->fillColor().toPacked();
            shd.strokeColor = shape->strokeColor().toPacked();
            shd.strokeWidth = shape->strokeWidth();

            if (auto textBox = std::dynamic_pointer_cast<TextBox>(shape)) {
                shd.text = textBox->text();
                shd.fontSize = textBox->textStyle().fontSize;
                shd.textColor = textBox->textStyle().color.toPacked();
                shd.textAlign = static_cast<int>(textBox->align());
                shd.textVAlign = static_cast<int>(textBox->valign());
            }

            if (auto imgShape = std::dynamic_pointer_cast<ImageShape>(shape)) {
                shd.imageSource = imgShape->imageData();
            }

            sd.shapes.push_back(shd);
        }

        data.slides.push_back(sd);
    }

    return data;
}

void DocumentPersist::populate(YSlides& doc, const YSlidesData& data) {
    doc.setSlideSize(data.slideWidth, data.slideHeight);

    // Remove default slide
    while (doc.slideCount() > 0) {
        doc.removeSlide(0);
    }

    // Add slides
    for (const auto& sd : data.slides) {
        auto slide = doc.addSlide();
        slide->setBgColor(Color::fromPacked(sd.bgColor));

        doc.setCurrentSlide(doc.slideCount() - 1);

        for (const auto& shd : sd.shapes) {
            ShapePtr shape;

            switch (static_cast<ShapeType>(shd.shapeType)) {
                case ShapeType::Rectangle:
                    shape = doc.addRectangle(shd.x, shd.y, shd.width, shd.height);
                    break;
                case ShapeType::Ellipse:
                    shape = doc.addEllipse(shd.x, shd.y, shd.width, shd.height);
                    break;
                case ShapeType::TextBox: {
                    auto tb = doc.addTextBox(shd.x, shd.y, shd.width, shd.height, shd.text);
                    if (tb) {
                        auto style = tb->textStyle();
                        style.fontSize = shd.fontSize;
                        style.color = Color::fromPacked(shd.textColor);
                        tb->setTextStyle(style);
                        tb->setAlign(static_cast<TextBox::Align>(shd.textAlign));
                        tb->setVAlign(static_cast<TextBox::VAlign>(shd.textVAlign));
                    }
                    shape = tb;
                    break;
                }
                case ShapeType::Line:
                    shape = doc.addLine(shd.x, shd.y, shd.x + shd.width, shd.y + shd.height);
                    break;
                case ShapeType::Image: {
                    auto img = doc.addImage(shd.x, shd.y, shd.width, shd.height);
                    if (img) {
                        img->setImageData(shd.imageSource);
                    }
                    shape = img;
                    break;
                }
                default:
                    break;
            }

            if (shape) {
                shape->setFillColor(Color::fromPacked(shd.fillColor));
                shape->setStrokeColor(Color::fromPacked(shd.strokeColor));
                shape->setStrokeWidth(shd.strokeWidth);
                shape->setRotation(shd.rotation);
                shape->setCornerRadius(shd.cornerRadius);
            }
        }
    }

    if (doc.slideCount() > 0) {
        doc.setCurrentSlide(0);
    }
}

std::string DocumentPersist::toYaml(const YSlides& doc) {
    auto data = extract(doc);
    std::ostringstream os;

    os << "# YSlides Presentation\n";
    os << "version: " << data.version << "\n";
    os << "type: " << data.type << "\n";
    os << "\n";

    os << "metadata:\n";
    os << "  modified: " << data.modified << "\n";
    os << "\n";

    os << "presentation:\n";
    os << "  slideWidth: " << data.slideWidth << "\n";
    os << "  slideHeight: " << data.slideHeight << "\n";
    os << "\n";

    os << "  slides:\n";
    for (const auto& slide : data.slides) {
        os << "    - index: " << slide.index << "\n";
        os << "      bgColor: " << colorToHex(slide.bgColor) << "\n";
        os << "      shapes:\n";

        for (const auto& shape : slide.shapes) {
            os << "        - type: " << shape.shapeType << "\n";
            os << "          x: " << shape.x << "\n";
            os << "          y: " << shape.y << "\n";
            os << "          width: " << shape.width << "\n";
            os << "          height: " << shape.height << "\n";
            os << "          fillColor: " << colorToHex(shape.fillColor) << "\n";
            os << "          strokeColor: " << colorToHex(shape.strokeColor) << "\n";

            if (!shape.text.empty()) {
                os << "          text: " << escapeYaml(shape.text) << "\n";
                os << "          fontSize: " << shape.fontSize << "\n";
            }

            if (!shape.imageSource.empty()) {
                os << "          imageSource: " << escapeYaml(shape.imageSource) << "\n";
            }
        }
    }

    return os.str();
}

Result<YSlides::Ptr> DocumentPersist::fromYamlSlides(std::string_view yaml) {
    try {
        YAML::Node root = YAML::Load(std::string(yaml));

        if (root["type"].as<std::string>("") != "yslides") {
            return Err<YSlides::Ptr>("Not a YSlides document");
        }

        YSlidesData data;
        data.slideWidth = root["presentation"]["slideWidth"].as<float>(960.0f);
        data.slideHeight = root["presentation"]["slideHeight"].as<float>(540.0f);

        if (root["presentation"]["slides"]) {
            for (const auto& snode : root["presentation"]["slides"]) {
                SlideData sd;
                sd.index = snode["index"].as<int>(0);
                sd.bgColor = hexToColor(snode["bgColor"].as<std::string>("#FFFFFFFF"));

                if (snode["shapes"]) {
                    for (const auto& shnode : snode["shapes"]) {
                        ShapeData shd;
                        shd.shapeType = shnode["type"].as<int>(0);
                        shd.x = shnode["x"].as<float>(0);
                        shd.y = shnode["y"].as<float>(0);
                        shd.width = shnode["width"].as<float>(100);
                        shd.height = shnode["height"].as<float>(100);
                        shd.fillColor = hexToColor(shnode["fillColor"].as<std::string>("#FFFFFFFF"));
                        shd.strokeColor = hexToColor(shnode["strokeColor"].as<std::string>("#FF000000"));
                        shd.strokeWidth = shnode["strokeWidth"].as<float>(1.0f);
                        shd.text = shnode["text"].as<std::string>("");
                        shd.fontSize = shnode["fontSize"].as<float>(24.0f);
                        shd.imageSource = shnode["imageSource"].as<std::string>("");

                        sd.shapes.push_back(shd);
                    }
                }
                data.slides.push_back(sd);
            }
        }

        auto docResult = YSlides::create();
        if (!docResult) return Err<YSlides::Ptr>("Failed to create YSlides");

        populate(**docResult, data);
        return docResult;

    } catch (const std::exception& e) {
        return Err<YSlides::Ptr>(std::string("YAML parse error: ") + e.what());
    }
}

std::vector<uint8_t> DocumentPersist::toBinary(const YSlides& doc) {
    auto data = extract(doc);
    std::stringstream ss;
    msgpack::packer<std::stringstream> pk(ss);

    pk.pack_map(4);
    pk.pack("type");
    pk.pack(data.type);
    pk.pack("slideWidth");
    pk.pack(data.slideWidth);
    pk.pack("slideHeight");
    pk.pack(data.slideHeight);

    pk.pack("slides");
    pk.pack_array(data.slides.size());
    for (const auto& slide : data.slides) {
        pk.pack_map(3);
        pk.pack("index");
        pk.pack(slide.index);
        pk.pack("bgColor");
        pk.pack(slide.bgColor);

        pk.pack("shapes");
        pk.pack_array(slide.shapes.size());
        for (const auto& shape : slide.shapes) {
            pk.pack_array(12);
            pk.pack(shape.shapeType);
            pk.pack(shape.x);
            pk.pack(shape.y);
            pk.pack(shape.width);
            pk.pack(shape.height);
            pk.pack(shape.fillColor);
            pk.pack(shape.strokeColor);
            pk.pack(shape.strokeWidth);
            pk.pack(shape.text);
            pk.pack(shape.fontSize);
            pk.pack(shape.textColor);
            pk.pack(shape.imageSource);
        }
    }

    std::string str = ss.str();
    return std::vector<uint8_t>(str.begin(), str.end());
}

Result<YSlides::Ptr> DocumentPersist::fromBinarySlides(const uint8_t* data, size_t len) {
    try {
        msgpack::object_handle oh = msgpack::unpack(reinterpret_cast<const char*>(data), len);
        msgpack::object obj = oh.get();

        YSlidesData docData;
        auto map = obj.as<std::map<std::string, msgpack::object>>();

        if (map.count("slideWidth")) docData.slideWidth = map["slideWidth"].as<float>();
        if (map.count("slideHeight")) docData.slideHeight = map["slideHeight"].as<float>();

        if (map.count("slides")) {
            auto& arr = map["slides"];
            if (arr.type == msgpack::type::ARRAY) {
                for (size_t i = 0; i < arr.via.array.size; ++i) {
                    auto& sobj = arr.via.array.ptr[i];
                    SlideData sd;
                    auto smap = sobj.as<std::map<std::string, msgpack::object>>();

                    if (smap.count("index")) sd.index = smap["index"].as<int>();
                    if (smap.count("bgColor")) sd.bgColor = smap["bgColor"].as<uint32_t>();

                    if (smap.count("shapes")) {
                        auto& shapes = smap["shapes"];
                        if (shapes.type == msgpack::type::ARRAY) {
                            for (size_t j = 0; j < shapes.via.array.size; ++j) {
                                auto& shobj = shapes.via.array.ptr[j];
                                if (shobj.type == msgpack::type::ARRAY && shobj.via.array.size >= 12) {
                                    ShapeData shd;
                                    shd.shapeType = shobj.via.array.ptr[0].as<int>();
                                    shd.x = shobj.via.array.ptr[1].as<float>();
                                    shd.y = shobj.via.array.ptr[2].as<float>();
                                    shd.width = shobj.via.array.ptr[3].as<float>();
                                    shd.height = shobj.via.array.ptr[4].as<float>();
                                    shd.fillColor = shobj.via.array.ptr[5].as<uint32_t>();
                                    shd.strokeColor = shobj.via.array.ptr[6].as<uint32_t>();
                                    shd.strokeWidth = shobj.via.array.ptr[7].as<float>();
                                    shd.text = shobj.via.array.ptr[8].as<std::string>();
                                    shd.fontSize = shobj.via.array.ptr[9].as<float>();
                                    shd.textColor = shobj.via.array.ptr[10].as<uint32_t>();
                                    shd.imageSource = shobj.via.array.ptr[11].as<std::string>();
                                    sd.shapes.push_back(shd);
                                }
                            }
                        }
                    }
                    docData.slides.push_back(sd);
                }
            }
        }

        auto docResult = YSlides::create();
        if (!docResult) return Err<YSlides::Ptr>("Failed to create YSlides");

        populate(**docResult, docData);
        return docResult;

    } catch (const std::exception& e) {
        return Err<YSlides::Ptr>(std::string("Binary parse error: ") + e.what());
    }
}

Result<void> DocumentPersist::saveYSlides(const YSlides& doc, const std::string& path) {
    Format fmt = detectFormat(path);

    std::ofstream file(path, std::ios::binary);
    if (!file) return Err("Failed to open file for writing: " + path);

    if (fmt == Format::Yaml) {
        std::string yaml = toYaml(doc);
        file.write(yaml.data(), yaml.size());
    } else {
        auto binary = toBinary(doc);
        file.write(reinterpret_cast<const char*>(binary.data()), binary.size());
    }

    return Ok();
}

Result<YSlides::Ptr> DocumentPersist::loadYSlides(const std::string& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file) return Err<YSlides::Ptr>("Failed to open file: " + path);

    auto size = file.tellg();
    file.seekg(0);

    std::vector<char> buffer(size);
    file.read(buffer.data(), size);

    Format fmt = detectFormat(path);

    if (fmt == Format::Yaml) {
        return fromYamlSlides(std::string_view(buffer.data(), buffer.size()));
    } else {
        return fromBinarySlides(reinterpret_cast<const uint8_t*>(buffer.data()), buffer.size());
    }
}

//=============================================================================
// Data-only serialization (for testing)
//=============================================================================

std::string DocumentPersist::dataToYaml(const YDocData& data) {
    std::ostringstream os;

    os << "# YDoc Document\n";
    os << "version: " << data.version << "\n";
    os << "type: " << data.type << "\n";
    os << "\n";

    os << "metadata:\n";
    os << "  modified: " << data.modified << "\n";
    if (!data.author.empty()) {
        os << "  author: " << escapeYaml(data.author) << "\n";
    }
    os << "\n";

    os << "document:\n";
    os << "  pageWidth: " << data.pageWidth << "\n";
    os << "  margin: " << data.margin << "\n";
    os << "\n";

    // Paragraphs
    if (!data.paragraphs.empty()) {
        os << "  paragraphs:\n";
        for (const auto& para : data.paragraphs) {
            os << "    - text: " << escapeYaml(para.text) << "\n";
            if (para.fontSize != 14.0f) {
                os << "      fontSize: " << para.fontSize << "\n";
            }
            if (para.color != 0xFF000000) {
                os << "      color: " << colorToHex(para.color) << "\n";
            }
            if (!para.runs.empty()) {
                os << "      runs:\n";
                for (const auto& run : para.runs) {
                    os << "        - start: " << run.start << "\n";
                    os << "          end: " << run.end << "\n";
                    if (run.format != 0) {
                        os << "          format: " << run.format << "\n";
                    }
                    if (run.fontSize != para.fontSize) {
                        os << "          fontSize: " << run.fontSize << "\n";
                    }
                    if (run.color != para.color) {
                        os << "          color: " << colorToHex(run.color) << "\n";
                    }
                }
            }
        }
        os << "\n";
    }

    return os.str();
}

Result<YDocData> DocumentPersist::parseYDocYaml(std::string_view yaml) {
    try {
        YAML::Node root = YAML::Load(std::string(yaml));

        if (root["type"].as<std::string>("") != "ydoc") {
            return Err<YDocData>("Not a YDoc document");
        }

        YDocData data;
        data.version = root["version"].as<int>(1);
        data.pageWidth = root["document"]["pageWidth"].as<float>(600.0f);
        data.margin = root["document"]["margin"].as<float>(20.0f);

        if (root["metadata"]) {
            data.author = root["metadata"]["author"].as<std::string>("");
            data.modified = root["metadata"]["modified"].as<uint64_t>(0);
        }

        // Paragraphs
        if (root["document"]["paragraphs"]) {
            for (const auto& pnode : root["document"]["paragraphs"]) {
                ParagraphData pd;
                pd.text = pnode["text"].as<std::string>("");
                pd.fontSize = pnode["fontSize"].as<float>(14.0f);
                pd.color = hexToColor(pnode["color"].as<std::string>("#FF000000"));

                if (pnode["runs"]) {
                    for (const auto& rnode : pnode["runs"]) {
                        TextRunData rd;
                        rd.start = rnode["start"].as<int>(0);
                        rd.end = rnode["end"].as<int>(0);
                        rd.format = rnode["format"].as<uint32_t>(0);
                        rd.fontSize = rnode["fontSize"].as<float>(pd.fontSize);
                        rd.color = hexToColor(rnode["color"].as<std::string>(colorToHex(pd.color)));
                        pd.runs.push_back(rd);
                    }
                }
                data.paragraphs.push_back(pd);
            }
        }

        return Ok(data);

    } catch (const std::exception& e) {
        return Err<YDocData>(std::string("YAML parse error: ") + e.what());
    }
}

std::vector<uint8_t> DocumentPersist::dataToBinary(const YDocData& data) {
    std::stringstream ss;
    msgpack::packer<std::stringstream> pk(ss);

    pk.pack_map(6);

    pk.pack("version");
    pk.pack(data.version);

    pk.pack("type");
    pk.pack(data.type);

    pk.pack("pageWidth");
    pk.pack(data.pageWidth);

    pk.pack("margin");
    pk.pack(data.margin);

    pk.pack("modified");
    pk.pack(data.modified);

    // Paragraphs
    pk.pack("paragraphs");
    pk.pack_array(data.paragraphs.size());
    for (const auto& para : data.paragraphs) {
        pk.pack_map(4);
        pk.pack("text");
        pk.pack(para.text);
        pk.pack("fontSize");
        pk.pack(para.fontSize);
        pk.pack("color");
        pk.pack(para.color);
        pk.pack("runs");
        pk.pack_array(para.runs.size());
        for (const auto& run : para.runs) {
            pk.pack_array(5);
            pk.pack(run.start);
            pk.pack(run.end);
            pk.pack(run.format);
            pk.pack(run.fontSize);
            pk.pack(run.color);
        }
    }

    std::string str = ss.str();
    return std::vector<uint8_t>(str.begin(), str.end());
}

Result<YDocData> DocumentPersist::parseYDocBinary(const uint8_t* data, size_t len) {
    try {
        msgpack::object_handle oh = msgpack::unpack(reinterpret_cast<const char*>(data), len);
        msgpack::object obj = oh.get();

        if (obj.type != msgpack::type::MAP) {
            return Err<YDocData>("Invalid binary format");
        }

        YDocData docData;
        docData.version = 1;

        auto map = obj.as<std::map<std::string, msgpack::object>>();

        if (map.count("pageWidth")) docData.pageWidth = map["pageWidth"].as<float>();
        if (map.count("margin")) docData.margin = map["margin"].as<float>();

        if (map.count("paragraphs")) {
            auto& arr = map["paragraphs"];
            if (arr.type == msgpack::type::ARRAY) {
                for (size_t i = 0; i < arr.via.array.size; ++i) {
                    auto& pobj = arr.via.array.ptr[i];
                    ParagraphData pd;

                    auto pmap = pobj.as<std::map<std::string, msgpack::object>>();
                    if (pmap.count("text")) pd.text = pmap["text"].as<std::string>();
                    if (pmap.count("fontSize")) pd.fontSize = pmap["fontSize"].as<float>();
                    if (pmap.count("color")) pd.color = pmap["color"].as<uint32_t>();

                    if (pmap.count("runs")) {
                        auto& runs = pmap["runs"];
                        if (runs.type == msgpack::type::ARRAY) {
                            for (size_t j = 0; j < runs.via.array.size; ++j) {
                                auto& robj = runs.via.array.ptr[j];
                                if (robj.type == msgpack::type::ARRAY && robj.via.array.size >= 5) {
                                    TextRunData rd;
                                    rd.start = robj.via.array.ptr[0].as<int>();
                                    rd.end = robj.via.array.ptr[1].as<int>();
                                    rd.format = robj.via.array.ptr[2].as<uint32_t>();
                                    rd.fontSize = robj.via.array.ptr[3].as<float>();
                                    rd.color = robj.via.array.ptr[4].as<uint32_t>();
                                    pd.runs.push_back(rd);
                                }
                            }
                        }
                    }
                    docData.paragraphs.push_back(pd);
                }
            }
        }

        return Ok(docData);

    } catch (const std::exception& e) {
        return Err<YDocData>(std::string("Binary parse error: ") + e.what());
    }
}

// YSpreadsheet data-only serialization

std::string DocumentPersist::dataToYaml(const YSpreadsheetData& data) {
    std::ostringstream os;

    os << "# YSpreadsheet Document\n";
    os << "version: " << data.version << "\n";
    os << "type: " << data.type << "\n";
    os << "\n";

    os << "metadata:\n";
    os << "  modified: " << data.modified << "\n";
    os << "\n";

    os << "spreadsheet:\n";
    os << "  rows: " << data.rows << "\n";
    os << "  cols: " << data.cols << "\n";
    os << "\n";

    if (!data.columnWidths.empty()) {
        os << "  columnWidths: [";
        for (size_t i = 0; i < data.columnWidths.size(); ++i) {
            if (i > 0) os << ", ";
            os << data.columnWidths[i];
        }
        os << "]\n\n";
    }

    if (!data.cells.empty()) {
        os << "  cells:\n";
        for (const auto& cell : data.cells) {
            char colLetter = 'A' + cell.col;
            os << "    " << colLetter << (cell.row + 1) << ": " << escapeYaml(cell.value) << "\n";
        }
    }

    return os.str();
}

Result<YSpreadsheetData> DocumentPersist::parseYSpreadsheetYaml(std::string_view yaml) {
    try {
        YAML::Node root = YAML::Load(std::string(yaml));

        if (root["type"].as<std::string>("") != "yspreadsheet") {
            return Err<YSpreadsheetData>("Not a YSpreadsheet document");
        }

        YSpreadsheetData data;

        if (root["spreadsheet"]["rows"]) {
            data.rows = root["spreadsheet"]["rows"].as<int>(100);
        }
        if (root["spreadsheet"]["cols"]) {
            data.cols = root["spreadsheet"]["cols"].as<int>(26);
        }

        if (root["spreadsheet"]["columnWidths"]) {
            for (const auto& w : root["spreadsheet"]["columnWidths"]) {
                data.columnWidths.push_back(w.as<float>(80.0f));
            }
        }

        if (root["spreadsheet"]["cells"]) {
            for (const auto& it : root["spreadsheet"]["cells"]) {
                std::string key = it.first.as<std::string>();
                std::string val = it.second.as<std::string>();

                // Parse cell address like "A1", "B10"
                if (!key.empty() && key[0] >= 'A' && key[0] <= 'Z') {
                    SpreadsheetCellData cd;
                    cd.col = key[0] - 'A';
                    cd.row = std::stoi(key.substr(1)) - 1;
                    cd.value = val;
                    data.cells.push_back(cd);
                }
            }
        }

        return Ok(data);

    } catch (const std::exception& e) {
        return Err<YSpreadsheetData>(std::string("YAML parse error: ") + e.what());
    }
}

std::vector<uint8_t> DocumentPersist::dataToBinary(const YSpreadsheetData& data) {
    std::stringstream ss;
    msgpack::packer<std::stringstream> pk(ss);

    pk.pack_map(4);
    pk.pack("type");
    pk.pack(data.type);
    pk.pack("rows");
    pk.pack(data.rows);
    pk.pack("cols");
    pk.pack(data.cols);

    pk.pack("cells");
    pk.pack_array(data.cells.size());
    for (const auto& cell : data.cells) {
        pk.pack_array(3);
        pk.pack(cell.row);
        pk.pack(cell.col);
        pk.pack(cell.value);
    }

    std::string str = ss.str();
    return std::vector<uint8_t>(str.begin(), str.end());
}

Result<YSpreadsheetData> DocumentPersist::parseYSpreadsheetBinary(const uint8_t* data, size_t len) {
    try {
        msgpack::object_handle oh = msgpack::unpack(reinterpret_cast<const char*>(data), len);
        msgpack::object obj = oh.get();

        YSpreadsheetData docData;
        auto map = obj.as<std::map<std::string, msgpack::object>>();

        if (map.count("rows")) docData.rows = map["rows"].as<int>();
        if (map.count("cols")) docData.cols = map["cols"].as<int>();

        if (map.count("cells")) {
            auto& arr = map["cells"];
            if (arr.type == msgpack::type::ARRAY) {
                for (size_t i = 0; i < arr.via.array.size; ++i) {
                    auto& cobj = arr.via.array.ptr[i];
                    if (cobj.type == msgpack::type::ARRAY && cobj.via.array.size >= 3) {
                        SpreadsheetCellData cd;
                        cd.row = cobj.via.array.ptr[0].as<int>();
                        cd.col = cobj.via.array.ptr[1].as<int>();
                        cd.value = cobj.via.array.ptr[2].as<std::string>();
                        docData.cells.push_back(cd);
                    }
                }
            }
        }

        return Ok(docData);

    } catch (const std::exception& e) {
        return Err<YSpreadsheetData>(std::string("Binary parse error: ") + e.what());
    }
}

// YSlides data-only serialization

std::string DocumentPersist::dataToYaml(const YSlidesData& data) {
    std::ostringstream os;

    os << "# YSlides Presentation\n";
    os << "version: " << data.version << "\n";
    os << "type: " << data.type << "\n";
    os << "\n";

    os << "metadata:\n";
    os << "  modified: " << data.modified << "\n";
    os << "\n";

    os << "presentation:\n";
    os << "  slideWidth: " << data.slideWidth << "\n";
    os << "  slideHeight: " << data.slideHeight << "\n";
    os << "\n";

    os << "  slides:\n";
    for (const auto& slide : data.slides) {
        os << "    - index: " << slide.index << "\n";
        os << "      bgColor: " << colorToHex(slide.bgColor) << "\n";
        os << "      shapes:\n";

        for (const auto& shape : slide.shapes) {
            os << "        - type: " << shape.shapeType << "\n";
            os << "          x: " << shape.x << "\n";
            os << "          y: " << shape.y << "\n";
            os << "          width: " << shape.width << "\n";
            os << "          height: " << shape.height << "\n";
            os << "          fillColor: " << colorToHex(shape.fillColor) << "\n";
            os << "          strokeColor: " << colorToHex(shape.strokeColor) << "\n";

            if (!shape.text.empty()) {
                os << "          text: " << escapeYaml(shape.text) << "\n";
                os << "          fontSize: " << shape.fontSize << "\n";
            }

            if (!shape.imageSource.empty()) {
                os << "          imageSource: " << escapeYaml(shape.imageSource) << "\n";
            }
        }
    }

    return os.str();
}

Result<YSlidesData> DocumentPersist::parseYSlidesYaml(std::string_view yaml) {
    try {
        YAML::Node root = YAML::Load(std::string(yaml));

        if (root["type"].as<std::string>("") != "yslides") {
            return Err<YSlidesData>("Not a YSlides document");
        }

        YSlidesData data;
        data.slideWidth = root["presentation"]["slideWidth"].as<float>(960.0f);
        data.slideHeight = root["presentation"]["slideHeight"].as<float>(540.0f);

        if (root["presentation"]["slides"]) {
            for (const auto& snode : root["presentation"]["slides"]) {
                SlideData sd;
                sd.index = snode["index"].as<int>(0);
                sd.bgColor = hexToColor(snode["bgColor"].as<std::string>("#FFFFFFFF"));

                if (snode["shapes"]) {
                    for (const auto& shnode : snode["shapes"]) {
                        ShapeData shd;
                        shd.shapeType = shnode["type"].as<int>(0);
                        shd.x = shnode["x"].as<float>(0);
                        shd.y = shnode["y"].as<float>(0);
                        shd.width = shnode["width"].as<float>(100);
                        shd.height = shnode["height"].as<float>(100);
                        shd.fillColor = hexToColor(shnode["fillColor"].as<std::string>("#FFFFFFFF"));
                        shd.strokeColor = hexToColor(shnode["strokeColor"].as<std::string>("#FF000000"));
                        shd.strokeWidth = shnode["strokeWidth"].as<float>(1.0f);
                        shd.text = shnode["text"].as<std::string>("");
                        shd.fontSize = shnode["fontSize"].as<float>(24.0f);
                        shd.imageSource = shnode["imageSource"].as<std::string>("");

                        sd.shapes.push_back(shd);
                    }
                }
                data.slides.push_back(sd);
            }
        }

        return Ok(data);

    } catch (const std::exception& e) {
        return Err<YSlidesData>(std::string("YAML parse error: ") + e.what());
    }
}

std::vector<uint8_t> DocumentPersist::dataToBinary(const YSlidesData& data) {
    std::stringstream ss;
    msgpack::packer<std::stringstream> pk(ss);

    pk.pack_map(4);
    pk.pack("type");
    pk.pack(data.type);
    pk.pack("slideWidth");
    pk.pack(data.slideWidth);
    pk.pack("slideHeight");
    pk.pack(data.slideHeight);

    pk.pack("slides");
    pk.pack_array(data.slides.size());
    for (const auto& slide : data.slides) {
        pk.pack_map(3);
        pk.pack("index");
        pk.pack(slide.index);
        pk.pack("bgColor");
        pk.pack(slide.bgColor);

        pk.pack("shapes");
        pk.pack_array(slide.shapes.size());
        for (const auto& shape : slide.shapes) {
            pk.pack_array(12);
            pk.pack(shape.shapeType);
            pk.pack(shape.x);
            pk.pack(shape.y);
            pk.pack(shape.width);
            pk.pack(shape.height);
            pk.pack(shape.fillColor);
            pk.pack(shape.strokeColor);
            pk.pack(shape.strokeWidth);
            pk.pack(shape.text);
            pk.pack(shape.fontSize);
            pk.pack(shape.textColor);
            pk.pack(shape.imageSource);
        }
    }

    std::string str = ss.str();
    return std::vector<uint8_t>(str.begin(), str.end());
}

Result<YSlidesData> DocumentPersist::parseYSlidesBinary(const uint8_t* data, size_t len) {
    try {
        msgpack::object_handle oh = msgpack::unpack(reinterpret_cast<const char*>(data), len);
        msgpack::object obj = oh.get();

        YSlidesData docData;
        auto map = obj.as<std::map<std::string, msgpack::object>>();

        if (map.count("slideWidth")) docData.slideWidth = map["slideWidth"].as<float>();
        if (map.count("slideHeight")) docData.slideHeight = map["slideHeight"].as<float>();

        if (map.count("slides")) {
            auto& arr = map["slides"];
            if (arr.type == msgpack::type::ARRAY) {
                for (size_t i = 0; i < arr.via.array.size; ++i) {
                    auto& sobj = arr.via.array.ptr[i];
                    SlideData sd;
                    auto smap = sobj.as<std::map<std::string, msgpack::object>>();

                    if (smap.count("index")) sd.index = smap["index"].as<int>();
                    if (smap.count("bgColor")) sd.bgColor = smap["bgColor"].as<uint32_t>();

                    if (smap.count("shapes")) {
                        auto& shapes = smap["shapes"];
                        if (shapes.type == msgpack::type::ARRAY) {
                            for (size_t j = 0; j < shapes.via.array.size; ++j) {
                                auto& shobj = shapes.via.array.ptr[j];
                                if (shobj.type == msgpack::type::ARRAY && shobj.via.array.size >= 12) {
                                    ShapeData shd;
                                    shd.shapeType = shobj.via.array.ptr[0].as<int>();
                                    shd.x = shobj.via.array.ptr[1].as<float>();
                                    shd.y = shobj.via.array.ptr[2].as<float>();
                                    shd.width = shobj.via.array.ptr[3].as<float>();
                                    shd.height = shobj.via.array.ptr[4].as<float>();
                                    shd.fillColor = shobj.via.array.ptr[5].as<uint32_t>();
                                    shd.strokeColor = shobj.via.array.ptr[6].as<uint32_t>();
                                    shd.strokeWidth = shobj.via.array.ptr[7].as<float>();
                                    shd.text = shobj.via.array.ptr[8].as<std::string>();
                                    shd.fontSize = shobj.via.array.ptr[9].as<float>();
                                    shd.textColor = shobj.via.array.ptr[10].as<uint32_t>();
                                    shd.imageSource = shobj.via.array.ptr[11].as<std::string>();
                                    sd.shapes.push_back(shd);
                                }
                            }
                        }
                    }
                    docData.slides.push_back(sd);
                }
            }
        }

        return Ok(docData);

    } catch (const std::exception& e) {
        return Err<YSlidesData>(std::string("Binary parse error: ") + e.what());
    }
}

//=============================================================================
// Format detection
//=============================================================================

DocumentPersist::DocType DocumentPersist::detectType(const std::string& path) {
    if (path.find(".ydoc") != std::string::npos) return DocType::YDoc;
    if (path.find(".ysheet") != std::string::npos) return DocType::YSpreadsheet;
    if (path.find(".yslides") != std::string::npos) return DocType::YSlides;
    return DocType::Unknown;
}

DocumentPersist::Format DocumentPersist::detectFormat(const std::string& path) {
    if (path.size() >= 5 && path.substr(path.size() - 5) == ".yaml") {
        return Format::Yaml;
    }
    if (path.size() >= 4 && path.substr(path.size() - 4) == ".yml") {
        return Format::Yaml;
    }
    return Format::Binary;
}

} // namespace yetty::yrich
