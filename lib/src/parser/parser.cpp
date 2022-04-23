#include <CFG/cfg.hpp>
#include <common/errorlogger.hpp>
#include <iostream>
#include <parser/parser.hpp>

namespace Crust {

void Parser::skipToNextSemiColon() {
    while (mCurrentToken != Lexer::Token::TOK_EOF and mCurrentToken != Lexer::Token::SEMI_COLON) {
        mCurrentToken = mLexer.getNextToken();
    }
}

Lexer::Token Parser::peekNextToken() {
    Lexer lexer_copy = mLexer;
    return lexer_copy.getNextToken();
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
    auto progDeclNode = std::make_unique<ProgDecl>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and progDeclNode->first.find(mCurrentToken) == progDeclNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::EXPECTED_DECL, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    std::unique_ptr<Crust::DeclList> declList = parseDeclList();
    progDeclNode.reset(new ProgDecl(std::move(declList)));

    return progDeclNode;
}

std::unique_ptr<DeclList> Parser::parseDeclList() {
    auto declListNode = std::make_unique<DeclList>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and declListNode->first.find(mCurrentToken) == declListNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::EXPECTED_DECL, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::TOK_EOF) {
    } else {
        std::unique_ptr<Crust::Decl> decl = parseDecl();
        std::unique_ptr<Crust::DeclList> declList = parseDeclList();
        declListNode.reset(new DeclList(std::move(decl), std::move(declList)));
    }

    return declListNode;
}

std::unique_ptr<Decl> Parser::parseDecl() {
    auto declNode = std::make_unique<Decl>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and declNode->first.find(mCurrentToken) == declNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::EXPECTED_DECL, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::KW_FN) {
        std::unique_ptr<Crust::FnDecl> fnDecl = parseFnDecl();
        declNode.reset(new Decl(std::move(fnDecl)));
    }

    else if (mCurrentToken == Lexer::Token::LBRACKET or (mCurrentToken >= Lexer::Token::KW_INT_32 and mCurrentToken <= Lexer::Token::KW_VOID)) {
        std::unique_ptr<Crust::VarDecl> varDecl = parseVarDecl();
        std::unique_ptr<Crust::Token> semi_colon = parseToken(Lexer::Token::SEMI_COLON);

        declNode.reset(
            new Decl(
                std::move(varDecl),
                std::move(semi_colon)));
    }

    return declNode;
}

std::unique_ptr<VarDecl> Parser::parseVarDecl() {
    auto varDeclNode = std::make_unique<VarDecl>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and varDeclNode->first.find(mCurrentToken) == varDeclNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::EXPECTED_DECL, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::LBRACKET or (mCurrentToken >= Lexer::Token::KW_INT_32 and mCurrentToken <= Lexer::Token::KW_VOID)) {
        std::unique_ptr<Crust::Type> type = parseType();
        std::unique_ptr<Crust::VarDeclList> varDeclList = parseVarDeclList();
        varDeclNode.reset(new VarDecl(std::move(type),
                                      std::move(varDeclList)));
    }

    return varDeclNode;
}

std::unique_ptr<VarDeclList> Parser::parseVarDeclList() {
    auto varDeclListNode = std::make_unique<VarDeclList>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and varDeclListNode->first.find(mCurrentToken) == varDeclListNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::MISSING_SEMI_COLON, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::IDENTIFIER) {
        std::unique_ptr<Token> identifier = parseToken(Lexer::Token::IDENTIFIER);
        std::unique_ptr<VarDeclList_> varDeclList_ = parseVarDeclList_();

        varDeclListNode.reset(new VarDeclList(
            std::move(identifier),
            std::move(varDeclList_)));
    }

    return varDeclListNode;
}

