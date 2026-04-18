#include "yast.h"
#include <cctype>
#include <cstdlib>
#include <sstream>
#include <algorithm>

namespace yetty::yast {

//=============================================================================
// Lexer implementation
//=============================================================================

Lexer::Lexer(std::string_view source) : source_(source) {}

char Lexer::current() const {
    if (pos_ >= source_.size()) return '\0';
    return source_[pos_];
}

char Lexer::advance() {
    char c = current();
    if (c != '\0') {
        pos_++;
        if (c == '\n') {
            line_++;
            column_ = 1;
        } else {
            column_++;
        }
    }
    return c;
}

void Lexer::skipWhitespace() {
    while (std::isspace(current())) {
        advance();
    }
}

Token Lexer::makeToken(TokenType type, size_t start) {
    Token tok;
    tok.type = type;
    tok.text = source_.substr(start, pos_ - start);
    tok.line = line_;
    tok.column = column_ - tok.text.size();
    return tok;
}

Token Lexer::scanNumber() {
    size_t start = pos_;
    
    // Optional leading minus handled by parser as unary op
    while (std::isdigit(current())) advance();
    
    // Decimal part
    if (current() == '.' && std::isdigit(source_[pos_ + 1])) {
        advance(); // consume '.'
        while (std::isdigit(current())) advance();
    }
    
    // Scientific notation
    if (current() == 'e' || current() == 'E') {
        advance();
        if (current() == '+' || current() == '-') advance();
        while (std::isdigit(current())) advance();
    }
    
    Token tok = makeToken(TokenType::Number, start);
    tok.numValue = std::strtod(std::string(tok.text).c_str(), nullptr);
    return tok;
}

Token Lexer::scanIdentifier() {
    size_t start = pos_;
    
    while (std::isalnum(current()) || current() == '_') {
        advance();
    }
    
    return makeToken(TokenType::Identifier, start);
}

Token Lexer::scanString() {
    char quote = current();  // Could be " or '
    advance(); // consume opening quote
    size_t start = pos_;
    
    while (current() != quote && current() != '\0') {
        if (current() == '\\' && source_[pos_ + 1] != '\0') {
            advance(); // skip escape
        }
        advance();
    }
    
    Token tok = makeToken(TokenType::String, start);
    
    if (current() == quote) {
        advance(); // consume closing quote
    }
    
    return tok;
}

Token Lexer::peek() {
    if (!peeked_) {
        peeked_ = next();
    }
    return *peeked_;
}

Token Lexer::next() {
    if (peeked_) {
        Token tok = *peeked_;
        peeked_.reset();
        return tok;
    }
    
    skipWhitespace();
    
    if (pos_ >= source_.size()) {
        return {TokenType::Eof, "", line_, column_};
    }
    
    char c = current();
    size_t start = pos_;
    
    // Single character tokens
    switch (c) {
        case '+': advance(); return makeToken(TokenType::Plus, start);
        case '-': advance(); return makeToken(TokenType::Minus, start);
        case '*': advance(); return makeToken(TokenType::Star, start);
        case '/': advance(); return makeToken(TokenType::Slash, start);
        case '^': advance(); return makeToken(TokenType::Caret, start);
        case '(': advance(); return makeToken(TokenType::LParen, start);
        case ')': advance(); return makeToken(TokenType::RParen, start);
        case '{': advance(); return makeToken(TokenType::LBrace, start);
        case '}': advance(); return makeToken(TokenType::RBrace, start);
        case ':': advance(); return makeToken(TokenType::Colon, start);
        case ',': advance(); return makeToken(TokenType::Comma, start);
        case '=': advance(); return makeToken(TokenType::Equals, start);
        case '@': advance(); return makeToken(TokenType::At, start);
        case ';': advance(); return makeToken(TokenType::Semicolon, start);
        case '"': return scanString();
        case '\'': return scanString();  // Also support single quotes
        
        case '#': {
            // Hex color: #RRGGBB or #RGB
            advance();
            size_t colorStart = pos_;
            while (std::isxdigit(current())) advance();
            Token tok;
            tok.type = TokenType::HexColor;
            tok.text = source_.substr(start, pos_ - start);
            tok.line = line_;
            tok.column = column_ - tok.text.size();
            return tok;
        }
        
        case '.':
            if (source_[pos_ + 1] == '.') {
                advance(); advance();
                return makeToken(TokenType::DotDot, start);
            }
            // Single dot
            if (!std::isdigit(source_[pos_ + 1])) {
                advance();
                return makeToken(TokenType::Dot, start);
            }
            // Fall through to number if followed by digit
            break;
    }
    
    // Numbers
    if (std::isdigit(c)) {
        return scanNumber();
    }
    
    // Identifiers
    if (std::isalpha(c) || c == '_') {
        return scanIdentifier();
    }
    
    // Unknown character
    advance();
    Token tok = makeToken(TokenType::Error, start);
    return tok;
}

//=============================================================================
// Parser implementation
//=============================================================================

Parser::Parser(std::string_view source) : lexer_(source) {
    advance();
}

void Parser::advance() {
    current_ = lexer_.next();
}

bool Parser::check(TokenType type) const {
    return current_.type == type;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

void Parser::expect(TokenType type, const char* message) {
    if (!match(type)) {
        error(message);
    }
}

void Parser::error(const char* message) {
    errors_.push_back({message, current_.line, current_.column});
}

//-----------------------------------------------------------------------------
// Expression parsing
//-----------------------------------------------------------------------------

NodePtr Parser::parseExpression() {
    return parseExpr();
}

//-----------------------------------------------------------------------------
// Multi-plot expression parsing
// Grammar: statement (';' statement)*
// statement: IDENT '=' expr | '@' IDENT '.' IDENT '=' value
//-----------------------------------------------------------------------------

std::shared_ptr<PlotExprNode> Parser::parsePlotExpression() {
    auto result = std::make_shared<PlotExprNode>();
    
    while (!check(TokenType::Eof)) {
        // Skip leading semicolons
        while (match(TokenType::Semicolon)) {}
        
        if (check(TokenType::Eof)) break;
        
        if (check(TokenType::At)) {
            // Plot attribute: @name.attr = value
            advance();  // consume '@'
            
            if (!check(TokenType::Identifier)) {
                error("expected identifier after '@'");
                break;
            }
            std::string plotName(current_.text);
            advance();
            
            if (!match(TokenType::Dot)) {
                error("expected '.' after plot name");
                break;
            }
            
            if (!check(TokenType::Identifier)) {
                error("expected attribute name after '.'");
                break;
            }
            std::string attrName(current_.text);
            advance();
            
            if (!match(TokenType::Equals)) {
                error("expected '=' after attribute name");
                break;
            }
            
            // Parse value: could be hex color, string, or identifier
            std::string value;
            if (check(TokenType::HexColor)) {
                value = std::string(current_.text);
                advance();
            } else if (check(TokenType::String)) {
                value = std::string(current_.text);
                advance();
            } else if (check(TokenType::Identifier)) {
                value = std::string(current_.text);
                advance();
            } else {
                error("expected value after '='");
                break;
            }
            
            result->attributes.push_back(
                std::make_shared<PlotAttrNode>(std::move(plotName), std::move(attrName), std::move(value)));
        } else if (check(TokenType::Identifier)) {
            // Could be: name = expr  OR  bare expression like sin(x)
            // Look ahead to distinguish
            std::string name(current_.text);
            advance();

            if (match(TokenType::Equals)) {
                // Plot definition: name = expr
                auto expr = parseExpr();
                if (!expr) {
                    error("expected expression after '='");
                    break;
                }

                result->definitions.push_back(
                    std::make_shared<PlotDefNode>(std::move(name), std::move(expr)));
            } else {
                // Bare expression: sin(x), x*x, etc.
                // Put back the identifier and parse as full expression
                // We already consumed the identifier, so handle function call or just identifier
                NodePtr expr;
                if (check(TokenType::LParen)) {
                    // Function call: sin(x)
                    advance(); // consume '('
                    std::vector<NodePtr> args;
                    if (!check(TokenType::RParen)) {
                        args.push_back(parseExpr());
                        while (match(TokenType::Comma)) {
                            args.push_back(parseExpr());
                        }
                    }
                    if (!match(TokenType::RParen)) {
                        error("expected ')' after function arguments");
                        break;
                    }
                    expr = std::make_shared<CallNode>(std::move(name), std::move(args));

                    // Handle any operators after the function call
                    while (check(TokenType::Plus) || check(TokenType::Minus) ||
                           check(TokenType::Star) || check(TokenType::Slash)) {
                        BinaryOpNode::Op op;
                        if (check(TokenType::Plus)) op = BinaryOpNode::Op::Add;
                        else if (check(TokenType::Minus)) op = BinaryOpNode::Op::Sub;
                        else if (check(TokenType::Star)) op = BinaryOpNode::Op::Mul;
                        else op = BinaryOpNode::Op::Div;
                        advance();
                        auto right = parseTerm();
                        expr = std::make_shared<BinaryOpNode>(op, std::move(expr), std::move(right));
                    }
                } else {
                    // Just an identifier or identifier with operators (x*x)
                    expr = std::make_shared<IdentifierNode>(std::move(name));

                    // Handle operators after the identifier
                    while (check(TokenType::Plus) || check(TokenType::Minus) ||
                           check(TokenType::Star) || check(TokenType::Slash)) {
                        BinaryOpNode::Op op;
                        if (check(TokenType::Plus)) op = BinaryOpNode::Op::Add;
                        else if (check(TokenType::Minus)) op = BinaryOpNode::Op::Sub;
                        else if (check(TokenType::Star)) op = BinaryOpNode::Op::Mul;
                        else op = BinaryOpNode::Op::Div;
                        advance();
                        auto right = parseTerm();
                        expr = std::make_shared<BinaryOpNode>(op, std::move(expr), std::move(right));
                    }
                }

                // Auto-generate name for bare expression
                std::string autoName = "plot" + std::to_string(result->definitions.size() + 1);
                result->definitions.push_back(
                    std::make_shared<PlotDefNode>(std::move(autoName), std::move(expr)));
            }
        } else if (check(TokenType::Number) || check(TokenType::LParen) || check(TokenType::Minus)) {
            // Bare expression starting with number, parenthesis, or unary minus
            auto expr = parseExpr();
            if (!expr) {
                error("expected expression");
                break;
            }

            std::string autoName = "plot" + std::to_string(result->definitions.size() + 1);
            result->definitions.push_back(
                std::make_shared<PlotDefNode>(std::move(autoName), std::move(expr)));
        } else {
            error("expected plot definition or expression");
            break;
        }
        
        // Optional semicolon after statement
        match(TokenType::Semicolon);
    }
    
    return result;
}

NodePtr Parser::parseExpr() {
    auto left = parseTerm();
    
    while (check(TokenType::Plus) || check(TokenType::Minus)) {
        auto op = current_.type == TokenType::Plus 
            ? BinaryOpNode::Op::Add : BinaryOpNode::Op::Sub;
        advance();
        auto right = parseTerm();
        left = std::make_shared<BinaryOpNode>(op, std::move(left), std::move(right));
    }
    
    return left;
}

NodePtr Parser::parseTerm() {
    auto left = parseFactor();
    
    while (check(TokenType::Star) || check(TokenType::Slash)) {
        auto op = current_.type == TokenType::Star 
            ? BinaryOpNode::Op::Mul : BinaryOpNode::Op::Div;
        advance();
        auto right = parseFactor();
        left = std::make_shared<BinaryOpNode>(op, std::move(left), std::move(right));
    }
    
    return left;
}

NodePtr Parser::parseFactor() {
    auto left = parseUnary();
    
    if (check(TokenType::Caret)) {
        advance();
        auto right = parseUnary();
        left = std::make_shared<BinaryOpNode>(BinaryOpNode::Op::Pow, std::move(left), std::move(right));
    }
    
    return left;
}

NodePtr Parser::parseUnary() {
    if (check(TokenType::Minus)) {
        advance();
        auto operand = parseUnary();
        return std::make_shared<UnaryOpNode>(UnaryOpNode::Op::Neg, std::move(operand));
    }
    
    return parsePrimary();
}

NodePtr Parser::parsePrimary() {
    // Number
    if (check(TokenType::Number)) {
        auto node = std::make_shared<NumberNode>(current_.numValue);
        node->line = current_.line;
        node->column = current_.column;
        advance();
        return node;
    }
    
    // Buffer reference: @buffer1
    if (check(TokenType::At)) {
        advance();
        if (!check(TokenType::Identifier)) {
            error("expected identifier after '@'");
            return nullptr;
        }
        std::string name(current_.text);
        advance();
        
        // Extract buffer index from name like "buffer1"
        int index = 0;
        if (name.substr(0, 6) == "buffer") {
            index = std::atoi(name.substr(6).c_str());
        }
        
        return std::make_shared<BufferRefNode>(name, index);
    }
    
    // Identifier or function call
    if (check(TokenType::Identifier)) {
        std::string name(current_.text);
        advance();
        
        // Function call: sin(x)
        if (check(TokenType::LParen)) {
            return parseCall(std::move(name));
        }
        
        // Simple identifier
        return std::make_shared<IdentifierNode>(std::move(name));
    }
    
    // Parenthesized expression
    if (check(TokenType::LParen)) {
        advance();
        auto expr = parseExpr();
        expect(TokenType::RParen, "expected ')'");
        return expr;
    }
    
    error("expected expression");
    return nullptr;
}

NodePtr Parser::parseCall(std::string name) {
    expect(TokenType::LParen, "expected '('");
    
    NodeList args;
    
    if (!check(TokenType::RParen)) {
        args.push_back(parseExpr());
        
        while (match(TokenType::Comma)) {
            args.push_back(parseExpr());
        }
    }
    
    expect(TokenType::RParen, "expected ')'");
    
    return std::make_shared<CallNode>(std::move(name), std::move(args));
}

//-----------------------------------------------------------------------------
// Range and attribute parsing
//-----------------------------------------------------------------------------

NodePtr Parser::parseRange() {
    auto start = parseExpr();
    
    if (match(TokenType::DotDot)) {
        auto end = parseExpr();
        return std::make_shared<RangeNode>(std::move(start), std::move(end));
    }
    
    return start;
}

AttributeNode Parser::parseAttribute() {
    std::string name;
    
    // Handle @name attributes
    if (check(TokenType::At)) {
        advance();
        name = "@";
    }
    
    if (!check(TokenType::Identifier)) {
        error("expected attribute name");
        return {"", nullptr};
    }
    
    name += std::string(current_.text);
    advance();
    
    NodePtr value = nullptr;
    
    if (match(TokenType::Equals)) {
        value = parseRange();
    }
    
    return {std::move(name), std::move(value)};
}

std::vector<AttributeNode> Parser::parseAttributes() {
    std::vector<AttributeNode> attrs;
    
    while (!check(TokenType::Colon) && !check(TokenType::Eof) && !check(TokenType::RBrace)) {
        attrs.push_back(parseAttribute());
    }
    
    return attrs;
}

//-----------------------------------------------------------------------------
// Block parsing
//-----------------------------------------------------------------------------

std::shared_ptr<BlockNode> Parser::parseBlock() {
    auto block = std::make_shared<BlockNode>();
    
    if (!match(TokenType::LBrace)) {
        error("expected '{'");
        return block;
    }
    
    // Check if first token is a block type (identifier without '=')
    if (check(TokenType::Identifier)) {
        Token next = lexer_.peek();
        if (next.type != TokenType::Equals) {
            block->blockType = std::string(current_.text);
            advance();
        }
    }
    
    // Parse attributes until ':'
    block->attrs = parseAttributes();
    
    // Expect ':'
    if (!match(TokenType::Colon)) {
        error("expected ':' after attributes");
    }
    
    // Parse content until '}'
    block->content = parseContent();
    
    expect(TokenType::RBrace, "expected '}'");
    
    return block;
}

NodeList Parser::parseContent() {
    NodeList content;
    std::string textBuffer;
    
    auto flushText = [&]() {
        if (!textBuffer.empty()) {
            content.push_back(std::make_shared<TextNode>(std::move(textBuffer)));
            textBuffer.clear();
        }
    };
    
    while (!check(TokenType::RBrace) && !check(TokenType::Eof)) {
        // Nested block
        if (check(TokenType::LBrace)) {
            flushText();
            content.push_back(parseBlock());
            continue;
        }
        
        // For content mode, we want to capture most things as text
        // But expressions can be parsed in certain contexts
        
        // Simple approach: accumulate text
        if (current_.type != TokenType::Eof && current_.type != TokenType::RBrace) {
            textBuffer += std::string(current_.text);
            if (current_.type == TokenType::Number || 
                current_.type == TokenType::Identifier) {
                // Add space after identifiers/numbers if followed by same
                Token next = lexer_.peek();
                if (next.type == TokenType::Identifier || 
                    next.type == TokenType::Number) {
                    textBuffer += " ";
                }
            }
            advance();
        }
    }
    
    flushText();
    return content;
}

//=============================================================================
// WGSL Generator implementation
//=============================================================================

WGSLGenerator::Result WGSLGenerator::generate(const Node* node) {
    result_ = Result{};
    result_.code = visit(node);
    return result_;
}

std::string WGSLGenerator::visit(const Node* node) {
    if (!node) return "0.0";
    
    if (node->isNumber()) return visitNumber(node->as<NumberNode>());
    if (node->isIdentifier()) return visitIdentifier(node->as<IdentifierNode>());
    if (node->isBufferRef()) return visitBufferRef(node->as<BufferRefNode>());
    if (node->isBinaryOp()) return visitBinaryOp(node->as<BinaryOpNode>());
    if (node->isUnaryOp()) return visitUnaryOp(node->as<UnaryOpNode>());
    if (node->isCall()) return visitCall(node->as<CallNode>());
    
    return "0.0";
}

std::string WGSLGenerator::visitNumber(const NumberNode* node) {
    std::ostringstream ss;
    ss << node->value;
    std::string s = ss.str();
    
    // Ensure WGSL float format (must have decimal point)
    if (s.find('.') == std::string::npos && s.find('e') == std::string::npos) {
        s += ".0";
    }
    
    return s;
}

std::string WGSLGenerator::visitIdentifier(const IdentifierNode* node) {
    const std::string& name = node->name;
    
    // Built-in constants
    if (name == "pi") return "3.14159265358979323846";
    if (name == "e") return "2.71828182845904523536";
    
    // Track variable usage
    if (name == "x") result_.usesX = true;
    if (name == "y") result_.usesY = true;
    if (name == "time") result_.usesTime = true;
    
    return name;
}

std::string WGSLGenerator::visitBufferRef(const BufferRefNode* node) {
    // Track buffer reference
    if (std::find(result_.bufferRefs.begin(), result_.bufferRefs.end(), node->index) 
        == result_.bufferRefs.end()) {
        result_.bufferRefs.push_back(node->index);
    }
    
    // Generate buffer sampling call
    std::ostringstream ss;
    ss << "sampleBuffer(" << (node->index - 1) << ", x)";
    return ss.str();
}

std::string WGSLGenerator::visitBinaryOp(const BinaryOpNode* node) {
    std::string left = visit(node->left.get());
    std::string right = visit(node->right.get());
    
    // Power needs special handling in WGSL
    if (node->op == BinaryOpNode::Op::Pow) {
        return "pow(" + left + ", " + right + ")";
    }
    
    const char* op = "";
    switch (node->op) {
        case BinaryOpNode::Op::Add: op = " + "; break;
        case BinaryOpNode::Op::Sub: op = " - "; break;
        case BinaryOpNode::Op::Mul: op = " * "; break;
        case BinaryOpNode::Op::Div: op = " / "; break;
        default: break;
    }
    
    return "(" + left + op + right + ")";
}

std::string WGSLGenerator::visitUnaryOp(const UnaryOpNode* node) {
    std::string operand = visit(node->operand.get());
    
    if (node->op == UnaryOpNode::Op::Neg) {
        return "(-" + operand + ")";
    }
    
    return operand;
}

std::string WGSLGenerator::visitCall(const CallNode* node) {
    std::ostringstream ss;
    
    // Map function names to WGSL equivalents
    std::string fn = node->name;
    
    // Most math functions have same name in WGSL
    // log → log (natural log in WGSL)
    // log10 not available, use log(x)/log(10)
    if (fn == "log10") {
        ss << "(log(" << visit(node->args[0].get()) << ") / 2.302585093)";
        return ss.str();
    }
    if (fn == "ln") fn = "log";  // Alias
    
    ss << fn << "(";
    for (size_t i = 0; i < node->args.size(); ++i) {
        if (i > 0) ss << ", ";
        ss << visit(node->args[i].get());
    }
    ss << ")";
    
    return ss.str();
}

WGSLGenerator::MultiResult WGSLGenerator::generateMulti(const PlotExprNode* node) {
    MultiResult multi;
    
    if (!node) return multi;
    
    // Process each definition
    for (const auto& def : node->definitions) {
        PlotResult pr;
        pr.name = def->name;
        
        // Generate WGSL for the expression
        result_ = Result{};
        pr.expr.code = visit(def->expression.get());
        pr.expr.usesX = result_.usesX;
        pr.expr.usesY = result_.usesY;
        pr.expr.usesTime = result_.usesTime;
        pr.expr.bufferRefs = result_.bufferRefs;
        
        // Find attributes for this plot
        auto attrs = node->findAttrs(def->name);
        for (const auto* attr : attrs) {
            if (attr->attrName == "color") {
                pr.color = attr->value;
            } else if (attr->attrName == "text") {
                pr.text = attr->value;
            }
        }
        
        multi.plots.push_back(std::move(pr));
    }
    
    return multi;
}

//=============================================================================
// Convenience functions
//=============================================================================

NodePtr parseExpression(std::string_view source) {
    Parser parser(source);
    return parser.parseExpression();
}

std::shared_ptr<PlotExprNode> parsePlotExpression(std::string_view source) {
    Parser parser(source);
    return parser.parsePlotExpression();
}

std::shared_ptr<BlockNode> parseBlock(std::string_view source) {
    Parser parser(source);
    return parser.parseBlock();
}

WGSLGenerator::Result expressionToWGSL(std::string_view source) {
    Parser parser(source);
    auto ast = parser.parseExpression();
    
    if (parser.hasErrors() || !ast) {
        return {};
    }
    
    WGSLGenerator gen;
    return gen.generate(ast.get());
}

WGSLGenerator::MultiResult plotExpressionToWGSL(std::string_view source) {
    Parser parser(source);
    auto ast = parser.parsePlotExpression();
    
    if (parser.hasErrors() || !ast) {
        return {};
    }
    
    WGSLGenerator gen;
    return gen.generateMulti(ast.get());
}

} // namespace yetty::yast
