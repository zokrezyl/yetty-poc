#pragma once

// YAST - Yetty Abstract Syntax Tree
//
// A reusable parser for yetty's expression and block syntax.
// Used by plot card, yecho, and other components that need
// to parse mathematical expressions or structured blocks.
//
// Grammar overview:
//   block      = '{' attrs ':' content '}'
//   attrs      = (IDENT '=' value)*
//   content    = (text | block | expr)*
//   expr       = term (('+' | '-') term)*
//   term       = factor (('*' | '/') factor)*
//   factor     = unary ('^' unary)?
//   unary      = '-'? primary
//   primary    = NUMBER | IDENT | IDENT '(' args ')' | '(' expr ')' | '@' IDENT

#include <memory>
#include <string>
#include <string_view>
#include <variant>
#include <vector>
#include <optional>
#include <unordered_map>

namespace yetty::yast {

//=============================================================================
// Token types
//=============================================================================

enum class TokenType {
    // Literals
    Number,         // 123, 3.14, -2.5
    Identifier,     // sin, x, time, buffer1
    String,         // "hello" or 'hello'
    HexColor,       // #FF0000
    
    // Operators
    Plus,           // +
    Minus,          // -
    Star,           // *
    Slash,          // /
    Caret,          // ^
    
    // Delimiters
    LParen,         // (
    RParen,         // )
    LBrace,         // {
    RBrace,         // }
    Colon,          // :
    Comma,          // ,
    Equals,         // =
    At,             // @
    Dot,            // .
    DotDot,         // ..
    Semicolon,      // ;
    
    // Special
    Text,           // Plain text (in content mode)
    Eof,
    Error
};

struct Token {
    TokenType type;
    std::string_view text;
    size_t line;
    size_t column;
    
    // For Number tokens
    double numValue = 0.0;
};

//=============================================================================
// AST Node types
//=============================================================================

struct Node;
using NodePtr = std::shared_ptr<Node>;
using NodeList = std::vector<NodePtr>;

// Forward declarations
struct NumberNode;
struct IdentifierNode;
struct BufferRefNode;
struct BinaryOpNode;
struct UnaryOpNode;
struct CallNode;
struct BlockNode;
struct TextNode;
struct RangeNode;
struct AttributeNode;
struct PlotDefNode;
struct PlotAttrNode;
struct PlotExprNode;

// Base node with location info
struct Node {
    size_t line = 0;
    size_t column = 0;
    
    virtual ~Node() = default;
    
    // Type checking helpers
    virtual bool isNumber() const { return false; }
    virtual bool isIdentifier() const { return false; }
    virtual bool isBufferRef() const { return false; }
    virtual bool isBinaryOp() const { return false; }
    virtual bool isUnaryOp() const { return false; }
    virtual bool isCall() const { return false; }
    virtual bool isBlock() const { return false; }
    virtual bool isText() const { return false; }
    virtual bool isRange() const { return false; }
    virtual bool isAttribute() const { return false; }
    virtual bool isPlotDef() const { return false; }
    virtual bool isPlotAttr() const { return false; }
    virtual bool isPlotExpr() const { return false; }
    
    // Safe casting
    template<typename T> T* as() { return dynamic_cast<T*>(this); }
    template<typename T> const T* as() const { return dynamic_cast<const T*>(this); }
};

// Numeric literal: 42, 3.14, -1.5
struct NumberNode : Node {
    double value;
    
    explicit NumberNode(double v) : value(v) {}
    bool isNumber() const override { return true; }
};

// Identifier: x, time, sin, pi
struct IdentifierNode : Node {
    std::string name;
    
    explicit IdentifierNode(std::string n) : name(std::move(n)) {}
    bool isIdentifier() const override { return true; }
};

// Buffer reference: @buffer1, @buffer2
struct BufferRefNode : Node {
    std::string name;   // "buffer1", "buffer2"
    int index;          // 1, 2, ...
    
    BufferRefNode(std::string n, int i) : name(std::move(n)), index(i) {}
    bool isBufferRef() const override { return true; }
};

// Binary operation: a + b, x * 2
struct BinaryOpNode : Node {
    enum class Op { Add, Sub, Mul, Div, Pow };
    
