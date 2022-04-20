#include <CFG/cfg.hpp>
#include <common/errorlogger.hpp>
#include <parser/parser.hpp>

namespace Crust {

void Parser::skipToNextSemiColon() {
    while (mCurrentToken != Lexer::Token::TOK_EOF and mCurrentToken != Lexer::Token::SEMI_COLON) {
        mCurrentToken = mLexer.getNextToken();
    }
}

std::unique_ptr<CFGNode> Parser::parseProgram(const std::string& filename) {
    //    Check the extension?
    if (!mLexer.init(filename)) {
        ErrorLogger::printError(ErrorLogger::ErrorType::ERROR_OPENING_FILE);
        return nullptr;
    }

    mCurrentToken = mLexer.getNextToken();
    return parseProgramDecl();
}

std::unique_ptr<ProgDecl> Parser::parseProgramDecl() {
    auto progDecl = std::make_unique<ProgDecl>();
    auto declListNode = std::make_unique<DeclList>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and mCurrentToken != Lexer::Token::KW_FN and (mCurrentToken < Lexer::Token::KW_INT_32 and mCurrentToken > Lexer::Token::KW_VOID) and (mCurrentToken != Lexer::Token::LBRACKET)) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::EXPECTED_DECL, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    switch (mCurrentToken) {
        case Lexer::Token::TOK_EOF:
            break;
        case Lexer::Token::KW_FN:
        case Lexer::Token::KW_INT_32... Lexer::Token::KW_VOID:
        case Lexer::Token::LBRACKET:

            declListNode.reset(parseDeclList().release());
            progDecl.reset(new ProgDecl(std::move(declListNode)));

            break;

        default:
            ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::EXPECTED_DECL, mLexer.GetCurrentLocation());
            break;
    }

    return progDecl;
}

std::unique_ptr<DeclList> Parser::parseDeclList() {
    auto declList = std::make_unique<DeclList>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and mCurrentToken != Lexer::Token::KW_FN and (mCurrentToken < Lexer::Token::KW_INT_32 and mCurrentToken > Lexer::Token::KW_VOID) and (mCurrentToken != Lexer::Token::LBRACKET)) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::EXPECTED_DECL, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    auto declNode = std::make_unique<Decl>();
    auto declListNode = std::make_unique<DeclList>();

    switch (mCurrentToken) {
        case Lexer::Token::TOK_EOF:
            break;

        case Lexer::Token::KW_FN:
        case Lexer::Token::KW_INT_32... Lexer::Token::KW_VOID:
        case Lexer::Token::LBRACKET:

            declNode.reset(parseDecl().release());
            declListNode.reset(parseDeclList().release());

            declList.reset(new DeclList(std::move(declNode), std::move(declListNode)));
            break;

        default:
            ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::EXPECTED_DECL, mLexer.GetCurrentLocation());
            break;
    }

    return declList;
}

std::unique_ptr<Decl> Parser::parseDecl() {
    auto declNode = std::make_unique<Decl>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and mCurrentToken != Lexer::Token::KW_FN and (mCurrentToken < Lexer::Token::KW_INT_32 and mCurrentToken > Lexer::Token::KW_VOID) and (mCurrentToken != Lexer::Token::LBRACKET)) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::EXPECTED_DECL, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    auto varDecl = std::make_unique<VarDecl>();
    auto fnDecl = std::make_unique<FnDecl>();

    switch (mCurrentToken) {
        case Lexer::Token::KW_FN:
            fnDecl.reset(parseFnDecl().release());
            declNode.reset(new Decl(std::move(fnDecl)));
            break;

        case Lexer::Token::KW_INT_32... Lexer::Token::KW_VOID:
        case Lexer::Token::LBRACKET:
            varDecl.reset(parseVarDecl().release());
            declNode.reset(new Decl(std::move(varDecl)));

            break;

        default:
            ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::EXPECTED_DECL, mLexer.GetCurrentLocation());
            break;
    }

    return declNode;
}

