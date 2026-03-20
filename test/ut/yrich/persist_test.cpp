//=============================================================================
// YRich Persistence Tests - data serialization roundtrip tests
//=============================================================================

#include <cstddef>
#include <version>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <boost/ut.hpp>

#include "yetty/yrich/yrich-persist.h"
// yrich-persist.h already includes ydoc.h, yspreadsheet.h, yslides.h

using namespace boost::ut;
using namespace yetty::yrich;

//=============================================================================
// Format detection tests
//=============================================================================

suite persist_format_detection = [] {
    "detect_ydoc_yaml"_test = [] {
        expect(DocumentPersist::detectType("test.ydoc.yaml") == DocumentPersist::DocType::YDoc);
        expect(DocumentPersist::detectFormat("test.ydoc.yaml") == DocumentPersist::Format::Yaml);
    };

    "detect_ydoc_binary"_test = [] {
        expect(DocumentPersist::detectType("test.ydoc") == DocumentPersist::DocType::YDoc);
        expect(DocumentPersist::detectFormat("test.ydoc") == DocumentPersist::Format::Binary);
    };

    "detect_spreadsheet_yaml"_test = [] {
        expect(DocumentPersist::detectType("data.ysheet.yaml") == DocumentPersist::DocType::YSpreadsheet);
        expect(DocumentPersist::detectFormat("data.ysheet.yaml") == DocumentPersist::Format::Yaml);
    };

    "detect_spreadsheet_binary"_test = [] {
        expect(DocumentPersist::detectType("data.ysheet") == DocumentPersist::DocType::YSpreadsheet);
        expect(DocumentPersist::detectFormat("data.ysheet") == DocumentPersist::Format::Binary);
    };

    "detect_slides_yaml"_test = [] {
        expect(DocumentPersist::detectType("pres.yslides.yaml") == DocumentPersist::DocType::YSlides);
        expect(DocumentPersist::detectFormat("pres.yslides.yaml") == DocumentPersist::Format::Yaml);
    };

    "detect_slides_binary"_test = [] {
        expect(DocumentPersist::detectType("pres.yslides") == DocumentPersist::DocType::YSlides);
        expect(DocumentPersist::detectFormat("pres.yslides") == DocumentPersist::Format::Binary);
    };

    "detect_unknown_type"_test = [] {
        expect(DocumentPersist::detectType("random.txt") == DocumentPersist::DocType::Unknown);
        expect(DocumentPersist::detectFormat("random.txt") == DocumentPersist::Format::Binary);
    };
};

//=============================================================================
// Data structure defaults tests
//=============================================================================

suite persist_data_structures = [] {
    "text_run_data_defaults"_test = [] {
        TextRunData rd;
        expect(rd.start == 0_i);
        expect(rd.end == 0_i);
        expect(rd.fontSize == 14.0f);
        expect(rd.format == 0_u);
    };

    "paragraph_data_defaults"_test = [] {
        ParagraphData pd;
        expect(pd.text.empty());
        expect(pd.runs.empty());
        expect(pd.fontSize == 14.0f);
    };

    "ydoc_data_defaults"_test = [] {
        YDocData data;
        expect(data.version == 1_i);
        expect(data.type == std::string("ydoc"));
        expect(data.pageWidth == 600.0f);
        expect(data.margin == 20.0f);
        expect(data.paragraphs.empty());
    };

    "yspreadsheet_data_defaults"_test = [] {
        YSpreadsheetData data;
        expect(data.version == 1_i);
        expect(data.type == std::string("yspreadsheet"));
        expect(data.rows == 100_i);
        expect(data.cols == 26_i);
        expect(data.cells.empty());
    };

    "yslides_data_defaults"_test = [] {
        YSlidesData data;
        expect(data.version == 1_i);
        expect(data.type == std::string("yslides"));
        expect(data.slideWidth == 960.0f);
        expect(data.slideHeight == 540.0f);
        expect(data.slides.empty());
    };
};

//=============================================================================
// YDoc YAML roundtrip tests
//=============================================================================

