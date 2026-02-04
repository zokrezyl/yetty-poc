//=============================================================================
// YAST Parser Tests
//
// Tests for the Yetty Abstract Syntax Tree parser and WGSL generator
//=============================================================================

// Include C++ standard headers before boost/ut.hpp (cdb version file conflict)
#include <cstddef>
#include <version>
#include <algorithm>

#include <boost/ut.hpp>
#include "yetty/yast/yast.h"

using namespace boost::ut;
using namespace yetty::yast;

suite yast_lexer_tests = [] {
    "lexer tokenizes number"_test = [] {
        Lexer lexer("42");
        auto tok = lexer.next();
        expect(tok.type == TokenType::Number);
    };

    "lexer tokenizes float"_test = [] {
        Lexer lexer("3.14");
        auto tok = lexer.next();
        expect(tok.type == TokenType::Number);
    };

    "lexer tokenizes identifier"_test = [] {
        Lexer lexer("sin");
        auto tok = lexer.next();
        expect(tok.type == TokenType::Identifier);
    };

    "lexer tokenizes buffer reference"_test = [] {
        Lexer lexer("@buffer1");
        auto tok = lexer.next();
        expect(tok.type == TokenType::At);
        tok = lexer.next();
        expect(tok.type == TokenType::Identifier);
    };

    "lexer tokenizes operators"_test = [] {
        Lexer lexer("+ - * / ^");
        expect(lexer.next().type == TokenType::Plus);
        expect(lexer.next().type == TokenType::Minus);
        expect(lexer.next().type == TokenType::Star);
        expect(lexer.next().type == TokenType::Slash);
        expect(lexer.next().type == TokenType::Caret);
    };

    "lexer tokenizes parentheses"_test = [] {
        Lexer lexer("()");
        expect(lexer.next().type == TokenType::LParen);
        expect(lexer.next().type == TokenType::RParen);
    };

    "lexer tokenizes braces"_test = [] {
        Lexer lexer("{}");
        expect(lexer.next().type == TokenType::LBrace);
        expect(lexer.next().type == TokenType::RBrace);
    };

    "lexer tokenizes comma and colon"_test = [] {
        Lexer lexer(",:");
        expect(lexer.next().type == TokenType::Comma);
        expect(lexer.next().type == TokenType::Colon);
    };

    "lexer tokenizes range"_test = [] {
        Lexer lexer("..");
        expect(lexer.next().type == TokenType::DotDot);
    };

    "lexer tokenizes complex expression"_test = [] {
        Lexer lexer("sin(x) + 2.0 * y");
        expect(lexer.next().type == TokenType::Identifier);  // sin
        expect(lexer.next().type == TokenType::LParen);
        expect(lexer.next().type == TokenType::Identifier);  // x
        expect(lexer.next().type == TokenType::RParen);
        expect(lexer.next().type == TokenType::Plus);
        expect(lexer.next().type == TokenType::Number);      // 2.0
        expect(lexer.next().type == TokenType::Star);
        expect(lexer.next().type == TokenType::Identifier);  // y
        expect(lexer.next().type == TokenType::Eof);
    };

    "lexer handles whitespace"_test = [] {
        Lexer lexer("  x   +   y  ");
        expect(lexer.next().type == TokenType::Identifier);
        expect(lexer.next().type == TokenType::Plus);
        expect(lexer.next().type == TokenType::Identifier);
        expect(lexer.next().type == TokenType::Eof);
    };
};

