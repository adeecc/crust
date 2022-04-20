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

    while (mCurrentToken != Lexer::Token::TOK_EOF and
           mCurrentToken != Lexer::Token::KW_FN) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::KW_FN) {
        std::unique_ptr<Token> kw_fn = parseToken(Lexer::Token::KW_FN);
        std::unique_ptr<Token> id = parseToken(Lexer::Token::IDENTIFIER);
        std::unique_ptr<Token> lparen = parseToken(Lexer::Token::LPAREN);
        std::unique_ptr<ArgList> argList = parseArgList();
        std::unique_ptr<Token> rparen = parseToken(Lexer::Token::RPAREN);
        std::unique_ptr<Type> type = parseType();
        std::unique_ptr<Token> lbrace = parseToken(Lexer::Token::LBRACE);
        std::unique_ptr<StmtList> stmtList = parseStmtList();
        std::unique_ptr<Token> rbrace = parseToken(Lexer::Token::RBRACE);

        fnDeclNode.reset(new FnDecl(
            std::move(kw_fn),
            std::move(id),
            std::move(lparen),
            std::move(argList),
            std::move(rparen),
            std::move(type),
            std::move(lbrace),
            std::move(stmtList),
            std::move(rbrace)));
    }

    return fnDeclNode;
}

std::unique_ptr<ArgList> Parser::parseArgList() {
    auto argListNode = std::make_unique<ArgList>();

    // TODO: add atomic_type condition in while and if

    while (mCurrentToken != Lexer::Token::TOK_EOF and
           (mCurrentToken < Lexer::Token::KW_INT_32 and mCurrentToken > Lexer::Token::KW_VOID) and
           mCurrentToken != Lexer::Token::LBRACKET and
           mCurrentToken != Lexer::Token::RPAREN) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::LBRACKET or
        (mCurrentToken >= Lexer::Token::KW_INT_32 and mCurrentToken <= Lexer::Token::KW_VOID)) {
        std::unique_ptr<Type> type = parseType();
        std::unique_ptr<Token> id = parseToken(Lexer::Token::IDENTIFIER);
        std::unique_ptr<Token> comma = parseToken(Lexer::Token::COMMA);
        std::unique_ptr<ArgList> argList = parseArgList();

        argListNode.reset(new ArgList(
            std::move(type),
            std::move(id),
            std::move(comma),
            std::move(argList)));
    } else if (mCurrentToken == Lexer::Token::RPAREN) {
        // epslion do nothing
    }

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
    if (mCurrentToken == Lexer::Token::LPAREN or
        (mCurrentToken >= Lexer::Token::INT_LITERAL and mCurrentToken <= Lexer::Token::STR_LITERAL) or
        (mCurrentToken >= Lexer::Token::KW_TRUE and mCurrentToken <= Lexer::Token::KW_FALSE) or
        mCurrentToken == Lexer::Token::IDENTIFIER) {
        std::unique_ptr<Term> term = parseTerm();
        std::unique_ptr<ExpressionRHS> expressionRHS = parseExpressionRHS();

        expressionNode.reset(new Expression(
            std::move(term),
            std::move(expressionRHS)));
    }

    return expressionNode;
}

std::unique_ptr<ExpressionRHS> Parser::parseExpressionRHS() {
    auto expressionRHSNode = std::unique_ptr<ExpressionRHS>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and
           (mCurrentToken < Lexer::Token::OP_PLUS and mCurrentToken > Lexer::Token::OP_LT) and
           mCurrentToken != Lexer::Token::SEMI_COLON and
           mCurrentToken != Lexer::Token::COMMA and
           mCurrentToken != Lexer::Token::RPAREN and
           mCurrentToken != Lexer::Token::LBRACE) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken >= Lexer::Token::OP_PLUS and mCurrentToken <= Lexer::Token::OP_LT) {
        std::unique_ptr<Token> bin_op = parseToken(mCurrentToken);
        std::unique_ptr<Expression> expression = parseExpression();

        expressionRHSNode.reset(new ExpressionRHS(
            std::move(bin_op),
            std::move(expression)));
    } else if (mCurrentToken == Lexer::Token::SEMI_COLON or
               mCurrentToken == Lexer::Token::COMMA or
               mCurrentToken == Lexer::Token::RPAREN or
               mCurrentToken == Lexer::Token::LBRACE) {
        // epsilon do nothing
    }

    return expressionRHSNode;
}