std::unique_ptr<VarDecl> Parser::parseVarDecl() {
    auto varDeclNode = std::make_unique<VarDecl>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and (mCurrentToken < Lexer::Token::KW_INT_32 and mCurrentToken > Lexer::Token::KW_VOID) and (mCurrentToken != Lexer::Token::LBRACKET)) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::EXPECTED_DECL, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    auto type = std::make_unique<Type>();
    auto atomicDeclList = std::make_unique<AtomicDeclList>();
    auto semi_colon = std::make_unique<Token>();

    switch (mCurrentToken) {
        case Lexer::Token::KW_INT_32... Lexer::Token::KW_VOID:
        case Lexer::Token::LBRACKET:
            type.reset(parseType().release());
            atomicDeclList.reset(parseAtomicDeclList().release());
            semi_colon.reset(parseToken(Lexer::Token::SEMI_COLON).release());

            varDeclNode.reset(new VarDecl(std::move(type),
                                          std::move(atomicDeclList),
                                          std::move(semi_colon)));

            break;

        default:
            ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::EXPECTED_DECL, mLexer.GetCurrentLocation());
            break;
    }

    return varDeclNode;
}

std::unique_ptr<AtomicDeclList> Parser::parseAtomicDeclList() {
    auto atomicDeclListNode = std::make_unique<AtomicDeclList>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and mCurrentToken != Lexer::Token::IDENTIFIER and mCurrentToken != Lexer::Token::SEMI_COLON) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::MISSING_SEMI_COLON, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::IDENTIFIER) {
        std::unique_ptr<AtomicDecl> atomicDecl = parseAtomicDecl();
        std::unique_ptr<Token> comma = parseToken(Lexer::Token::COMMA);
        std::unique_ptr<AtomicDeclList> atomicDeclList = parseAtomicDeclList();

        atomicDeclListNode.reset(new AtomicDeclList(
            std::move(atomicDecl),
            std::move(comma),
            std::move(atomicDeclList)));
    } else if (mCurrentToken == Lexer::Token::SEMI_COLON) {
        // Do nothing
    }

    return atomicDeclListNode;
}

std::unique_ptr<AtomicDecl> Parser::parseAtomicDecl() {
    auto atomicDeclNode = std::make_unique<AtomicDecl>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and mCurrentToken != Lexer::Token::IDENTIFIER) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::IDENTIFIER) {
        std::unique_ptr<Token> identifier = parseToken(Lexer::Token::IDENTIFIER);
        std::unique_ptr<Token> assign = parseToken(Lexer::Token::ASSIGN);
        std::unique_ptr<Expression> expression = parseExpression();

        atomicDeclNode.reset(new AtomicDecl(
            std::move(identifier),
            std::move(assign),
            std::move(expression)));
    }

    return atomicDeclNode;
}

std::unique_ptr<FnDecl> Parser::parseFnDecl() {
    auto fnDeclNode = std::make_unique<FnDecl>();

    return fnDeclNode;
}

std::unique_ptr<ArgList> Parser::parseArgList() {
    auto argListNode = std::make_unique<ArgList>();

    return argListNode;
}

std::unique_ptr<Expression> Parser::parseExpression() {
    auto expressionNode = std::make_unique<Expression>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and
           mCurrentToken != Lexer::Token::LPAREN and
           (mCurrentToken < Lexer::Token::INT_LITERAL and mCurrentToken > Lexer::Token::STR_LITERAL) and
           (mCurrentToken < Lexer::Token::KW_TRUE and mCurrentToken > Lexer::Token::KW_FALSE) and
           mCurrentToken != Lexer::Token::IDENTIFIER) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    // TODO: If condition

    return expressionNode;
}

std::unique_ptr<Expression> Parser::parseExpressionRHS() {
}

std::unique_ptr<Term> Parser::parseTerm() {
    auto parent = std::make_unique<Term>();
    return parent;
}

std::unique_ptr<IdOrCallTerm> Parser::parseIdOrCallTerm() {
    auto parent = std::make_unique<IdOrCallTerm>();
    return parent;
}