suite yast_parser_tests = [] {
    "parser parses number"_test = [] {
        Parser parser("42");
        auto node = parser.parseExpression();
        expect(node != nullptr);
        auto* num = dynamic_cast<NumberNode*>(node.get());
        expect(num != nullptr);
        expect(num->value == 42.0_d);
    };

    "parser parses float"_test = [] {
        Parser parser("3.14");
        auto node = parser.parseExpression();
        auto* num = dynamic_cast<NumberNode*>(node.get());
        expect(num != nullptr);
        expect(num->value > 3.13_d);
        expect(num->value < 3.15_d);
    };

    "parser parses identifier"_test = [] {
        Parser parser("x");
        auto node = parser.parseExpression();
        expect(node != nullptr);
        auto* id = dynamic_cast<IdentifierNode*>(node.get());
        expect(id != nullptr);
        expect(id->name == std::string("x"));
    };

    "parser parses buffer reference"_test = [] {
        Parser parser("@buffer1");
        auto node = parser.parseExpression();
        expect(node != nullptr);
        auto* buf = dynamic_cast<BufferRefNode*>(node.get());
        expect(buf != nullptr);
        expect(buf->index == 1_i);
    };

    "parser parses function call"_test = [] {
        Parser parser("sin(x)");
        auto node = parser.parseExpression();
        expect(node != nullptr);
        auto* call = dynamic_cast<CallNode*>(node.get());
        expect(call != nullptr);
        expect(call->name == std::string("sin"));
        expect(call->args.size() == 1_u);
    };

    "parser parses function with multiple args"_test = [] {
        Parser parser("pow(x, 2)");
        auto node = parser.parseExpression();
        expect(node != nullptr);
        auto* call = dynamic_cast<CallNode*>(node.get());
        expect(call != nullptr);
        expect(call->name == std::string("pow"));
        expect(call->args.size() == 2_u);
    };

    "parser parses binary addition"_test = [] {
        Parser parser("x + y");
        auto node = parser.parseExpression();
        expect(node != nullptr);
        auto* bin = dynamic_cast<BinaryOpNode*>(node.get());
        expect(bin != nullptr);
        expect(bin->op == BinaryOpNode::Op::Add);
    };

    "parser parses binary subtraction"_test = [] {
        Parser parser("x - y");
        auto node = parser.parseExpression();
        auto* bin = dynamic_cast<BinaryOpNode*>(node.get());
        expect(bin != nullptr);
        expect(bin->op == BinaryOpNode::Op::Sub);
    };

    "parser parses multiplication"_test = [] {
        Parser parser("x * y");
        auto node = parser.parseExpression();
        auto* bin = dynamic_cast<BinaryOpNode*>(node.get());
        expect(bin != nullptr);
        expect(bin->op == BinaryOpNode::Op::Mul);
    };

    "parser parses division"_test = [] {
        Parser parser("x / y");
        auto node = parser.parseExpression();
        auto* bin = dynamic_cast<BinaryOpNode*>(node.get());
        expect(bin != nullptr);
        expect(bin->op == BinaryOpNode::Op::Div);
    };

    "parser parses exponentiation"_test = [] {
        Parser parser("x ^ 2");
        auto node = parser.parseExpression();
        auto* bin = dynamic_cast<BinaryOpNode*>(node.get());
        expect(bin != nullptr);
        expect(bin->op == BinaryOpNode::Op::Pow);
    };

    "parser respects operator precedence"_test = [] {
        // x + y * z should parse as x + (y * z)
        Parser parser("x + y * z");
        auto node = parser.parseExpression();
        auto* bin = dynamic_cast<BinaryOpNode*>(node.get());
        expect(bin != nullptr);
        expect(bin->op == BinaryOpNode::Op::Add);  // top level is +
        auto* rhs = dynamic_cast<BinaryOpNode*>(bin->right.get());
        expect(rhs != nullptr);
        expect(rhs->op == BinaryOpNode::Op::Mul);  // right side is *
    };

    "parser handles parentheses"_test = [] {
        // (x + y) * z should parse with + at lower level
        Parser parser("(x + y) * z");
        auto node = parser.parseExpression();
        auto* bin = dynamic_cast<BinaryOpNode*>(node.get());
        expect(bin != nullptr);
        expect(bin->op == BinaryOpNode::Op::Mul);  // top level is *
        auto* lhs = dynamic_cast<BinaryOpNode*>(bin->left.get());
        expect(lhs != nullptr);
        expect(lhs->op == BinaryOpNode::Op::Add);  // left side is +
    };

    "parser parses unary minus"_test = [] {
        Parser parser("-x");
        auto node = parser.parseExpression();
        auto* unary = dynamic_cast<UnaryOpNode*>(node.get());
        expect(unary != nullptr);
        expect(unary->op == UnaryOpNode::Op::Neg);
    };

    "parser parses nested function calls"_test = [] {
        Parser parser("sin(cos(x))");
        auto node = parser.parseExpression();
        auto* outer = dynamic_cast<CallNode*>(node.get());
        expect(outer != nullptr);
        expect(outer->name == std::string("sin"));
        auto* inner = dynamic_cast<CallNode*>(outer->args[0].get());
        expect(inner != nullptr);
        expect(inner->name == std::string("cos"));
    };

    "parser parses complex expression"_test = [] {
        Parser parser("sin(x) + cos(2 * x) / 3");
        auto node = parser.parseExpression();
        expect(node != nullptr);
        auto* bin = dynamic_cast<BinaryOpNode*>(node.get());
        expect(bin != nullptr);
        expect(bin->op == BinaryOpNode::Op::Add);
    };

    "parser parses range in block attribute"_test = [] {
        // Range parsing is used in attributes, test via block
        Parser parser("{plot x=0..10: sin(x)}");
        auto node = parser.parseBlock();
        auto* block = dynamic_cast<BlockNode*>(node.get());
        expect(block != nullptr);
        auto* attr = block->findAttr("x");
        expect(attr != nullptr);
        auto* range = dynamic_cast<RangeNode*>(attr->value.get());
        expect(range != nullptr);
    };
};