    Op op;
    NodePtr left;
    NodePtr right;
    
    BinaryOpNode(Op o, NodePtr l, NodePtr r) 
        : op(o), left(std::move(l)), right(std::move(r)) {}
    bool isBinaryOp() const override { return true; }
    
    static const char* opToString(Op o) {
        switch (o) {
            case Op::Add: return "+";
            case Op::Sub: return "-";
            case Op::Mul: return "*";
            case Op::Div: return "/";
            case Op::Pow: return "^";
        }
        return "?";
    }
};

// Unary operation: -x, -5
struct UnaryOpNode : Node {
    enum class Op { Neg };
    
    Op op;
    NodePtr operand;
    
    UnaryOpNode(Op o, NodePtr operand) : op(o), operand(std::move(operand)) {}
    bool isUnaryOp() const override { return true; }
};

// Function call: sin(x), pow(x, 2)
struct CallNode : Node {
    std::string name;
    NodeList args;
    
    CallNode(std::string n, NodeList a) : name(std::move(n)), args(std::move(a)) {}
    bool isCall() const override { return true; }
};

// Range: -pi..pi, 0..10
struct RangeNode : Node {
    NodePtr start;
    NodePtr end;
    
    RangeNode(NodePtr s, NodePtr e) : start(std::move(s)), end(std::move(e)) {}
    bool isRange() const override { return true; }
};

// Attribute: x=-pi..pi, @view=-1..1,0..1
struct AttributeNode : Node {
    std::string name;       // "x", "@view"
    NodePtr value;          // Could be Range, Number, or expression
    
    AttributeNode(std::string n, NodePtr v) : name(std::move(n)), value(std::move(v)) {}
    bool isAttribute() const override { return true; }
};

// Plain text content
struct TextNode : Node {
    std::string text;
    
    explicit TextNode(std::string t) : text(std::move(t)) {}
    bool isText() const override { return true; }
};

// Block: {attrs: content}
struct BlockNode : Node {
    std::string blockType;              // First word if no '=' (e.g., "plot", "ytext")
    std::vector<AttributeNode> attrs;   // Attributes before ':'
    NodeList content;                   // Content after ':'
    
    bool isBlock() const override { return true; }
    
    // Helper to find attribute by name
    const AttributeNode* findAttr(const std::string& name) const {
        for (const auto& attr : attrs) {
            if (attr.name == name) return &attr;
        }
        return nullptr;
    }
};

// Plot definition: name = expr (e.g., "plot1 = sin(x)")
struct PlotDefNode : Node {
    std::string name;       // "plot1"
    NodePtr expression;     // The math expression AST
    
    PlotDefNode(std::string n, NodePtr e) : name(std::move(n)), expression(std::move(e)) {}
    bool isPlotDef() const override { return true; }
};

// Plot attribute: @name.attr = value (e.g., "@plot1.color = #FF0000")
struct PlotAttrNode : Node {
    std::string plotName;   // "plot1"
    std::string attrName;   // "color", "text"
    std::string value;      // "#FF0000" or "'Some text'"
    
    PlotAttrNode(std::string pn, std::string an, std::string v) 
        : plotName(std::move(pn)), attrName(std::move(an)), value(std::move(v)) {}
    bool isPlotAttr() const override { return true; }
};

// Container for multiple plot definitions: "plot1 = sin(x); @plot1.color = #FF0000; plot2 = cos(x)"
struct PlotExprNode : Node {
    std::vector<std::shared_ptr<PlotDefNode>> definitions;
    std::vector<std::shared_ptr<PlotAttrNode>> attributes;
    
    bool isPlotExpr() const override { return true; }
    
    // Helper to find definition by name
    const PlotDefNode* findDef(const std::string& name) const {
        for (const auto& def : definitions) {
            if (def->name == name) return def.get();
        }
        return nullptr;
    }
    
    // Helper to find attributes for a plot
    std::vector<const PlotAttrNode*> findAttrs(const std::string& plotName) const {
        std::vector<const PlotAttrNode*> result;
        for (const auto& attr : attributes) {
            if (attr->plotName == plotName) {
                result.push_back(attr.get());
            }
        }
        return result;
    }
};

//=============================================================================
// Lexer
//=============================================================================

class Lexer {
public:
    explicit Lexer(std::string_view source);
    