std::unique_ptr<Term> Parser::parseTerm() {
    auto termNode = std::make_unique<Term>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and
           mCurrentToken != Lexer::Token::LPAREN and
           (mCurrentToken < Lexer::Token::INT_LITERAL and mCurrentToken > Lexer::Token::STR_LITERAL) and
           (mCurrentToken < Lexer::Token::KW_TRUE and mCurrentToken > Lexer::Token::KW_FALSE) and
           mCurrentToken != Lexer::Token::IDENTIFIER) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::LPAREN) {
        std::unique_ptr<Token> lparen = parseToken(Lexer::Token::LPAREN);
        std::unique_ptr<Expression> expression = parseExpression();
        std::unique_ptr<Token> rparen = parseToken(Lexer::Token::RPAREN);

        termNode.reset(new Term(
            std::move(lparen),
            std::move(expression),
            std::move(rparen)));
    } else if (
        mCurrentToken == Lexer::Token::INT_LITERAL or
        mCurrentToken == Lexer::Token::FLOAT_LITERAL or
        mCurrentToken == Lexer::Token::STR_LITERAL or
        mCurrentToken == Lexer::Token::KW_TRUE or
        mCurrentToken == Lexer::Token::KW_FALSE) {
        std::unique_ptr<Token> literal = parseToken(mCurrentToken);

        termNode.reset(new Term(
            std::move(literal)));
    } else if (mCurrentToken == Lexer::Token::IDENTIFIER) {
        std::unique_ptr<IdOrCallTerm> idOrCallTerm = parseIdOrCallTerm();

        termNode.reset(new Term(
            std::move(idOrCallTerm)));
    }

    return termNode;
}

std::unique_ptr<IdOrCallTerm> Parser::parseIdOrCallTerm() {
    auto idOrCallTermNode = std::make_unique<IdOrCallTerm>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and
           mCurrentToken != Lexer::Token::IDENTIFIER) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::IDENTIFIER) {
        std::unique_ptr<Token> identifier = parseToken(Lexer::Token::IDENTIFIER);
        std::unique_ptr<Call> call = parseCall();

        idOrCallTermNode.reset(new IdOrCallTerm(
            std::move(identifier),
            std::move(call)));
    }

    return idOrCallTermNode;
}

std::unique_ptr<Call> Parser::parseCall() {
    auto callNode = std::make_unique<Call>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and
           mCurrentToken != Lexer::Token::LPAREN and
           (mCurrentToken < Lexer::Token::OP_PLUS and mCurrentToken > Lexer::Token::OP_LT) and
           mCurrentToken != Lexer::Token::COMMA and
           mCurrentToken != Lexer::Token::RPAREN and
           mCurrentToken != Lexer::Token::SEMI_COLON and mCurrentToken != Lexer::Token::LBRACE) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::LPAREN) {
        std::unique_ptr<Token> lparen = parseToken(Lexer::Token::LPAREN);
        std::unique_ptr<FormalArgList> formalArgList = parseFormalArgList();
        std::unique_ptr<Token> rparen = parseToken(Lexer::Token::RPAREN);

        callNode.reset(new Call(
            std::move(lparen),
            std::move(formalArgList),
            std::move(rparen)));
    } else if ((mCurrentToken >= Lexer::Token::OP_PLUS and mCurrentToken <= Lexer::Token::OP_LT) or
               mCurrentToken == Lexer::Token::COMMA or
               mCurrentToken == Lexer::Token::RPAREN or
               mCurrentToken == Lexer::Token::SEMI_COLON or
               mCurrentToken == Lexer::Token::LBRACE) {
        // epsilon do nothing
    }

    return callNode;
}