suite ydoc_yaml_roundtrip = [] {
    "empty_ydoc_yaml_roundtrip"_test = [] {
        YDocData original;
        original.pageWidth = 800.0f;
        original.margin = 30.0f;

        std::string yaml = DocumentPersist::dataToYaml(original);
        expect(!yaml.empty()) << "YAML should not be empty";

        auto result = DocumentPersist::parseYDocYaml(yaml);
        expect(result.has_value()) << "Parsing should succeed";
        if (!result.has_value()) return;

        auto& parsed = *result;
        expect(std::abs(parsed.pageWidth - original.pageWidth) < 0.01f);
        expect(std::abs(parsed.margin - original.margin) < 0.01f);
    };

    "ydoc_with_paragraphs_yaml_roundtrip"_test = [] {
        YDocData original;
        original.pageWidth = 700.0f;
        original.margin = 25.0f;

        ParagraphData p1;
        p1.text = "Hello World";
        p1.fontSize = 24.0f;
        p1.color = 0xFF2563EB;

        ParagraphData p2;
        p2.text = "Second paragraph with special chars";
        p2.fontSize = 14.0f;
        p2.color = 0xFF000000;

        original.paragraphs.push_back(p1);
        original.paragraphs.push_back(p2);

        std::string yaml = DocumentPersist::dataToYaml(original);
        expect(!yaml.empty());
        expect(yaml.find("Hello World") != std::string::npos) << "Should contain paragraph text";

        auto result = DocumentPersist::parseYDocYaml(yaml);
        expect(result.has_value()) << "Parsing should succeed";
        if (!result.has_value()) return;

        auto& parsed = *result;
        expect(parsed.paragraphs.size() == 2u) << "Should have 2 paragraphs";
        if (parsed.paragraphs.size() < 1) return;
        expect(parsed.paragraphs[0].text == std::string("Hello World"));
        expect(std::abs(parsed.paragraphs[0].fontSize - 24.0f) < 0.01f);
        expect(parsed.paragraphs[0].color == 0xFF2563EBu);
    };

    "ydoc_with_runs_yaml_roundtrip"_test = [] {
        YDocData original;

        ParagraphData p;
        p.text = "Bold and italic text";
        p.fontSize = 14.0f;
        p.color = 0xFF000000;

        TextRunData run1;
        run1.start = 0;
        run1.end = 4;
        run1.format = 1;
        run1.fontSize = 14.0f;
        run1.color = 0xFFFF0000;

        TextRunData run2;
        run2.start = 9;
        run2.end = 15;
        run2.format = 2;
        run2.fontSize = 16.0f;
        run2.color = 0xFF0000FF;

        p.runs.push_back(run1);
        p.runs.push_back(run2);
        original.paragraphs.push_back(p);

        std::string yaml = DocumentPersist::dataToYaml(original);
        auto result = DocumentPersist::parseYDocYaml(yaml);
        expect(result.has_value());
        if (!result.has_value()) return;

        auto& parsed = *result;
        expect(parsed.paragraphs.size() == 1u);
        if (parsed.paragraphs.empty()) return;
        expect(parsed.paragraphs[0].runs.size() == 2u) << "Should have 2 runs";
        if (parsed.paragraphs[0].runs.size() < 2) return;
        expect(parsed.paragraphs[0].runs[0].start == 0_i);
        expect(parsed.paragraphs[0].runs[0].end == 4_i);
        expect(parsed.paragraphs[0].runs[0].format == 1_u);
    };
};

//=============================================================================
// YDoc binary roundtrip tests
//=============================================================================

