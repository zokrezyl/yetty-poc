#pragma once

#include "ydoc.h"
#include "yspreadsheet.h"
#include "yslides.h"
#include <string>
#include <vector>

namespace yetty::yrich {

//=============================================================================
// Document file extensions
//=============================================================================
constexpr const char* YDOC_EXT_YAML = ".ydoc.yaml";
constexpr const char* YDOC_EXT_BIN = ".ydoc";
constexpr const char* YSHEET_EXT_YAML = ".ysheet.yaml";
constexpr const char* YSHEET_EXT_BIN = ".ysheet";
constexpr const char* YSLIDES_EXT_YAML = ".yslides.yaml";
constexpr const char* YSLIDES_EXT_BIN = ".yslides";

//=============================================================================
// Common data structures for serialization
//=============================================================================

struct TextRunData {
    int start = 0;
    int end = 0;
    float fontSize = 14.0f;
    uint32_t color = 0xFF000000;  // ARGB
    uint32_t bgColor = 0x00000000;
    uint32_t format = 0;  // TextFormat flags
    int fontId = 0;
};

struct ParagraphData {
    std::string text;
    std::vector<TextRunData> runs;
    float fontSize = 14.0f;
    uint32_t color = 0xFF000000;
};

struct TableCellData {
    std::string text;
    uint32_t bgColor = 0xFFFFFFFF;
    uint32_t textColor = 0xFF000000;
};

struct TableData {
    int rows = 0;
    int cols = 0;
    std::vector<float> columnWidths;
    std::vector<float> rowHeights;
    std::vector<TableCellData> cells;  // row-major order
};

struct ImageData {
    float x = 0, y = 0;
    float width = 100, height = 100;
    std::string source;  // base64 or path
    std::string altText;
    std::string caption;
    int align = 1;  // 0=left, 1=center, 2=right
};

struct CommentData {
    uint64_t id = 0;
    int paragraphIndex = 0;
    int startPos = 0;
    int endPos = 0;
    std::string text;
    std::string author;
    uint64_t timestamp = 0;
    bool resolved = false;
    std::vector<CommentData> replies;
};

struct VersionData {
    uint64_t timestamp = 0;
    std::string author;
    std::string description;
    uint64_t operationIndex = 0;
};

//=============================================================================
// YDoc data
//=============================================================================
struct YDocData {
    int version = 1;
    std::string type = "ydoc";

    // Metadata
    uint64_t created = 0;
    uint64_t modified = 0;
    std::string author;

    // Document settings
    float pageWidth = 600.0f;
    float margin = 20.0f;

    // Content
    std::vector<ParagraphData> paragraphs;
    std::vector<TableData> tables;
    std::vector<ImageData> images;
    std::vector<CommentData> comments;
    std::vector<VersionData> versions;
};

//=============================================================================
// YSpreadsheet data
//=============================================================================
struct CellStyleData {
    uint32_t bgColor = 0xFFFFFFFF;
    uint32_t textColor = 0xFF000000;
    uint32_t format = 0;  // TextFormat flags
    float fontSize = 12.0f;
    int align = 0;  // 0=left, 1=center, 2=right
};

struct SpreadsheetCellData {
    int row = 0;
    int col = 0;
    std::string value;
    std::string formula;  // if starts with '='
    CellStyleData style;
};

struct YSpreadsheetData {
    int version = 1;
    std::string type = "yspreadsheet";

    // Metadata
    uint64_t created = 0;
    uint64_t modified = 0;
    std::string author;

    // Grid settings
    int rows = 100;
    int cols = 26;
    std::vector<float> columnWidths;
    std::vector<float> rowHeights;

    // Cells (sparse - only non-empty)
    std::vector<SpreadsheetCellData> cells;

    // Default styles
    CellStyleData defaultStyle;
};

//=============================================================================
// YSlides data
//=============================================================================
struct ShapeData {
    uint64_t id = 0;
    int shapeType = 0;  // ShapeType enum
    float x = 0, y = 0;
    float width = 100, height = 100;
    float rotation = 0;
    float cornerRadius = 0;
    uint32_t fillColor = 0xFFFFFFFF;
    uint32_t strokeColor = 0xFF000000;
    float strokeWidth = 1.0f;

    // For TextBox
    std::string text;
    float fontSize = 24.0f;
    uint32_t textColor = 0xFF000000;
    int textAlign = 1;   // 0=left, 1=center, 2=right
    int textVAlign = 1;  // 0=top, 1=middle, 2=bottom

    // For ImageShape
    std::string imageSource;
};

struct SlideData {
    int index = 0;
    uint32_t bgColor = 0xFFFFFFFF;
    std::vector<ShapeData> shapes;
};

struct YSlidesData {
    int version = 1;
    std::string type = "yslides";

    // Metadata
    uint64_t created = 0;
    uint64_t modified = 0;
    std::string author;

    // Slide dimensions
    float slideWidth = 960.0f;
    float slideHeight = 540.0f;

    // Slides
    std::vector<SlideData> slides;
};

//=============================================================================
// DocumentPersist - serialization/deserialization for all document types
//=============================================================================
class DocumentPersist {
public:
    //=========================================================================
    // YDoc
    //=========================================================================

    // Extract data from document
    static YDocData extract(const YDoc& doc);

    // Populate document from data
    static void populate(YDoc& doc, const YDocData& data);

    // YAML
    static std::string toYaml(const YDoc& doc);
    static Result<YDoc::Ptr> fromYaml(std::string_view yaml);

    // Binary (msgpack)
    static std::vector<uint8_t> toBinary(const YDoc& doc);
    static Result<YDoc::Ptr> fromBinary(const uint8_t* data, size_t len);

    // File I/O
    static Result<void> saveYDoc(const YDoc& doc, const std::string& path);
    static Result<YDoc::Ptr> loadYDoc(const std::string& path);

    //=========================================================================
    // YSpreadsheet
    //=========================================================================

    static YSpreadsheetData extract(const YSpreadsheet& doc);
    static void populate(YSpreadsheet& doc, const YSpreadsheetData& data);

    static std::string toYaml(const YSpreadsheet& doc);
    static Result<YSpreadsheet::Ptr> fromYaml(std::string_view yaml, int dummy);  // dummy to disambiguate

    static std::vector<uint8_t> toBinary(const YSpreadsheet& doc);
    static Result<YSpreadsheet::Ptr> fromBinary(const uint8_t* data, size_t len, int dummy);

    static Result<void> saveYSpreadsheet(const YSpreadsheet& doc, const std::string& path);
    static Result<YSpreadsheet::Ptr> loadYSpreadsheet(const std::string& path);

    //=========================================================================
    // YSlides
    //=========================================================================

    static YSlidesData extract(const YSlides& doc);
    static void populate(YSlides& doc, const YSlidesData& data);

    static std::string toYaml(const YSlides& doc);
    static Result<YSlides::Ptr> fromYamlSlides(std::string_view yaml);

    static std::vector<uint8_t> toBinary(const YSlides& doc);
    static Result<YSlides::Ptr> fromBinarySlides(const uint8_t* data, size_t len);

    static Result<void> saveYSlides(const YSlides& doc, const std::string& path);
    static Result<YSlides::Ptr> loadYSlides(const std::string& path);

    //=========================================================================
    // Auto-detect format from file extension
    //=========================================================================

    enum class DocType { Unknown, YDoc, YSpreadsheet, YSlides };
    enum class Format { Unknown, Yaml, Binary };

    static DocType detectType(const std::string& path);
    static Format detectFormat(const std::string& path);

};

} // namespace yetty::yrich