std::unique_ptr<FormalArgList> Parser::parseFormalArgList() {
    auto formalArgListNode = std::make_unique<FormalArgList>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and
           mCurrentToken != Lexer::Token::LPAREN and
           (mCurrentToken < Lexer::Token::INT_LITERAL and mCurrentToken > Lexer::Token::STR_LITERAL) and
           (mCurrentToken < Lexer::Token::KW_TRUE and mCurrentToken > Lexer::Token::KW_FALSE) and
           mCurrentToken != Lexer::Token::IDENTIFIER and
           mCurrentToken != Lexer::Token::RPAREN) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::LPAREN or
        (mCurrentToken >= Lexer::Token::INT_LITERAL and mCurrentToken <= Lexer::Token::STR_LITERAL) or
        (mCurrentToken >= Lexer::Token::KW_TRUE and mCurrentToken <= Lexer::Token::KW_FALSE) or
        mCurrentToken == Lexer::Token::IDENTIFIER) {
        std::unique_ptr<Expression> expression = parseExpression();
        std::unique_ptr<Token> comma = parseToken(Lexer::Token::COMMA);
        std::unique_ptr<FormalArgList> formalArgList = parseFormalArgList();

        formalArgListNode.reset(new FormalArgList(
            std::move(expression),
            std::move(comma),
            std::move(formalArgList)));
    } else if (mCurrentToken == Lexer::Token::RPAREN) {
        // epsilon do nothing
    }

    return formalArgListNode;
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
    auto assignmentStmtNode = std::make_unique<AssignmentStmt>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and
           mCurrentToken != Lexer::Token::IDENTIFIER) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::IDENTIFIER) {
        std::unique_ptr<Token> id = parseToken(Lexer::Token::IDENTIFIER);
        std::unique_ptr<Token> assign = parseToken(Lexer::Token::ASSIGN);
        std::unique_ptr<Expression> expression = parseExpression();

        assignmentStmtNode.reset(new AssignmentStmt(
            std::move(id),
            std::move(assign),
            std::move(expression)));
    }

    return assignmentStmtNode;
}

std::unique_ptr<ConditionalStmt> Parser::parseConditionalStmt() {
    auto conditionalStmtNode = std::make_unique<ConditionalStmt>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and
           mCurrentToken != Lexer::Token::KW_IF) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::KW_IF) {
        std::unique_ptr<IfBlock> ifBlock = parseIfBlock();
        std::unique_ptr<ElifBlocks> elifBlocks = parseElifBlocks();
        std::unique_ptr<ElseBlock> elseBlock = parseElseBlock();

        conditionalStmtNode.reset(new ConditionalStmt(
            std::move(ifBlock),
            std::move(elifBlocks),
            std::move(elseBlock)));
    }

    return conditionalStmtNode;
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
    auto ifBlockNode = std::make_unique<IfBlock>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and
           mCurrentToken != Lexer::Token::KW_IF) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::KW_IF) {
        std::unique_ptr<Token> kw_if = parseToken(Lexer::Token::KW_IF);
        std::unique_ptr<Expression> exp = parseExpression();
        std::unique_ptr<Token> lbrace = parseToken(Lexer::Token::LBRACE);
        std::unique_ptr<StmtList> stmtList = parseStmtList();
        std::unique_ptr<Token> rbrace = parseToken(Lexer::Token::RBRACE);

        ifBlockNode.reset(new IfBlock(
            std::move(kw_if),
            std::move(exp),
            std::move(lbrace),
            std::move(stmtList),
            std::move(rbrace)));
    }

    return ifBlockNode;
}

std::unique_ptr<ElifBlocks> Parser::parseElifBlocks() {
    auto elifBlocksNode = std::make_unique<ElifBlocks>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and
           mCurrentToken != Lexer::Token::KW_ELIF and
           mCurrentToken != Lexer::Token::KW_ELSE and
           mCurrentToken != Lexer::Token::SEMI_COLON) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::KW_ELIF) {
        std::unique_ptr<ElifBlock> elifBlock = parseElifBlock();
        std::unique_ptr<ElifBlocks> elifBlocks = parseElifBlocks();

        elifBlocksNode.reset(new ElifBlocks(
            std::move(elifBlock),
            std::move(elifBlocks)));
    } else if (mCurrentToken == Lexer::Token::KW_ELSE or
               mCurrentToken == Lexer::Token::SEMI_COLON) {
        // epsilon do nothing
    }

    return elifBlocksNode;
}

std::unique_ptr<ElifBlock> Parser::parseElifBlock() {
    auto elifBlockNode = std::make_unique<ElifBlock>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and
           mCurrentToken != Lexer::Token::KW_ELIF) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::KW_ELIF) {
        std::unique_ptr<Token> kw_elif = parseToken(Lexer::Token::KW_ELIF);
        std::unique_ptr<Expression> exp = parseExpression();
        std::unique_ptr<Token> lbrace = parseToken(Lexer::Token::LBRACE);
        std::unique_ptr<StmtList> stmtList = parseStmtList();
        std::unique_ptr<Token> rbrace = parseToken(Lexer::Token::RBRACE);

        elifBlockNode.reset(new ElifBlock(
            std::move(kw_elif),
            std::move(exp),
            std::move(lbrace),
            std::move(stmtList),
            std::move(rbrace)));
    }

    return elifBlockNode;
}