suite yast_wgsl_generator_tests = [] {
    "generator converts number"_test = [] {
        auto result = expressionToWGSL("42");
        expect(result.code == std::string("42.0"));
    };

    "generator converts identifier x"_test = [] {
        auto result = expressionToWGSL("x");
        expect(result.code == std::string("x"));
        expect(result.usesX);
    };

    "generator converts identifier y"_test = [] {
        auto result = expressionToWGSL("y");
        expect(result.code == std::string("y"));
        expect(result.usesY);
    };

    "generator converts identifier time"_test = [] {
        auto result = expressionToWGSL("time");
        expect(result.code == std::string("time"));
        expect(result.usesTime);
    };

    "generator converts pi constant"_test = [] {
        auto result = expressionToWGSL("pi");
        expect(result.code == std::string("3.14159265358979323846"));
    };

    "generator converts e constant"_test = [] {
        auto result = expressionToWGSL("e");
        expect(result.code == std::string("2.71828182845904523536"));
    };

    "generator converts sin function"_test = [] {
        auto result = expressionToWGSL("sin(x)");
        expect(result.code == std::string("sin(x)"));
        expect(result.usesX);
    };

    "generator converts pow function"_test = [] {
        auto result = expressionToWGSL("pow(x, 2)");
        expect(result.code == std::string("pow(x, 2.0)"));
    };

    "generator converts caret to pow"_test = [] {
        auto result = expressionToWGSL("x^2");
        expect(result.code == std::string("pow(x, 2.0)"));
    };

    "generator converts addition"_test = [] {
        auto result = expressionToWGSL("x + y");
        expect(result.code == std::string("(x + y)"));
        expect(result.usesX);
        expect(result.usesY);
    };

    "generator converts buffer reference"_test = [] {
        auto result = expressionToWGSL("@buffer1");
        expect(result.code == std::string("sampleBuffer(0, x)"));
        expect(result.bufferRefs.size() == 1_u);
        expect(result.bufferRefs[0] == 1_i);  // Stored as 1-based index
    };

    "generator converts buffer2"_test = [] {
        auto result = expressionToWGSL("@buffer2");
        expect(result.code == std::string("sampleBuffer(1, x)"));
        expect(result.bufferRefs[0] == 2_i);  // Stored as 1-based index
    };

    "generator converts unary minus"_test = [] {
        auto result = expressionToWGSL("-x");
        expect(result.code == std::string("(-x)"));
    };

    "generator converts complex expression"_test = [] {
        auto result = expressionToWGSL("sin(x) + cos(2*x)");
        expect(result.usesX);
        expect(!result.usesY);
        expect(!result.usesTime);
    };

    "generator tracks multiple variables"_test = [] {
        auto result = expressionToWGSL("x * y + time");
        expect(result.usesX);
        expect(result.usesY);
        expect(result.usesTime);
    };

    "generator converts nested expression"_test = [] {
        auto result = expressionToWGSL("sin(x) / x");
        expect(result.code == std::string("(sin(x) / x)"));
    };

    "generator handles parentheses correctly"_test = [] {
        auto result = expressionToWGSL("(x + 1) * 2");
        expect(result.code == std::string("((x + 1.0) * 2.0)"));
    };

    "generator converts sqrt function"_test = [] {
        auto result = expressionToWGSL("sqrt(x)");
        expect(result.code == std::string("sqrt(x)"));
    };

    "generator converts abs function"_test = [] {
        auto result = expressionToWGSL("abs(x)");
        expect(result.code == std::string("abs(x)"));
    };

    "generator converts log function"_test = [] {
        auto result = expressionToWGSL("log(x)");
        expect(result.code == std::string("log(x)"));
    };

    "generator converts exp function"_test = [] {
        auto result = expressionToWGSL("exp(x)");
        expect(result.code == std::string("exp(x)"));
    };

    "generator converts tan function"_test = [] {
        auto result = expressionToWGSL("tan(x)");
        expect(result.code == std::string("tan(x)"));
    };

    "generator converts atan function"_test = [] {
        auto result = expressionToWGSL("atan(x)");
        expect(result.code == std::string("atan(x)"));
    };

    "generator converts floor function"_test = [] {
        auto result = expressionToWGSL("floor(x)");
        expect(result.code == std::string("floor(x)"));
    };

    "generator converts ceil function"_test = [] {
        auto result = expressionToWGSL("ceil(x)");
        expect(result.code == std::string("ceil(x)"));
    };

    "generator converts min function"_test = [] {
        auto result = expressionToWGSL("min(x, y)");
        expect(result.code == std::string("min(x, y)"));
    };

    "generator converts max function"_test = [] {
        auto result = expressionToWGSL("max(x, y)");
        expect(result.code == std::string("max(x, y)"));
    };
};