std::unique_ptr<VarDeclList_> Parser::parseVarDeclList_() {
    auto varDeclList_Node = std::make_unique<VarDeclList_>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and varDeclList_Node->first.find(mCurrentToken) == varDeclList_Node->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::COMMA) {
        std::unique_ptr<Token> comma = parseToken(Lexer::Token::COMMA);
        std::unique_ptr<VarDeclList> varDeclList = parseVarDeclList();

        varDeclList_Node.reset(new VarDeclList_(
            std::move(comma),
            std::move(varDeclList)));
    } else if (mCurrentToken == Lexer::Token::SEMI_COLON) {
    }

    return varDeclList_Node;
}

std::unique_ptr<FnDecl> Parser::parseFnDecl() {
    auto fnDeclNode = std::make_unique<FnDecl>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and fnDeclNode->first.find(mCurrentToken) == fnDeclNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::KW_FN) {
        std::unique_ptr<Token> kw_fn = parseToken(Lexer::Token::KW_FN);
        std::unique_ptr<Token> id = parseToken(Lexer::Token::IDENTIFIER);
        std::unique_ptr<Token> lparen = parseToken(Lexer::Token::LPAREN);
        std::unique_ptr<FnParamList> argList = parseFnParamList();
        std::unique_ptr<Token> rparen = parseToken(Lexer::Token::RPAREN);
        std::unique_ptr<Type> type = parseType();
        std::unique_ptr<Segment> segment = parseSegment();

        fnDeclNode.reset(new FnDecl(
            std::move(kw_fn),
            std::move(id),
            std::move(lparen),
            std::move(argList),
            std::move(rparen),
            std::move(type),
            std::move(segment)));
    }

    return fnDeclNode;
}

std::unique_ptr<FnParamList> Parser::parseFnParamList() {
    auto fnParamListNode = std::make_unique<FnParamList>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and fnParamListNode->first.find(mCurrentToken) == fnParamListNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::LBRACKET or
        (mCurrentToken >= Lexer::Token::KW_INT_32 and mCurrentToken <= Lexer::Token::KW_VOID)) {
        std::unique_ptr<FnParam> fnParam = parseFnParam();
        std::unique_ptr<FnParamList_> fnParamList_ = parseFnParamList_();

        fnParamListNode.reset(new FnParamList(
            std::move(fnParam),
            std::move(fnParamList_)));
    } else if (mCurrentToken == Lexer::Token::RPAREN) {
        // epslion do nothing
    }

    return fnParamListNode;
}

std::unique_ptr<FnParam> Parser::parseFnParam() {
    auto fnParamNode = std::make_unique<FnParam>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and fnParamNode->first.find(mCurrentToken) == fnParamNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::LBRACKET or
        (mCurrentToken >= Lexer::Token::KW_INT_32 and mCurrentToken <= Lexer::Token::KW_VOID)) {
        std::unique_ptr<Type> type = parseType();
        std::unique_ptr<Token> identifier = parseToken(Lexer::Token::IDENTIFIER);

        fnParamNode.reset(new FnParam(
            std::move(type),
            std::move(identifier)));
    }

    return fnParamNode;
}

std::unique_ptr<FnParamList_> Parser::parseFnParamList_() {
    auto fnParamList_Node = std::make_unique<FnParamList_>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and fnParamList_Node->first.find(mCurrentToken) == fnParamList_Node->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::COMMA) {
        std::unique_ptr<Token> comma = parseToken(Lexer::Token::COMMA);
        std::unique_ptr<FnParamList> fnParamList = parseFnParamList();

        fnParamList_Node.reset(new FnParamList_(
            std::move(comma),
            std::move(fnParamList)));
    } else if (mCurrentToken == Lexer::Token::RPAREN) {
        // epslion do nothing
    }

    return fnParamList_Node;
}

