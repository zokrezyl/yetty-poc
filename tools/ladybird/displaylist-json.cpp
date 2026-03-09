// JSON-based DisplayList serialization
// Simple JSON parser for IPC - no external dependencies

#include "displaylist-json.h"
#include <sstream>
#include <cctype>
#include <stdexcept>
#include <cstring>

namespace displaylist {

namespace {

// Simple JSON tokenizer
class JSONParser {
public:
    explicit JSONParser(const std::string& json) : _json(json), _pos(0) {}

    void skipWhitespace() {
        while (_pos < _json.size() && std::isspace(_json[_pos])) ++_pos;
    }

    char peek() {
        skipWhitespace();
        return _pos < _json.size() ? _json[_pos] : '\0';
    }

    char next() {
        skipWhitespace();
        return _pos < _json.size() ? _json[_pos++] : '\0';
    }

    void expect(char c) {
        if (next() != c) {
            throw std::runtime_error(std::string("Expected '") + c + "'");
        }
    }

    std::string parseString() {
        expect('"');
        std::string result;
        while (_pos < _json.size() && _json[_pos] != '"') {
            if (_json[_pos] == '\\' && _pos + 1 < _json.size()) {
                ++_pos;
                switch (_json[_pos]) {
                    case 'n': result += '\n'; break;
                    case 't': result += '\t'; break;
                    case 'r': result += '\r'; break;
                    case '"': result += '"'; break;
                    case '\\': result += '\\'; break;
                    default: result += _json[_pos]; break;
                }
            } else {
                result += _json[_pos];
            }
            ++_pos;
        }
        expect('"');
        return result;
    }

    int parseInt() {
        skipWhitespace();
        bool neg = false;
        if (_json[_pos] == '-') {
            neg = true;
            ++_pos;
        }
        int value = 0;
        while (_pos < _json.size() && std::isdigit(_json[_pos])) {
            value = value * 10 + (_json[_pos] - '0');
            ++_pos;
        }
        return neg ? -value : value;
    }

    float parseFloat() {
        skipWhitespace();
        size_t start = _pos;
        if (_json[_pos] == '-') ++_pos;
        while (_pos < _json.size() && (std::isdigit(_json[_pos]) || _json[_pos] == '.')) ++_pos;
        return std::stof(_json.substr(start, _pos - start));
    }

    bool parseBool() {
        skipWhitespace();
        if (_json.substr(_pos, 4) == "true") {
            _pos += 4;
            return true;
        } else if (_json.substr(_pos, 5) == "false") {
            _pos += 5;
            return false;
        }
        throw std::runtime_error("Expected boolean");
    }

    void parseNull() {
        skipWhitespace();
        if (_json.substr(_pos, 4) == "null") {
            _pos += 4;
        } else {
            throw std::runtime_error("Expected null");
        }
    }

    Color parseColor() {
        Color c;
        expect('{');
        while (peek() != '}') {
            std::string key = parseString();
            expect(':');
            if (key == "r") c.r = parseInt();
            else if (key == "g") c.g = parseInt();
            else if (key == "b") c.b = parseInt();
            else if (key == "a") c.a = parseInt();
            if (peek() == ',') next();
        }
        expect('}');
        return c;
    }

    Point parsePoint() {
        Point p;
        expect('{');
        while (peek() != '}') {
            std::string key = parseString();
            expect(':');
            if (key == "x") p.x = parseInt();
            else if (key == "y") p.y = parseInt();
            if (peek() == ',') next();
        }
        expect('}');
        return p;
    }

    FloatPoint parseFloatPoint() {
        FloatPoint p;
        expect('{');
        while (peek() != '}') {
            std::string key = parseString();
            expect(':');
            if (key == "x") p.x = parseFloat();
            else if (key == "y") p.y = parseFloat();
            if (peek() == ',') next();
        }
        expect('}');
        return p;
    }

    Rect parseRect() {
        Rect r;
        expect('{');
        while (peek() != '}') {
            std::string key = parseString();
            expect(':');
            if (key == "x") r.x = parseInt();
            else if (key == "y") r.y = parseInt();
            else if (key == "width") r.width = parseInt();
            else if (key == "height") r.height = parseInt();
            if (peek() == ',') next();
        }
        expect('}');
        return r;
    }