suite yast_block_parser_tests = [] {
    "parser parses simple block"_test = [] {
        Parser parser("{plot: sin(x)}");
        auto node = parser.parseBlock();
        auto* block = dynamic_cast<BlockNode*>(node.get());
        expect(block != nullptr);
        expect(block->blockType == std::string("plot"));
    };

    "parser parses block with attributes"_test = [] {
        Parser parser("{plot x=0..10: sin(x)}");
        auto node = parser.parseBlock();
        auto* block = dynamic_cast<BlockNode*>(node.get());
        expect(block != nullptr);
        expect(block->blockType == std::string("plot"));
        expect(block->attrs.size() == 1_u);
        expect(block->findAttr("x") != nullptr);
    };

    "parser parses block with multiple attributes"_test = [] {
        Parser parser("{plot x=0..10 y=-1..1: sin(x)}");
        auto node = parser.parseBlock();
        auto* block = dynamic_cast<BlockNode*>(node.get());
        expect(block != nullptr);
        expect(block->attrs.size() == 2_u);
        expect(block->findAttr("x") != nullptr);
        expect(block->findAttr("y") != nullptr);
    };
};

suite yast_edge_cases = [] {
    "parser handles negative numbers"_test = [] {
        Parser parser("-3.14");
        auto node = parser.parseExpression();
        auto* unary = dynamic_cast<UnaryOpNode*>(node.get());
        expect(unary != nullptr);
        auto* num = dynamic_cast<NumberNode*>(unary->operand.get());
        expect(num != nullptr);
        expect(num->value > 3.13_d);
    };

    "parser handles double negative"_test = [] {
        Parser parser("--x");
        auto node = parser.parseExpression();
        auto* outer = dynamic_cast<UnaryOpNode*>(node.get());
        expect(outer != nullptr);
        auto* inner = dynamic_cast<UnaryOpNode*>(outer->operand.get());
        expect(inner != nullptr);
    };

    "generator handles deeply nested expression"_test = [] {
        auto result = expressionToWGSL("sin(cos(tan(x)))");
        expect(result.code == std::string("sin(cos(tan(x)))"));
        expect(result.usesX);
    };

    "generator handles complex math"_test = [] {
        auto result = expressionToWGSL("x^2 + 2*x + 1");
        expect(result.usesX);
        expect(!result.usesY);
    };

    "parser handles zero"_test = [] {
        Parser parser("0");
        auto node = parser.parseExpression();
        auto* num = dynamic_cast<NumberNode*>(node.get());
        expect(num != nullptr);
        expect(num->value == 0.0_d);
    };

    "parser handles scientific notation identifier"_test = [] {
        // 'e' alone should be identifier (Euler's number)
        Parser parser("e");
        auto node = parser.parseExpression();
        auto* id = dynamic_cast<IdentifierNode*>(node.get());
        expect(id != nullptr);
        expect(id->name == std::string("e"));
    };

    "generator handles multiple buffers"_test = [] {
        auto result = expressionToWGSL("@buffer1 + @buffer2");
        expect(result.bufferRefs.size() == 2_u);
    };

    "parser handles empty function call"_test = [] {
        Parser parser("time()");  // time with no args
        auto node = parser.parseExpression();
        auto* call = dynamic_cast<CallNode*>(node.get());
        expect(call != nullptr);
        expect(call->args.size() == 0_u);
    };

    "parser handles expression with all operators"_test = [] {
        Parser parser("a + b - c * d / e ^ f");
        auto node = parser.parseExpression();
        expect(node != nullptr);
    };

    "block parser handles negative range"_test = [] {
        Parser parser("{plot x=-pi..pi: sin(x)}");
        auto node = parser.parseBlock();
        auto* block = dynamic_cast<BlockNode*>(node.get());
        expect(block != nullptr);
        auto* attr = block->findAttr("x");
        expect(attr != nullptr);
        auto* range = dynamic_cast<RangeNode*>(attr->value.get());
        expect(range != nullptr);
    };
};