suite ydoc_binary_roundtrip = [] {
    "empty_ydoc_binary_roundtrip"_test = [] {
        YDocData original;
        original.pageWidth = 850.0f;
        original.margin = 35.0f;

        auto binary = DocumentPersist::dataToBinary(original);
        expect(!binary.empty()) << "Binary should not be empty";

        auto result = DocumentPersist::parseYDocBinary(binary.data(), binary.size());
        expect(result.has_value()) << "Parsing should succeed";
        if (!result.has_value()) return;

        auto& parsed = *result;
        expect(std::abs(parsed.pageWidth - original.pageWidth) < 0.01f);
        expect(std::abs(parsed.margin - original.margin) < 0.01f);
    };

    "ydoc_with_paragraphs_binary_roundtrip"_test = [] {
        YDocData original;
        original.pageWidth = 600.0f;

        ParagraphData p1;
        p1.text = "Binary test paragraph";
        p1.fontSize = 18.0f;
        p1.color = 0xFF123456;

        ParagraphData p2;
        p2.text = "Another paragraph";
        p2.fontSize = 12.0f;

        original.paragraphs.push_back(p1);
        original.paragraphs.push_back(p2);

        auto binary = DocumentPersist::dataToBinary(original);
        auto result = DocumentPersist::parseYDocBinary(binary.data(), binary.size());
        expect(result.has_value());
        if (!result.has_value()) return;

        auto& parsed = *result;
        expect(parsed.paragraphs.size() == 2u);
        if (parsed.paragraphs.empty()) return;
        expect(parsed.paragraphs[0].text == std::string("Binary test paragraph"));
        expect(std::abs(parsed.paragraphs[0].fontSize - 18.0f) < 0.01f);
        expect(parsed.paragraphs[0].color == 0xFF123456u);
    };

    "ydoc_with_runs_binary_roundtrip"_test = [] {
        YDocData original;

        ParagraphData p;
        p.text = "Formatted text";
        p.color = 0xFF000000;

        TextRunData run;
        run.start = 0;
        run.end = 9;
        run.format = 3;
        run.fontSize = 20.0f;
        run.color = 0xFFABCDEF;

        p.runs.push_back(run);
        original.paragraphs.push_back(p);

        auto binary = DocumentPersist::dataToBinary(original);
        auto result = DocumentPersist::parseYDocBinary(binary.data(), binary.size());
        expect(result.has_value());
        if (!result.has_value()) return;

        auto& parsed = *result;
        if (parsed.paragraphs.empty() || parsed.paragraphs[0].runs.empty()) return;
        expect(parsed.paragraphs[0].runs.size() == 1u);
        expect(parsed.paragraphs[0].runs[0].format == 3_u);
        expect(parsed.paragraphs[0].runs[0].color == 0xFFABCDEFu);
    };
};

//=============================================================================
// YSpreadsheet YAML roundtrip tests
//=============================================================================

suite yspreadsheet_yaml_roundtrip = [] {
    "empty_spreadsheet_yaml_roundtrip"_test = [] {
        YSpreadsheetData original;
        original.rows = 50;
        original.cols = 10;

        std::string yaml = DocumentPersist::dataToYaml(original);
        expect(!yaml.empty());

        auto result = DocumentPersist::parseYSpreadsheetYaml(yaml);
        expect(result.has_value());
        if (!result.has_value()) return;

        auto& parsed = *result;
        expect(parsed.rows == 50_i);
        expect(parsed.cols == 10_i);
    };

    "spreadsheet_with_cells_yaml_roundtrip"_test = [] {
        YSpreadsheetData original;
        original.rows = 100;
        original.cols = 26;

        SpreadsheetCellData c1;
        c1.row = 0;
        c1.col = 0;
        c1.value = "Header";

        SpreadsheetCellData c2;
        c2.row = 0;
        c2.col = 1;
        c2.value = "Value";

        SpreadsheetCellData c3;
        c3.row = 1;
        c3.col = 0;
        c3.value = "123.45";

        SpreadsheetCellData c4;
        c4.row = 9;
        c4.col = 2;
        c4.value = "Deep cell";

        original.cells.push_back(c1);
        original.cells.push_back(c2);
        original.cells.push_back(c3);
        original.cells.push_back(c4);

        std::string yaml = DocumentPersist::dataToYaml(original);
        expect(yaml.find("A1:") != std::string::npos) << "Should contain A1";
        expect(yaml.find("Header") != std::string::npos);

        auto result = DocumentPersist::parseYSpreadsheetYaml(yaml);
        expect(result.has_value());
        if (!result.has_value()) return;

        auto& parsed = *result;
        expect(parsed.cells.size() == 4u) << "Should have 4 cells";

        bool foundA1 = false;
        for (const auto& cell : parsed.cells) {
            if (cell.row == 0 && cell.col == 0) {
                expect(cell.value == std::string("Header"));
                foundA1 = true;
            }
        }
        expect(foundA1) << "Should find A1 cell";
    };

    "spreadsheet_special_values_yaml_roundtrip"_test = [] {
        YSpreadsheetData original;

        SpreadsheetCellData c1;
        c1.row = 0;
        c1.col = 0;
        c1.value = "Text with colon";

        SpreadsheetCellData c2;
        c2.row = 1;
        c2.col = 0;
        c2.value = "=SUM(A1:A5)";

        original.cells.push_back(c1);
        original.cells.push_back(c2);

        std::string yaml = DocumentPersist::dataToYaml(original);
        auto result = DocumentPersist::parseYSpreadsheetYaml(yaml);
        expect(result.has_value());
        if (!result.has_value()) return;

        auto& parsed = *result;
        expect(parsed.cells.size() == 2u);
    };
};