std::unique_ptr<Expression> Parser::parseExpression() {
    auto expressionNode = std::make_unique<Expression>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and expressionNode->first.find(mCurrentToken) == expressionNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::LPAREN or mCurrentToken == Lexer::Token::OP_MINUS or
        mCurrentToken == Lexer::Token::KW_TRUE or mCurrentToken == Lexer::Token::KW_FALSE or
        (mCurrentToken >= Lexer::Token::INT_LITERAL and mCurrentToken <= Lexer::Token::STR_LITERAL) or
        mCurrentToken == Lexer::Token::IDENTIFIER) {
        std::unique_ptr<Term> term = parseTerm();
        std::unique_ptr<ExpressionRHS> expressionRHS = parseExpressionRHS();

        expressionNode.reset(
            new Expression(
                std::move(term),
                std::move(expressionRHS)));
    }

    return expressionNode;
}

std::unique_ptr<ExpressionRHS> Parser::parseExpressionRHS() {
    auto expressionRHSNode = std::make_unique<ExpressionRHS>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and expressionRHSNode->first.find(mCurrentToken) == expressionRHSNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken >= Lexer::Token::OP_PLUS and mCurrentToken <= Lexer::Token::OP_LT) {
        std::unique_ptr<Token> bin_op = parseToken(mCurrentToken);
        std::unique_ptr<Expression> expression = parseExpression();

        expressionRHSNode.reset(new ExpressionRHS(
            std::move(bin_op),
            std::move(expression)));
    }

    else if (
        mCurrentToken == Lexer::Token::RPAREN or
        mCurrentToken == Lexer::Token::RBRACKET or
        mCurrentToken == Lexer::Token::COMMA or
        mCurrentToken == Lexer::Token::SEMI_COLON or
        mCurrentToken == Lexer::Token::LBRACE or
        mCurrentToken == Lexer::Token::RANGE) {
        // epsilon do nothing
    }

    return expressionRHSNode;
}

std::unique_ptr<Term> Parser::parseTerm() {
    auto termNode = std::make_unique<Term>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and termNode->first.find(mCurrentToken) == termNode->first.end()) {
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

    } else if (mCurrentToken == Lexer::Token::OP_MINUS) {
        std::unique_ptr<Token> op_minus = parseToken(Lexer::Token::OP_MINUS);
        std::unique_ptr<FloatTerm> floatTerm = parseFloatTerm();
        std::unique_ptr<Token> rparen = parseToken(Lexer::Token::RPAREN);

        termNode.reset(new Term(
            std::move(op_minus),
            std::move(floatTerm)));

    } else if (mCurrentToken == Lexer::Token::INT_LITERAL or mCurrentToken == Lexer::Token::FLOAT_LITERAL) {
        std::unique_ptr<FloatTerm> floatTerm = parseFloatTerm();
        termNode.reset(new Term(std::move(floatTerm)));

    } else if (mCurrentToken == Lexer::Token::KW_TRUE or mCurrentToken == Lexer::Token::KW_FALSE or mCurrentToken == Lexer::Token::STR_LITERAL) {
        std::unique_ptr<Token> literal = parseToken(mCurrentToken);
        termNode.reset(new Term(std::move(literal)));

    } else if (mCurrentToken == Lexer::Token::IDENTIFIER) {
        auto nextToken = peekNextToken();
        if (nextToken == Lexer::Token::LBRACKET) {
            std::unique_ptr<ArraySubscript> arraySubscript = parseArraySubscript();
            termNode.reset(new Term(std::move(arraySubscript)));

        } else if (nextToken == Lexer::Token::LPAREN) {
            std::unique_ptr<Call> call = parseCall();
            termNode.reset(new Term(std::move(call)));
        } else {
            std::unique_ptr<Token> identifier = parseToken(Lexer::Token::IDENTIFIER);

            termNode.reset(new Term(std::move(identifier)));
        }
    }

    return termNode;
}