    CornerRadii parseCornerRadii() {
        CornerRadii c;
        expect('{');
        while (peek() != '}') {
            std::string key = parseString();
            expect(':');
            if (key == "top_left") c.top_left = parseFloat();
            else if (key == "top_right") c.top_right = parseFloat();
            else if (key == "bottom_left") c.bottom_left = parseFloat();
            else if (key == "bottom_right") c.bottom_right = parseFloat();
            if (peek() == ',') next();
        }
        expect('}');
        return c;
    }

    Command parseCommand() {
        expect('{');
        std::string type;

        // First find the type
        size_t saved = _pos;
        while (peek() != '}') {
            std::string key = parseString();
            expect(':');
            if (key == "type") {
                type = parseString();
                break;
            }
            // Skip value
            skipValue();
            if (peek() == ',') next();
        }
        _pos = saved;

        // Now parse based on type
        Command cmd;
        if (type == "FillRect") {
            FillRect c;
            parseCommandFields([&](const std::string& key) {
                if (key == "rect") c.rect = parseRect();
                else if (key == "color") c.color = parseColor();
                else skipValue();
            });
            cmd = c;
        } else if (type == "FillRectWithRoundedCorners") {
            FillRectWithRoundedCorners c;
            parseCommandFields([&](const std::string& key) {
                if (key == "rect") c.rect = parseRect();
                else if (key == "color") c.color = parseColor();
                else if (key == "radii") c.radii = parseCornerRadii();
                else skipValue();
            });
            cmd = c;
        } else if (type == "DrawGlyphRun") {
            DrawGlyphRun c;
            parseCommandFields([&](const std::string& key) {
                if (key == "rect") c.rect = parseRect();
                else if (key == "translation") c.translation = parseFloatPoint();
                else if (key == "color") c.color = parseColor();
                else if (key == "text") c.text = parseString();
                else if (key == "font_size") c.font_size = parseFloat();
                else skipValue();
            });
            cmd = c;
        } else if (type == "DrawLine") {
            DrawLine c;
            parseCommandFields([&](const std::string& key) {
                if (key == "color") c.color = parseColor();
                else if (key == "from") c.from = parsePoint();
                else if (key == "to") c.to = parsePoint();
                else if (key == "thickness") c.thickness = parseInt();
                else skipValue();
            });
            cmd = c;
        } else if (type == "FillEllipse") {
            FillEllipse c;
            parseCommandFields([&](const std::string& key) {
                if (key == "rect") c.rect = parseRect();
                else if (key == "color") c.color = parseColor();
                else skipValue();
            });
            cmd = c;
        } else if (type == "DrawEllipse") {
            DrawEllipse c;
            parseCommandFields([&](const std::string& key) {
                if (key == "rect") c.rect = parseRect();
                else if (key == "color") c.color = parseColor();
                else if (key == "thickness") c.thickness = parseInt();
                else skipValue();
            });
            cmd = c;
        } else if (type == "PaintLinearGradient") {
            PaintLinearGradient c;
            parseCommandFields([&](const std::string& key) {
                if (key == "rect") c.rect = parseRect();
                else if (key == "start_color") c.start_color = parseColor();
                else if (key == "end_color") c.end_color = parseColor();
                else skipValue();
            });
            cmd = c;
        } else if (type == "PaintRadialGradient") {
            PaintRadialGradient c;
            parseCommandFields([&](const std::string& key) {
                if (key == "rect") c.rect = parseRect();
                else if (key == "center") c.center = parsePoint();
                else if (key == "center_color") c.center_color = parseColor();
                else if (key == "edge_color") c.edge_color = parseColor();
                else skipValue();
            });
            cmd = c;
        } else if (type == "DrawScaledBitmap") {
            DrawScaledBitmap c;
            parseCommandFields([&](const std::string& key) {
                if (key == "dst_rect") c.dst_rect = parseRect();
                else if (key == "placeholder_color") c.placeholder_color = parseColor();
                else skipValue();
            });
            cmd = c;
        } else if (type == "PaintOuterBoxShadow") {
            PaintOuterBoxShadow c;
            parseCommandFields([&](const std::string& key) {
                if (key == "rect") c.rect = parseRect();
                else if (key == "color") c.color = parseColor();
                else if (key == "blur_radius") c.blur_radius = parseInt();
                else if (key == "offset_x") c.offset_x = parseInt();
                else if (key == "offset_y") c.offset_y = parseInt();
                else if (key == "radii") c.radii = parseCornerRadii();
                else skipValue();
            });
            cmd = c;
        } else if (type == "PaintInnerBoxShadow") {
            PaintInnerBoxShadow c;
            parseCommandFields([&](const std::string& key) {
                if (key == "rect") c.rect = parseRect();
                else if (key == "color") c.color = parseColor();
                else if (key == "blur_radius") c.blur_radius = parseInt();
                else skipValue();
            });
            cmd = c;
        } else if (type == "Save") {
            Save c;
            parseCommandFields([&](const std::string&) { skipValue(); });
            cmd = c;
        } else if (type == "Restore") {
            Restore c;
            parseCommandFields([&](const std::string&) { skipValue(); });
            cmd = c;
        } else if (type == "Translate") {
            Translate c;
            parseCommandFields([&](const std::string& key) {
                if (key == "delta") c.delta = parsePoint();
                else skipValue();
            });
            cmd = c;
        } else if (type == "AddClipRect") {
            AddClipRect c;
            parseCommandFields([&](const std::string& key) {
                if (key == "rect") c.rect = parseRect();
                else skipValue();
            });
            cmd = c;
        } else if (type == "AddRoundedRectClip") {
            AddRoundedRectClip c;
            parseCommandFields([&](const std::string& key) {
                if (key == "rect") c.rect = parseRect();
                else if (key == "radii") c.radii = parseCornerRadii();
                else skipValue();
            });
            cmd = c;
        } else if (type == "FillPath") {
            FillPath c;
            parseCommandFields([&](const std::string& key) {
                if (key == "bounding_rect") c.bounding_rect = parseRect();
                else if (key == "color") c.color = parseColor();
                else if (key == "path_data") c.path_data = parseString();
                else skipValue();
            });
            cmd = c;
        } else if (type == "StrokePath") {
            StrokePath c;
            parseCommandFields([&](const std::string& key) {
                if (key == "bounding_rect") c.bounding_rect = parseRect();
                else if (key == "color") c.color = parseColor();
                else if (key == "thickness") c.thickness = parseFloat();
                else if (key == "path_data") c.path_data = parseString();
                else skipValue();
            });
            cmd = c;
        } else {
            // Unknown command, skip
            parseCommandFields([&](const std::string&) { skipValue(); });
            cmd = Save{}; // Placeholder
        }
        return cmd;
    }