//=============================================================================
// YSpreadsheet binary roundtrip tests
//=============================================================================

suite yspreadsheet_binary_roundtrip = [] {
    "empty_spreadsheet_binary_roundtrip"_test = [] {
        YSpreadsheetData original;
        original.rows = 200;
        original.cols = 50;

        auto binary = DocumentPersist::dataToBinary(original);
        expect(!binary.empty());

        auto result = DocumentPersist::parseYSpreadsheetBinary(binary.data(), binary.size());
        expect(result.has_value());
        if (!result.has_value()) return;

        auto& parsed = *result;
        expect(parsed.rows == 200_i);
        expect(parsed.cols == 50_i);
    };

    "spreadsheet_with_cells_binary_roundtrip"_test = [] {
        YSpreadsheetData original;

        SpreadsheetCellData c1;
        c1.row = 5;
        c1.col = 3;
        c1.value = "Binary cell";

        SpreadsheetCellData c2;
        c2.row = 99;
        c2.col = 25;
        c2.value = "Last cell";

        original.cells.push_back(c1);
        original.cells.push_back(c2);

        auto binary = DocumentPersist::dataToBinary(original);
        auto result = DocumentPersist::parseYSpreadsheetBinary(binary.data(), binary.size());
        expect(result.has_value());
        if (!result.has_value()) return;

        auto& parsed = *result;
        expect(parsed.cells.size() == 2u);
        if (parsed.cells.empty()) return;
        expect(parsed.cells[0].row == 5_i);
        expect(parsed.cells[0].col == 3_i);
        expect(parsed.cells[0].value == std::string("Binary cell"));
    };
};

//=============================================================================
// YSlides YAML roundtrip tests
//=============================================================================

suite yslides_yaml_roundtrip = [] {
    "empty_slides_yaml_roundtrip"_test = [] {
        YSlidesData original;
        original.slideWidth = 1280.0f;
        original.slideHeight = 720.0f;

        std::string yaml = DocumentPersist::dataToYaml(original);
        expect(!yaml.empty());

        auto result = DocumentPersist::parseYSlidesYaml(yaml);
        expect(result.has_value());
        if (!result.has_value()) return;

        auto& parsed = *result;
        expect(std::abs(parsed.slideWidth - 1280.0f) < 0.01f);
        expect(std::abs(parsed.slideHeight - 720.0f) < 0.01f);
    };

    "slides_with_shapes_yaml_roundtrip"_test = [] {
        YSlidesData original;
        original.slideWidth = 960.0f;
        original.slideHeight = 540.0f;

        SlideData slide;
        slide.index = 0;
        slide.bgColor = 0xFFFFFFFF;

        ShapeData shape1;
        shape1.shapeType = 2;
        shape1.x = 100.0f;
        shape1.y = 50.0f;
        shape1.width = 300.0f;
        shape1.height = 80.0f;
        shape1.text = "Title Slide";
        shape1.fontSize = 48.0f;
        shape1.fillColor = 0x00000000;
        shape1.strokeColor = 0xFF000000;

        ShapeData shape2;
        shape2.shapeType = 0;
        shape2.x = 200.0f;
        shape2.y = 200.0f;
        shape2.width = 150.0f;
        shape2.height = 100.0f;
        shape2.fillColor = 0xFF3B82F6;
        shape2.strokeColor = 0xFF1E40AF;

        slide.shapes.push_back(shape1);
        slide.shapes.push_back(shape2);
        original.slides.push_back(slide);

        std::string yaml = DocumentPersist::dataToYaml(original);
        expect(yaml.find("Title Slide") != std::string::npos);

        auto result = DocumentPersist::parseYSlidesYaml(yaml);
        expect(result.has_value());
        if (!result.has_value()) return;

        auto& parsed = *result;
        expect(parsed.slides.size() == 1u);
        if (parsed.slides.empty()) return;
        expect(parsed.slides[0].shapes.size() == 2u);
        if (parsed.slides[0].shapes.empty()) return;
        expect(parsed.slides[0].shapes[0].text == std::string("Title Slide"));
        expect(std::abs(parsed.slides[0].shapes[0].fontSize - 48.0f) < 0.01f);
    };

    "slides_multiple_slides_yaml_roundtrip"_test = [] {
        YSlidesData original;

        SlideData slide1;
        slide1.index = 0;
        slide1.bgColor = 0xFFFFFFFF;

        SlideData slide2;
        slide2.index = 1;
        slide2.bgColor = 0xFF1E40AF;

        SlideData slide3;
        slide3.index = 2;
        slide3.bgColor = 0xFF10B981;

        original.slides.push_back(slide1);
        original.slides.push_back(slide2);
        original.slides.push_back(slide3);

        std::string yaml = DocumentPersist::dataToYaml(original);
        auto result = DocumentPersist::parseYSlidesYaml(yaml);
        expect(result.has_value());
        if (!result.has_value()) return;

        auto& parsed = *result;
        expect(parsed.slides.size() == 3u);
        if (parsed.slides.size() < 3) return;
        expect(parsed.slides[0].bgColor == 0xFFFFFFFFu);
        expect(parsed.slides[1].bgColor == 0xFF1E40AFu);
        expect(parsed.slides[2].bgColor == 0xFF10B981u);
    };
};