suite yast_plot_expression_tests = [] {
    "parser parses single plot definition"_test = [] {
        Parser parser("plot1 = sin(x)");
        auto node = parser.parsePlotExpression();
        expect(node != nullptr);
        expect(node->definitions.size() == 1_u);
        expect(node->definitions[0]->name == std::string("plot1"));
    };

    "parser parses plot with attribute"_test = [] {
        Parser parser("plot1 = sin(x); @plot1.color = #FF0000");
        auto node = parser.parsePlotExpression();
        expect(node != nullptr);
        expect(node->definitions.size() == 1_u);
        expect(node->attributes.size() == 1_u);
        expect(node->attributes[0]->plotName == std::string("plot1"));
        expect(node->attributes[0]->attrName == std::string("color"));
        expect(node->attributes[0]->value == std::string("#FF0000"));
    };

    "parser parses multiple plots"_test = [] {
        Parser parser("f1 = sin(x); f2 = cos(x); f3 = x * x");
        auto node = parser.parsePlotExpression();
        expect(node != nullptr);
        expect(node->definitions.size() == 3_u);
        expect(node->definitions[0]->name == std::string("f1"));
        expect(node->definitions[1]->name == std::string("f2"));
        expect(node->definitions[2]->name == std::string("f3"));
    };

    "parser parses plot with text attribute"_test = [] {
        Parser parser("wave = sin(x); @wave.text = 'Sine wave'");
        auto node = parser.parsePlotExpression();
        expect(node != nullptr);
        expect(node->definitions.size() == 1_u);
        expect(node->attributes.size() == 1_u);
        expect(node->attributes[0]->attrName == std::string("text"));
        expect(node->attributes[0]->value == std::string("Sine wave"));
    };

    "parser parses full multi-plot expression"_test = [] {
        Parser parser("plot1 = sin(x); @plot1.color = #FF0000; @plot1.text = 'Sine'; "
                      "plot2 = cos(x); @plot2.color = #00FF00; @plot2.text = 'Cosine'");
        auto node = parser.parsePlotExpression();
        expect(node != nullptr);
        expect(node->definitions.size() == 2_u);
        expect(node->attributes.size() == 4_u);
    };

    "generator generates multi-plot WGSL"_test = [] {
        auto result = plotExpressionToWGSL("plot1 = sin(x); @plot1.color = #FF0000; plot2 = cos(x)");
        expect(result.plots.size() == 2_u);
        expect(result.plots[0].name == std::string("plot1"));
        expect(result.plots[0].expr.code == std::string("sin(x)"));
        expect(result.plots[0].color == std::string("#FF0000"));
        expect(result.plots[1].name == std::string("plot2"));
        expect(result.plots[1].expr.code == std::string("cos(x)"));
    };

    "generator tracks time usage in multi-plot"_test = [] {
        auto result = plotExpressionToWGSL("wave = sin(x + time)");
        expect(result.plots.size() == 1_u);
        expect(result.plots[0].expr.usesTime);
        expect(result.plots[0].expr.usesX);
    };

    "parser handles expression with buffer reference"_test = [] {
        Parser parser("sig = buffer0 * sin(x)");
        auto node = parser.parsePlotExpression();
        expect(node != nullptr);
        expect(node->definitions.size() == 1_u);
    };

    "lexer tokenizes semicolon"_test = [] {
        Lexer lexer("a; b");
        expect(lexer.next().type == TokenType::Identifier);
        expect(lexer.next().type == TokenType::Semicolon);
        expect(lexer.next().type == TokenType::Identifier);
    };

    "lexer tokenizes hex color"_test = [] {
        Lexer lexer("#FF0000");
        auto tok = lexer.next();
        expect(tok.type == TokenType::HexColor);
        expect(tok.text == std::string_view("#FF0000"));
    };

    "lexer tokenizes single quoted string"_test = [] {
        Lexer lexer("'hello world'");
        auto tok = lexer.next();
        expect(tok.type == TokenType::String);
        expect(tok.text == std::string_view("hello world"));
    };

    "lexer tokenizes dot"_test = [] {
        Lexer lexer("@plot.color");
        expect(lexer.next().type == TokenType::At);
        expect(lexer.next().type == TokenType::Identifier);
        expect(lexer.next().type == TokenType::Dot);
        expect(lexer.next().type == TokenType::Identifier);
    };
};