std::unique_ptr<FloatTerm> Parser::parseFloatTerm() {
    auto floatTermNode = std::make_unique<FloatTerm>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and floatTermNode->first.find(mCurrentToken) == floatTermNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::INT_LITERAL) {
        std::unique_ptr<Token> int_literal = parseToken(Lexer::Token::INT_LITERAL);
        floatTermNode.reset(new FloatTerm(std::move(int_literal)));
    } else if (mCurrentToken == Lexer::Token::FLOAT_LITERAL) {
        std::unique_ptr<Token> float_literal = parseToken(Lexer::Token::FLOAT_LITERAL);
        floatTermNode.reset(new FloatTerm(std::move(float_literal)));
    }

    return floatTermNode;
}

std::unique_ptr<ArraySubscript> Parser::parseArraySubscript() {
    auto arraySubscriptNode = std::make_unique<ArraySubscript>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and arraySubscriptNode->first.find(mCurrentToken) == arraySubscriptNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::IDENTIFIER) {
        std::unique_ptr<Token> identifier = parseToken(Lexer::Token::IDENTIFIER);
        std::unique_ptr<Token> lbracket = parseToken(Lexer::Token::LBRACKET);
        std::unique_ptr<Expression> expression = parseExpression();
        std::unique_ptr<Token> rbracket = parseToken(Lexer::Token::RBRACKET);

        arraySubscriptNode.reset(new ArraySubscript(
            std::move(identifier),
            std::move(lbracket),
            std::move(expression),
            std::move(rbracket)));
    }

    return arraySubscriptNode;
}

std::unique_ptr<Call> Parser::parseCall() {
    auto callNode = std::make_unique<Call>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and callNode->first.find(mCurrentToken) == callNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::IDENTIFIER) {
        std::unique_ptr<Token> identifier = parseToken(Lexer::Token::IDENTIFIER);
        std::unique_ptr<Token> lparen = parseToken(Lexer::Token::LPAREN);
        std::unique_ptr<CallParamList> callParamList = parseCallParamList();
        std::unique_ptr<Token> rparen = parseToken(Lexer::Token::RPAREN);

        callNode.reset(new Call(
            std::move(identifier),
            std::move(lparen),
            std::move(callParamList),
            std::move(rparen)));
    }

    return callNode;
}

std::unique_ptr<CallParamList> Parser::parseCallParamList() {
    auto callParamListNode = std::make_unique<CallParamList>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and callParamListNode->first.find(mCurrentToken) == callParamListNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::LPAREN or mCurrentToken == Lexer::Token::OP_MINUS or
        (mCurrentToken >= Lexer::Token::INT_LITERAL and mCurrentToken <= Lexer::Token::STR_LITERAL) or
        mCurrentToken == Lexer::Token::KW_TRUE or mCurrentToken == Lexer::Token::KW_FALSE or
        mCurrentToken == Lexer::Token::IDENTIFIER) {
        std::unique_ptr<Expression> expression = parseExpression();
        std::unique_ptr<CallParamList_> callParamList_ = parseCallParamList_();

        callParamListNode.reset(new CallParamList(
            std::move(expression),
            std::move(callParamList_)));
    } else if (mCurrentToken == Lexer::Token::RPAREN) {
        // epsilon do nothing
    }

    return callParamListNode;
}

std::unique_ptr<CallParamList_> Parser::parseCallParamList_() {
    auto callParamList_Node = std::make_unique<CallParamList_>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and callParamList_Node->first.find(mCurrentToken) == callParamList_Node->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::COMMA) {
        std::unique_ptr<Token> comma = parseToken(Lexer::Token::COMMA);
        std::unique_ptr<CallParamList> callParamList = parseCallParamList();

        callParamList_Node.reset(new CallParamList_(
            std::move(comma),
            std::move(callParamList)));
    } else if (mCurrentToken == Lexer::Token::RPAREN) {
        // epsilon do nothing
    }

    return callParamList_Node;
}