//=============================================================================
// YSlides binary roundtrip tests
//=============================================================================

suite yslides_binary_roundtrip = [] {
    "empty_slides_binary_roundtrip"_test = [] {
        YSlidesData original;
        original.slideWidth = 1920.0f;
        original.slideHeight = 1080.0f;

        auto binary = DocumentPersist::dataToBinary(original);
        expect(!binary.empty());

        auto result = DocumentPersist::parseYSlidesBinary(binary.data(), binary.size());
        expect(result.has_value());
        if (!result.has_value()) return;

        auto& parsed = *result;
        expect(std::abs(parsed.slideWidth - 1920.0f) < 0.01f);
        expect(std::abs(parsed.slideHeight - 1080.0f) < 0.01f);
    };

    "slides_with_shapes_binary_roundtrip"_test = [] {
        YSlidesData original;

        SlideData slide;
        slide.index = 0;
        slide.bgColor = 0xFFEEEEEE;

        ShapeData shape;
        shape.shapeType = 1;
        shape.x = 50.0f;
        shape.y = 50.0f;
        shape.width = 200.0f;
        shape.height = 200.0f;
        shape.fillColor = 0xFFFF5722;
        shape.strokeColor = 0xFFE64A19;
        shape.strokeWidth = 3.0f;

        slide.shapes.push_back(shape);
        original.slides.push_back(slide);

        auto binary = DocumentPersist::dataToBinary(original);
        auto result = DocumentPersist::parseYSlidesBinary(binary.data(), binary.size());
        expect(result.has_value());
        if (!result.has_value()) return;

        auto& parsed = *result;
        expect(parsed.slides.size() == 1u);
        if (parsed.slides.empty()) return;
        expect(parsed.slides[0].shapes.size() == 1u);
        if (parsed.slides[0].shapes.empty()) return;
        expect(parsed.slides[0].shapes[0].shapeType == 1_i);
        expect(parsed.slides[0].shapes[0].fillColor == 0xFFFF5722u);
        expect(std::abs(parsed.slides[0].shapes[0].strokeWidth - 3.0f) < 0.01f);
    };
};

//=============================================================================
// Error handling tests
//=============================================================================

//=============================================================================
// Load demo files tests
//=============================================================================