    Token next();
    Token peek();
    
    // Switch modes for different parsing contexts
    void setExpressionMode(bool expr) { expressionMode_ = expr; }
    
private:
    std::string_view source_;
    size_t pos_ = 0;
    size_t line_ = 1;
    size_t column_ = 1;
    bool expressionMode_ = true;
    std::optional<Token> peeked_;
    
    char current() const;
    char advance();
    void skipWhitespace();
    Token makeToken(TokenType type, size_t start);
    Token scanNumber();
    Token scanIdentifier();
    Token scanString();
};

//=============================================================================
// Parser
//=============================================================================

class Parser {
public:
    struct Error {
        std::string message;
        size_t line;
        size_t column;
    };
    
    explicit Parser(std::string_view source);
    
    // Parse a complete expression (for simple plot expressions like "sin(x)")
    NodePtr parseExpression();
    
    // Parse a multi-plot expression (e.g., "plot1 = sin(x); @plot1.color = #FF0000; plot2 = cos(x)")
    std::shared_ptr<PlotExprNode> parsePlotExpression();
    
    // Parse a block with attributes and content
    std::shared_ptr<BlockNode> parseBlock();
    
    // Parse attribute list (before ':')
    std::vector<AttributeNode> parseAttributes();
    
    // Get parse errors
    const std::vector<Error>& errors() const { return errors_; }
    bool hasErrors() const { return !errors_.empty(); }
    
private:
    Lexer lexer_;
    Token current_;
    std::vector<Error> errors_;
    
    void advance();
    bool check(TokenType type) const;
    bool match(TokenType type);
    void expect(TokenType type, const char* message);
    void error(const char* message);
    
    // Expression parsing (recursive descent)
    NodePtr parseExpr();
    NodePtr parseTerm();
    NodePtr parseFactor();
    NodePtr parseUnary();
    NodePtr parsePrimary();
    NodePtr parseCall(std::string name);
    
    // Attribute/range parsing
    NodePtr parseRange();
    AttributeNode parseAttribute();
    
    // Content parsing (text + nested blocks)
    NodeList parseContent();
};

//=============================================================================
// WGSL Code Generator
//=============================================================================

class WGSLGenerator {
public:
    struct Result {
        std::string code;               // WGSL expression string
        std::vector<int> bufferRefs;    // Buffer indices referenced
        bool usesTime = false;          // References 'time' uniform
        bool usesX = false;             // References 'x' variable
        bool usesY = false;             // References 'y' variable
    };
    
    // Result for multi-plot expressions
    struct PlotResult {
        std::string name;               // Plot name (e.g., "plot1")
        Result expr;                    // WGSL for the expression
        std::string color;              // Hex color (e.g., "#FF0000") or empty
        std::string text;               // Label text or empty
    };
    
    struct MultiResult {
        std::vector<PlotResult> plots;  // All plot definitions with their attributes
    };
    
    // Generate WGSL expression from AST
    Result generate(const Node* node);
    
    // Generate WGSL from multi-plot expression
    MultiResult generateMulti(const PlotExprNode* node);
    
private:
    Result result_;
    
    std::string visit(const Node* node);
    std::string visitNumber(const NumberNode* node);
    std::string visitIdentifier(const IdentifierNode* node);
    std::string visitBufferRef(const BufferRefNode* node);
    std::string visitBinaryOp(const BinaryOpNode* node);
    std::string visitUnaryOp(const UnaryOpNode* node);
    std::string visitCall(const CallNode* node);
};

//=============================================================================
// Utility functions
//=============================================================================

// Parse just an expression string (convenience wrapper)
NodePtr parseExpression(std::string_view source);

// Parse a multi-plot expression string (convenience wrapper)
std::shared_ptr<PlotExprNode> parsePlotExpression(std::string_view source);

// Parse a block string (convenience wrapper)
std::shared_ptr<BlockNode> parseBlock(std::string_view source);

// Generate WGSL from simple expression string
WGSLGenerator::Result expressionToWGSL(std::string_view source);

// Generate WGSL from multi-plot expression string
WGSLGenerator::MultiResult plotExpressionToWGSL(std::string_view source);

} // namespace yetty::yast