std::unique_ptr<StmtList> Parser::parseStmtList() {
    auto stmtListNode = std::make_unique<StmtList>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and stmtListNode->first.find(mCurrentToken) == stmtListNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::LBRACE or
        (mCurrentToken >= Lexer::Token::KW_INT_32 and mCurrentToken <= Lexer::Token::KW_VOID) or
        mCurrentToken == Lexer::Token::LBRACKET or
        mCurrentToken == Lexer::Token::KW_RETURN or
        mCurrentToken == Lexer::Token::IDENTIFIER or
        mCurrentToken == Lexer::Token::LPAREN or
        mCurrentToken == Lexer::Token::OP_MINUS or
        mCurrentToken == Lexer::Token::INT_LITERAL or
        mCurrentToken == Lexer::Token::FLOAT_LITERAL or
        mCurrentToken == Lexer::Token::STR_LITERAL or
        mCurrentToken == Lexer::Token::KW_TRUE or
        mCurrentToken == Lexer::Token::KW_FALSE or
        mCurrentToken == Lexer::Token::KW_IF or
        mCurrentToken == Lexer::Token::KW_FOR or
        mCurrentToken == Lexer::Token::KW_WHILE) {
        std::unique_ptr<Stmt> stmt = parseStmt();
        std::unique_ptr<StmtList> stmtList = parseStmtList();

        stmtListNode.reset(new StmtList(
            std::move(stmt),
            std::move(stmtList)));
    }

    else if (mCurrentToken == Lexer::Token::RBRACE) {
        // do nothing
    }

    return stmtListNode;
}

std::unique_ptr<Stmt> Parser::parseStmt() {
    auto stmtNode = std::make_unique<Stmt>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and stmtNode->first.find(mCurrentToken) == stmtNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::LBRACE) {
        std::unique_ptr<Segment> segment = parseSegment();

        stmtNode.reset(new Stmt(std::move(segment)));
    }

    else if (mCurrentToken == Lexer::Token::KW_IF) {
        std::unique_ptr<ConditionalStmt> conditionalStmt = parseConditionalStmt();

        stmtNode.reset(new Stmt(
            std::move(conditionalStmt)));
    }

    else if (mCurrentToken == Lexer::Token::KW_FOR or mCurrentToken == Lexer::Token::KW_WHILE) {
        std::unique_ptr<LoopStmt> loopStmt = parseLoopStmt();
        stmtNode.reset(new Stmt(
            std::move(loopStmt)));
    }

    else if ((mCurrentToken >= Lexer::Token::KW_INT_32 and mCurrentToken <= Lexer::Token::KW_VOID) or
             mCurrentToken == Lexer::Token::LBRACKET) {
        std::unique_ptr<VarDecl> vardecl = parseVarDecl();
        std::unique_ptr<Token> semi_colon = parseToken(Lexer::Token::SEMI_COLON);

        stmtNode.reset(new Stmt(
            std::move(vardecl),
            std::move(semi_colon)));
    }

    else if (mCurrentToken == Lexer::Token::KW_RETURN) {
        std::unique_ptr<ReturnStmt> returnStmt = parseReturnStmt();
        std::unique_ptr<Token> semi_colon = parseToken(Lexer::Token::SEMI_COLON);

        stmtNode.reset(new Stmt(
            std::move(returnStmt), std::move(semi_colon)));
    }

    else if (mCurrentToken == Lexer::Token::IDENTIFIER) {
        if (peekNextToken() == Lexer::Token::ASSIGN) {
            std::unique_ptr<AssignmentStmt> assignment_stmt = parseAssignmentStmt();
            std::unique_ptr<Token> semi_colon = parseToken(Lexer::Token::SEMI_COLON);

            stmtNode.reset(new Stmt(std::move(assignment_stmt), std::move(semi_colon)));
        } else {
            std::unique_ptr<Expression> expression = parseExpression();
            std::unique_ptr<Token> semi_colon = parseToken(Lexer::Token::SEMI_COLON);

            stmtNode.reset(new Stmt(std::move(expression), std::move(semi_colon)));
        }
    }

    else if (mCurrentToken == Lexer::Token::LPAREN or
             mCurrentToken == Lexer::Token::OP_MINUS or
             mCurrentToken == Lexer::Token::INT_LITERAL or
             mCurrentToken == Lexer::Token::FLOAT_LITERAL or
             mCurrentToken == Lexer::Token::STR_LITERAL or
             mCurrentToken == Lexer::Token::KW_TRUE or
             mCurrentToken == Lexer::Token::KW_FALSE) {
        std::unique_ptr<Expression> exp = parseExpression();
        std::unique_ptr<Token> semi_colon = parseToken(Lexer::Token::SEMI_COLON);

        stmtNode.reset(new Stmt(
            std::move(exp), std::move(semi_colon)));
    }

    return stmtNode;
}