std::unique_ptr<ElseBlock> Parser::parseElseBlock() {
    auto elseBlockNode = std::make_unique<ElseBlock>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and
           mCurrentToken != Lexer::Token::KW_ELSE and
           mCurrentToken != Lexer::Token::SEMI_COLON) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::KW_ELSE) {
        std::unique_ptr<Token> kw_else = parseToken(Lexer::Token::KW_ELSE);
        std::unique_ptr<Token> lbrace = parseToken(Lexer::Token::LBRACE);
        std::unique_ptr<StmtList> stmtList = parseStmtList();
        std::unique_ptr<Token> rbrace = parseToken(Lexer::Token::RBRACE);

        elseBlockNode.reset(new ElseBlock(
            std::move(kw_else),
            std::move(lbrace),
            std::move(stmtList),
            std::move(rbrace)));
    } else if (mCurrentToken == Lexer::Token::SEMI_COLON) {
        // epsilon do nothing
    }

    return elseBlockNode;
}

std::unique_ptr<ForLoop> Parser::parseForLoop() {
    auto forLoopNode = std::make_unique<ForLoop>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and
           mCurrentToken != Lexer::Token::KW_FOR) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::KW_FOR) {
        std::unique_ptr<Token> kw_for = parseToken(Lexer::Token::KW_FOR);
        std::unique_ptr<Token> id = parseToken(Lexer::Token::IDENTIFIER);
        std::unique_ptr<Token> kw_in = parseToken(Lexer::Token::KW_IN);
        std::unique_ptr<LoopRange> loopRange = parseLoopRange();
        std::unique_ptr<Token> lbrace = parseToken(Lexer::Token::LBRACE);
        std::unique_ptr<StmtList> stmtList = parseStmtList();
        std::unique_ptr<Token> rbrace = parseToken(Lexer::Token::RBRACE);

        forLoopNode.reset(new ForLoop(
            std::move(kw_for),
            std::move(id),
            std::move(kw_in),
            std::move(loopRange),
            std::move(lbrace),
            std::move(stmtList),
            std::move(rbrace)));
    }

    return forLoopNode;
}

std::unique_ptr<LoopRange> Parser::parseLoopRange() {
    auto loopRangeNode = std::make_unique<LoopRange>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and
           mCurrentToken != Lexer::Token::INT_LITERAL) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::INT_LITERAL) {
        std::unique_ptr<Token> int_literal = parseToken(Lexer::Token::INT_LITERAL);
        std::unique_ptr<Token> range = parseToken(Lexer::Token::RANGE);
        std::unique_ptr<Token> int_literal_2 = parseToken(Lexer::Token::INT_LITERAL);
        std::unique_ptr<LoopStep> loopStep = parseLoopStep();

        loopRangeNode.reset(new LoopRange(
            std::move(int_literal),
            std::move(range),
            std::move(int_literal_2),
            std::move(loopStep)));
    }

    return loopRangeNode;
}

std::unique_ptr<LoopStep> Parser::parseLoopStep() {
    auto loopStepNode = std::make_unique<LoopStep>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and
           mCurrentToken != Lexer::Token::RANGE and
           mCurrentToken != Lexer::Token::LBRACE) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::RANGE) {
        std::unique_ptr<Token> range = parseToken(Lexer::Token::RANGE);
        std::unique_ptr<Token> int_literal = parseToken(Lexer::Token::INT_LITERAL);

        loopStepNode.reset(new LoopStep(
            std::move(range),
            std::move(int_literal)));
    }

    else if (mCurrentToken == Lexer::Token::LBRACE) {
        // do nothing
    }

    return loopStepNode;
}

std::unique_ptr<WhileLoop> Parser::parseWhileLoop() {
    auto whileLoopNode = std::make_unique<WhileLoop>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and
           mCurrentToken != Lexer::Token::KW_WHILE) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::KW_WHILE) {
        std::unique_ptr<Token> kw_while = parseToken(Lexer::Token::KW_WHILE);
        std::unique_ptr<Expression> exp = parseExpression();
        std::unique_ptr<Token> lbrace = parseToken(Lexer::Token::LBRACE);
        std::unique_ptr<StmtList> stmtList = parseStmtList();
        std::unique_ptr<Token> rbrace = parseToken(Lexer::Token::RBRACE);

        whileLoopNode.reset(new WhileLoop(
            std::move(kw_while),
            std::move(exp),
            std::move(lbrace),
            std::move(stmtList),
            std::move(rbrace)));
    }

    return whileLoopNode;
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