    template<typename F>
    void parseCommandFields(F&& handler) {
        while (peek() != '}') {
            std::string key = parseString();
            expect(':');
            if (key != "type") {
                handler(key);
            } else {
                parseString(); // skip type value
            }
            if (peek() == ',') next();
        }
        expect('}');
    }

    void skipValue() {
        char c = peek();
        if (c == '"') {
            parseString();
        } else if (c == '{') {
            expect('{');
            while (peek() != '}') {
                parseString(); expect(':');
                skipValue();
                if (peek() == ',') next();
            }
            expect('}');
        } else if (c == '[') {
            expect('[');
            while (peek() != ']') {
                skipValue();
                if (peek() == ',') next();
            }
            expect(']');
        } else if (c == 't' || c == 'f') {
            parseBool();
        } else if (c == 'n') {
            parseNull();
        } else {
            // Number
            while (_pos < _json.size() && (std::isdigit(_json[_pos]) || _json[_pos] == '.' || _json[_pos] == '-' || _json[_pos] == 'e' || _json[_pos] == 'E' || _json[_pos] == '+')) {
                ++_pos;
            }
        }
    }

    DisplayList parseDisplayList() {
        DisplayList list;
        expect('{');
        while (peek() != '}') {
            std::string key = parseString();
            expect(':');
            if (key == "width") {
                list.width = parseInt();
            } else if (key == "height") {
                list.height = parseInt();
            } else if (key == "commands") {
                expect('[');
                while (peek() != ']') {
                    list.commands.push_back(parseCommand());
                    if (peek() == ',') next();
                }
                expect(']');
            } else {
                skipValue();
            }
            if (peek() == ',') next();
        }
        expect('}');
        return list;
    }

private:
    const std::string& _json;
    size_t _pos;
};

} // anonymous namespace

DisplayList parseJSON(const std::string& json) {
    JSONParser parser(json);
    return parser.parseDisplayList();
}

std::string toJSON(const DisplayList& list) {
    std::ostringstream out;
    out << "{\n";
    out << "  \"width\": " << list.width << ",\n";
    out << "  \"height\": " << list.height << ",\n";
    out << "  \"commands\": [\n";

    auto colorJSON = [](const Color& c) {
        std::ostringstream s;
        s << "{\"r\":" << (int)c.r << ",\"g\":" << (int)c.g << ",\"b\":" << (int)c.b << ",\"a\":" << (int)c.a << "}";
        return s.str();
    };

    auto rectJSON = [](const Rect& r) {
        std::ostringstream s;
        s << "{\"x\":" << r.x << ",\"y\":" << r.y << ",\"width\":" << r.width << ",\"height\":" << r.height << "}";
        return s.str();
    };

    auto pointJSON = [](const Point& p) {
        std::ostringstream s;
        s << "{\"x\":" << p.x << ",\"y\":" << p.y << "}";
        return s.str();
    };

    auto radiiJSON = [](const CornerRadii& r) {
        std::ostringstream s;
        s << "{\"top_left\":" << r.top_left << ",\"top_right\":" << r.top_right;
        s << ",\"bottom_left\":" << r.bottom_left << ",\"bottom_right\":" << r.bottom_right << "}";
        return s.str();
    };

    for (size_t i = 0; i < list.commands.size(); ++i) {
        out << "    ";
        std::visit([&](auto&& cmd) {
            using T = std::decay_t<decltype(cmd)>;
            out << "{\"type\":\"" << T::type << "\"";
            if constexpr (std::is_same_v<T, FillRect>) {
                out << ",\"rect\":" << rectJSON(cmd.rect);
                out << ",\"color\":" << colorJSON(cmd.color);
            } else if constexpr (std::is_same_v<T, FillRectWithRoundedCorners>) {
                out << ",\"rect\":" << rectJSON(cmd.rect);
                out << ",\"color\":" << colorJSON(cmd.color);
                out << ",\"radii\":" << radiiJSON(cmd.radii);
            } else if constexpr (std::is_same_v<T, DrawGlyphRun>) {
                out << ",\"rect\":" << rectJSON(cmd.rect);
                out << ",\"translation\":{\"x\":" << cmd.translation.x << ",\"y\":" << cmd.translation.y << "}";
                out << ",\"color\":" << colorJSON(cmd.color);
                out << ",\"text\":\"" << cmd.text << "\"";
                out << ",\"font_size\":" << cmd.font_size;
            } else if constexpr (std::is_same_v<T, DrawLine>) {
                out << ",\"from\":" << pointJSON(cmd.from);
                out << ",\"to\":" << pointJSON(cmd.to);
                out << ",\"color\":" << colorJSON(cmd.color);
                out << ",\"thickness\":" << cmd.thickness;
            } else if constexpr (std::is_same_v<T, FillEllipse>) {
                out << ",\"rect\":" << rectJSON(cmd.rect);
                out << ",\"color\":" << colorJSON(cmd.color);
            } else if constexpr (std::is_same_v<T, DrawEllipse>) {
                out << ",\"rect\":" << rectJSON(cmd.rect);
                out << ",\"color\":" << colorJSON(cmd.color);
                out << ",\"thickness\":" << cmd.thickness;
            } else if constexpr (std::is_same_v<T, Translate>) {
                out << ",\"delta\":" << pointJSON(cmd.delta);
            } else if constexpr (std::is_same_v<T, AddClipRect>) {
                out << ",\"rect\":" << rectJSON(cmd.rect);
            }
            // Add more as needed
            out << "}";
        }, list.commands[i]);
        if (i + 1 < list.commands.size()) out << ",";
        out << "\n";
    }
    out << "  ]\n";
    out << "}\n";
    return out.str();
}

} // namespace displaylist