std::unique_ptr<AssignmentStmt> Parser::parseAssignmentStmt() {
    auto assignmentStmtNode = std::make_unique<AssignmentStmt>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and assignmentStmtNode->first.find(mCurrentToken) == assignmentStmtNode->first.end()) {
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

    while (mCurrentToken != Lexer::Token::TOK_EOF and conditionalStmtNode->first.find(mCurrentToken) == conditionalStmtNode->first.end()) {
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
    auto loopStmtNode = std::make_unique<LoopStmt>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and loopStmtNode->first.find(mCurrentToken) == loopStmtNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::KW_FOR) {
        std::unique_ptr<ForLoop> forLoop = parseForLoop();
        loopStmtNode.reset(new LoopStmt(std::move(forLoop)));
    }

    else if (mCurrentToken == Lexer::Token::KW_WHILE) {
        std::unique_ptr<WhileLoop> whileLoop = parseWhileLoop();
        loopStmtNode.reset(new LoopStmt(std::move(whileLoop)));
    }

    return loopStmtNode;
}

std::unique_ptr<ReturnStmt> Parser::parseReturnStmt() {
    auto returnStmtNode = std::make_unique<ReturnStmt>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and returnStmtNode->first.find(mCurrentToken) == returnStmtNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::KW_RETURN) {
        std::unique_ptr<Token> kw_return = parseToken(Lexer::Token::KW_RETURN);
        std::unique_ptr<ReturnVar> returnVar = parseReturnVar();

        returnStmtNode.reset(new ReturnStmt(
            std::move(kw_return),
            std::move(returnVar)));
    }

    return returnStmtNode;
}

std::unique_ptr<IfBlock> Parser::parseIfBlock() {
    auto ifBlockNode = std::make_unique<IfBlock>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and ifBlockNode->first.find(mCurrentToken) == ifBlockNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::KW_IF) {
        std::unique_ptr<Token> kw_if = parseToken(Lexer::Token::KW_IF);
        std::unique_ptr<Expression> exp = parseExpression();
        std::unique_ptr<Segment> segment = parseSegment();

        ifBlockNode.reset(new IfBlock(
            std::move(kw_if),
            std::move(exp),
            std::move(segment)));
    }

    return ifBlockNode;
}

std::unique_ptr<ElifBlocks> Parser::parseElifBlocks() {
    auto elifBlocksNode = std::make_unique<ElifBlocks>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and elifBlocksNode->first.find(mCurrentToken) == elifBlocksNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::KW_ELIF) {
        std::unique_ptr<ElifBlock> elifBlock = parseElifBlock();
        std::unique_ptr<ElifBlocks> elifBlocks = parseElifBlocks();

        elifBlocksNode.reset(new ElifBlocks(
            std::move(elifBlock),
            std::move(elifBlocks)));
    } else {
        // epsilon do nothing
    }

    return elifBlocksNode;
}