std::unique_ptr<Call> Parser::parseCall() {
    auto parent = std::make_unique<Call>();
    return parent;
}

std::unique_ptr<FormalArgList> Parser::parseFormalArgList() {
    auto parent = std::make_unique<FormalArgList>();
    return parent;
}

std::unique_ptr<StmtList> Parser::parseStmtList() {
    auto parent = std::make_unique<StmtList>();
    return parent;
}

std::unique_ptr<Stmt> Parser::parseStmt() {
    auto parent = std::make_unique<Stmt>();
    return parent;
}

std::unique_ptr<AssignmentStmt> Parser::parseAssignmentStmt() {
    auto parent = std::make_unique<AssignmentStmt>();
    return parent;
}

std::unique_ptr<ConditionalStmt> Parser::parseConditionalStmt() {
    auto parent = std::make_unique<ConditionalStmt>();
    return parent;
}

std::unique_ptr<LoopStmt> Parser::parseLoopStmt() {
    auto parent = std::make_unique<LoopStmt>();
    return parent;
}

std::unique_ptr<ReturnStmt> Parser::parseReturnStmt() {
    auto parent = std::make_unique<ReturnStmt>();
    return parent;
}

std::unique_ptr<IfBlock> Parser::parseIfBlock() {
    auto parent = std::make_unique<IfBlock>();
    return parent;
}

std::unique_ptr<ElifBlocks> Parser::parseElifBlocks() {
    auto parent = std::make_unique<ElifBlocks>();
    return parent;
}

std::unique_ptr<ElifBlock> Parser::parseElifBlock() {
    auto parent = std::make_unique<ElifBlock>();
    return parent;
}

std::unique_ptr<ElseBlock> Parser::parseElseBlock() {
    auto parent = std::make_unique<ElseBlock>();
    return parent;
}

std::unique_ptr<ForLoop> Parser::parseForLoop() {
    auto parent = std::make_unique<ForLoop>();
    return parent;
}

std::unique_ptr<LoopRange> Parser::parseLoopRange() {
    auto parent = std::make_unique<LoopRange>();
    return parent;
}

std::unique_ptr<LoopStep> Parser::parseLoopStep() {
    auto parent = std::make_unique<LoopStep>();
    return parent;
}

std::unique_ptr<WhileLoop> Parser::parseWhileLoop() {
    auto parent = std::make_unique<WhileLoop>();
    return parent;
}

std::unique_ptr<ReturnVar> Parser::parseReturnVar() {
    auto parent = std::make_unique<ReturnVar>();
    return parent;
}

std::unique_ptr<Token> Parser::parseToken(Lexer::Token token) {
    auto parent = std::make_unique<Token>(token);
    mCurrentToken = mLexer.getNextToken();
    return parent;
}

std::unique_ptr<Type> Parser::parseType() {
    auto typeNode = std::make_unique<Type>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and (mCurrentToken < Lexer::Token::KW_INT_32 and mCurrentToken > Lexer::Token::KW_VOID) and (mCurrentToken != Lexer::Token::LBRACKET)) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_TYPE, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken >= Lexer::Token::KW_INT_32 and mCurrentToken <= Lexer::Token::KW_VOID) {
        std::unique_ptr<Token> atomic_type = parseToken(mCurrentToken);
        typeNode.reset(new Type(std::move(atomic_type)));
    } else if (mCurrentToken == Lexer::Token::LBRACKET) {
        std::unique_ptr<Token> lbracket = parseToken(Lexer::Token::LBRACKET);
        std::unique_ptr<Token> int_literal = parseToken(Lexer::Token::INT_LITERAL);
        std::unique_ptr<Token> rbracket = parseToken(Lexer::Token::LBRACKET);
        std::unique_ptr<Type> type = parseType();

        typeNode.reset(new Type(std::move(lbracket),
                                std::move(int_literal),
                                std::move(rbracket),
                                std::move(type)));
    }

    return typeNode;
}

}  // namespace Crust