suite load_demo_files = [] {
    "load_ydoc_sample"_test = [] {
        std::ifstream file("demo/assets/yrich/ydoc/sample.ydoc.yaml");
        expect(file.good()) << "File should exist";
        if (!file.good()) return;

        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string yaml = buffer.str();

        auto result = DocumentPersist::parseYDocYaml(yaml);
        expect(result.has_value()) << "Should parse successfully";
        if (!result.has_value()) {
            std::cerr << "ERROR: " << result.error().to_string() << std::endl;
            return;
        }

        auto& data = *result;
        expect(data.type == std::string("ydoc"));
        expect(data.paragraphs.size() > 0u) << "Should have paragraphs";
    };

    "load_yspreadsheet_sample"_test = [] {
        std::ifstream file("demo/assets/yrich/yspreadsheet/sample.ysheet.yaml");
        expect(file.good()) << "File should exist";
        if (!file.good()) return;

        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string yaml = buffer.str();

        auto result = DocumentPersist::parseYSpreadsheetYaml(yaml);
        expect(result.has_value()) << "Should parse successfully";
        if (!result.has_value()) {
            std::cerr << "ERROR: " << result.error().to_string() << std::endl;
            return;
        }

        auto& data = *result;
        expect(data.type == std::string("yspreadsheet"));
        expect(data.cells.size() > 0u) << "Should have cells";
    };

    "load_yslides_sample"_test = [] {
        std::ifstream file("demo/assets/yrich/yslides/sample.yslides.yaml");
        expect(file.good()) << "File should exist";
        if (!file.good()) return;

        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string yaml = buffer.str();

        auto result = DocumentPersist::parseYSlidesYaml(yaml);
        expect(result.has_value()) << "Should parse successfully";
        if (!result.has_value()) {
            std::cerr << "ERROR: " << result.error().to_string() << std::endl;
            return;
        }

        auto& data = *result;
        expect(data.type == std::string("yslides"));
        expect(data.slides.size() > 0u) << "Should have slides";
    };

    "create_yslides"_test = [] {
        auto result = YSlides::create();
        expect(result.has_value()) << "Should create YSlides";
        if (!result.has_value()) return;

        auto slides = *result;
        expect(slides != nullptr) << "Should return valid ptr";
        expect(slides->slideCount() == 1_i) << "Should have 1 default slide";
    };

    "fromYamlSlides_sample"_test = [] {
        std::ifstream file("demo/assets/yrich/yslides/sample.yslides.yaml");
        expect(file.good()) << "File should exist";
        if (!file.good()) return;

        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string yaml = buffer.str();

        auto result = DocumentPersist::fromYamlSlides(yaml);
        expect(result.has_value()) << "Should load slides";
        if (!result.has_value()) {
            std::cerr << "ERROR: " << result.error().to_string() << std::endl;
            return;
        }

        auto slides = *result;
        expect(slides != nullptr) << "Should return valid ptr";
        expect(slides->slideCount() == 5_i) << "Should have 5 slides";
    };

    "loadYSlides_sample"_test = [] {
        auto result = DocumentPersist::loadYSlides("demo/assets/yrich/yslides/sample.yslides.yaml");
        expect(result.has_value()) << "Should load from file";
        if (!result.has_value()) {
            std::cerr << "ERROR: " << result.error().to_string() << std::endl;
            return;
        }

        auto slides = *result;
        expect(slides != nullptr) << "Should return valid ptr";
        expect(slides->slideCount() == 5_i) << "Should have 5 slides";
    };
};

//=============================================================================
// Error handling tests
//=============================================================================

suite persist_error_handling = [] {
    "parse_invalid_ydoc_yaml"_test = [] {
        auto result = DocumentPersist::parseYDocYaml("type: yspreadsheet\n");
        expect(!result.has_value()) << "Should fail for wrong type";
    };

    "parse_invalid_spreadsheet_yaml"_test = [] {
        auto result = DocumentPersist::parseYSpreadsheetYaml("type: ydoc\n");
        expect(!result.has_value()) << "Should fail for wrong type";
    };

    "parse_invalid_slides_yaml"_test = [] {
        auto result = DocumentPersist::parseYSlidesYaml("type: yspreadsheet\n");
        expect(!result.has_value()) << "Should fail for wrong type";
    };

    "parse_malformed_yaml"_test = [] {
        auto result = DocumentPersist::parseYDocYaml("{{{{not valid yaml");
        expect(!result.has_value()) << "Should fail for malformed YAML";
    };

    "parse_empty_binary"_test = [] {
        auto result = DocumentPersist::parseYDocBinary(nullptr, 0);
        expect(!result.has_value()) << "Should fail for empty binary";
    };
};