std::unique_ptr<ElifBlock> Parser::parseElifBlock() {
    auto elifBlockNode = std::make_unique<ElifBlock>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and elifBlockNode->first.find(mCurrentToken) == elifBlockNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::KW_ELIF) {
        std::unique_ptr<Token> kw_elif = parseToken(Lexer::Token::KW_ELIF);
        std::unique_ptr<Expression> exp = parseExpression();
        std::unique_ptr<Segment> segment = parseSegment();

        elifBlockNode.reset(new ElifBlock(
            std::move(kw_elif),
            std::move(exp),
            std::move(segment)));
    }

    return elifBlockNode;
}

std::unique_ptr<ElseBlock> Parser::parseElseBlock() {
    auto elseBlockNode = std::make_unique<ElseBlock>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and elseBlockNode->first.find(mCurrentToken) == elseBlockNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::KW_ELSE) {
        std::unique_ptr<Token> kw_else = parseToken(Lexer::Token::KW_ELSE);
        std::unique_ptr<Segment> segment = parseSegment();

        elseBlockNode.reset(new ElseBlock(
            std::move(kw_else),
            std::move(segment)));
    } else {
        // epsilon do nothing
    }

    return elseBlockNode;
}

std::unique_ptr<ForLoop> Parser::parseForLoop() {
    auto forLoopNode = std::make_unique<ForLoop>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and forLoopNode->first.find(mCurrentToken) == forLoopNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::KW_FOR) {
        std::unique_ptr<Token> kw_for = parseToken(Lexer::Token::KW_FOR);
        std::unique_ptr<Token> id = parseToken(Lexer::Token::IDENTIFIER);
        std::unique_ptr<Token> kw_in = parseToken(Lexer::Token::KW_IN);
        std::unique_ptr<LoopRange> loopRange = parseLoopRange();
        std::unique_ptr<Segment> segment = parseSegment();

        forLoopNode.reset(new ForLoop(
            std::move(kw_for),
            std::move(id),
            std::move(kw_in),
            std::move(loopRange),
            std::move(segment)));
    }

    return forLoopNode;
}

std::unique_ptr<LoopRange> Parser::parseLoopRange() {
    auto loopRangeNode = std::make_unique<LoopRange>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and loopRangeNode->first.find(mCurrentToken) == loopRangeNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::LPAREN or
        mCurrentToken == Lexer::Token::OP_MINUS or
        mCurrentToken == Lexer::Token::KW_TRUE or mCurrentToken == Lexer::Token::KW_FALSE or
        mCurrentToken == Lexer::Token::INT_LITERAL or
        mCurrentToken == Lexer::Token::FLOAT_LITERAL or
        mCurrentToken == Lexer::Token::IDENTIFIER) {
        std::unique_ptr<Expression> start_expression = parseExpression();
        std::unique_ptr<Token> range = parseToken(Lexer::Token::RANGE);
        std::unique_ptr<Expression> end_expression = parseExpression();
        std::unique_ptr<LoopStep> loopStep = parseLoopStep();

        loopRangeNode.reset(new LoopRange(
            std::move(start_expression),
            std::move(range),
            std::move(end_expression),
            std::move(loopStep)));
    }

    return loopRangeNode;
}

std::unique_ptr<LoopStep> Parser::parseLoopStep() {
    auto loopStepNode = std::make_unique<LoopStep>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and loopStepNode->first.find(mCurrentToken) == loopStepNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::RANGE) {
        std::unique_ptr<Token> range = parseToken(Lexer::Token::RANGE);
        std::unique_ptr<Expression> expression = parseExpression();

        loopStepNode.reset(new LoopStep(
            std::move(range),
            std::move(expression)));
    }

    else if (mCurrentToken == Lexer::Token::LBRACE) {
        // do nothing
    }

    return loopStepNode;
}

std::unique_ptr<WhileLoop> Parser::parseWhileLoop() {
    auto whileLoopNode = std::make_unique<WhileLoop>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and whileLoopNode->first.find(mCurrentToken) == whileLoopNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::KW_WHILE) {
        std::unique_ptr<Token> kw_while = parseToken(Lexer::Token::KW_WHILE);
        std::unique_ptr<Expression> exp = parseExpression();
        std::unique_ptr<Segment> segment = parseSegment();

        whileLoopNode.reset(new WhileLoop(
            std::move(kw_while),
            std::move(exp),
            std::move(segment)));
    }

    return whileLoopNode;
}

std::unique_ptr<ReturnVar> Parser::parseReturnVar() {
    auto returnVarNode = std::make_unique<ReturnVar>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and returnVarNode->first.find(mCurrentToken) == returnVarNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::LPAREN or
        mCurrentToken == Lexer::Token::OP_MINUS or
        mCurrentToken == Lexer::Token::INT_LITERAL or
        mCurrentToken == Lexer::Token::FLOAT_LITERAL or
        mCurrentToken == Lexer::Token::STR_LITERAL or
        mCurrentToken == Lexer::Token::KW_TRUE or
        mCurrentToken == Lexer::Token::KW_FALSE or
        mCurrentToken == Lexer::Token::IDENTIFIER) {
        std::unique_ptr<Expression> exp = parseExpression();

        returnVarNode.reset(new ReturnVar(
            std::move(exp)));
    }

    else if (mCurrentToken == Lexer::Token::SEMI_COLON) {
        // do nothing
    }

    return returnVarNode;
}

std::unique_ptr<Segment> Parser::parseSegment() {
    auto segmentNode = std::make_unique<Segment>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and segmentNode->first.find(mCurrentToken) == segmentNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::LBRACE) {
        std::unique_ptr<Token> lbrace = parseToken(Lexer::Token::LBRACE);
        std::unique_ptr<StmtList> stmtList = parseStmtList();
        std::unique_ptr<Token> rbrace = parseToken(Lexer::Token::RBRACE);

        segmentNode.reset(new Segment(
            std::move(lbrace),
            std::move(stmtList),
            std::move(rbrace)));
    }

    return segmentNode;
}

std::unique_ptr<Token> Parser::parseToken(Lexer::Token token) {
    std::unique_ptr<Crust::Token> parent;

    if (mCurrentToken != token) {
        std::cout << "Unknown Error\n";  // FIXME: Use ErrorLogger
    }

    else if (token == Lexer::Token::IDENTIFIER or token == Lexer::Token::STR_LITERAL) {
        parent = std::make_unique<Token>(token, mLexer.getCurrentStr());
    }

    else if (token == Lexer::Token::INT_LITERAL) {
        parent = std::make_unique<Token>(token, mLexer.getCurrentInt());
    }

    else if (token == Lexer::Token::FLOAT_LITERAL) {
        parent = std::make_unique<Token>(token, mLexer.getCurrentFloat());
    }

    else {
        parent = std::make_unique<Token>(token);
    }

    mCurrentToken = mLexer.getNextToken();
    return parent;
}

std::unique_ptr<Type> Parser::parseType() {
    auto typeNode = std::make_unique<Type>();

    while (mCurrentToken != Lexer::Token::TOK_EOF and typeNode->first.find(mCurrentToken) == typeNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken >= Lexer::Token::KW_INT_32 and mCurrentToken <= Lexer::Token::KW_VOID) {
        std::unique_ptr<Token> atomic_type = parseToken(mCurrentToken);
        typeNode.reset(new Type(std::move(atomic_type)));
    } else if (mCurrentToken == Lexer::Token::LBRACKET) {
        std::unique_ptr<Token> lbracket = parseToken(Lexer::Token::LBRACKET);
        std::unique_ptr<Token> int_literal = parseToken(Lexer::Token::INT_LITERAL);
        std::unique_ptr<Token> rbracket = parseToken(Lexer::Token::RBRACKET);
        std::unique_ptr<Type> type = parseType();

        typeNode.reset(new Type(std::move(lbracket),
                                std::move(int_literal),
                                std::move(rbracket),
                                std::move(type)));
    }

    return typeNode;
}

}  // namespace Crust