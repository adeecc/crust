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

CFGNode* Parser::parseProgram(const std::string& filename) {
    //    Check the extension?
    if (!mLexer.init(filename)) {
        ErrorLogger::printError(ErrorLogger::ErrorType::ERROR_OPENING_FILE);
        return nullptr;
    }

    mCurrentToken = mLexer.getNextToken();
    return parseProgramDecl();
}

ProgDecl* Parser::parseProgramDecl() {
    auto progDeclNode = new ProgDecl();

    while (mCurrentToken != Lexer::Token::TOK_EOF and progDeclNode->first.find(mCurrentToken) == progDeclNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::EXPECTED_DECL, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    Crust::DeclList* declList = parseDeclList();
    delete progDeclNode;
    progDeclNode = new ProgDecl(declList);

    return progDeclNode;
}

DeclList* Parser::parseDeclList() {
    auto declListNode = new DeclList();

    while (mCurrentToken != Lexer::Token::TOK_EOF and declListNode->first.find(mCurrentToken) == declListNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::EXPECTED_DECL, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::TOK_EOF) {
    } else {
        Crust::Decl* decl = parseDecl();
        Crust::DeclList* declList = parseDeclList();
        delete declListNode;
        declListNode = new DeclList(decl, declList);
    }

    return declListNode;
}

Decl* Parser::parseDecl() {
    auto declNode = new Decl();

    while (mCurrentToken != Lexer::Token::TOK_EOF and declNode->first.find(mCurrentToken) == declNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::EXPECTED_DECL, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::KW_FN) {
        Crust::FnDecl* fnDecl = parseFnDecl();
        delete declNode;
        declNode = new Decl(fnDecl);
    }

    else if (mCurrentToken == Lexer::Token::LBRACKET or (mCurrentToken >= Lexer::Token::KW_INT_32 and mCurrentToken <= Lexer::Token::KW_VOID)) {
        Crust::VarDecl* varDecl = parseVarDecl();
        Crust::Token* semi_colon = parseToken(Lexer::Token::SEMI_COLON);

        delete declNode;
        declNode = new Decl(varDecl, semi_colon);
    }

    return declNode;
}

VarDecl* Parser::parseVarDecl() {
    auto varDeclNode = new VarDecl();

    while (mCurrentToken != Lexer::Token::TOK_EOF and varDeclNode->first.find(mCurrentToken) == varDeclNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::EXPECTED_DECL, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::LBRACKET or (mCurrentToken >= Lexer::Token::KW_INT_32 and mCurrentToken <= Lexer::Token::KW_VOID)) {
        Crust::Type* type = parseType();
        Crust::VarDeclList* varDeclList = parseVarDeclList();
        delete varDeclNode;
        varDeclNode = new VarDecl(type, varDeclList);
    }

    return varDeclNode;
}

VarDeclList* Parser::parseVarDeclList() {
    auto varDeclListNode = new VarDeclList();

    while (mCurrentToken != Lexer::Token::TOK_EOF and varDeclListNode->first.find(mCurrentToken) == varDeclListNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::MISSING_SEMI_COLON, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::IDENTIFIER) {
        Token* identifier = parseToken(Lexer::Token::IDENTIFIER);
        VarDeclList_* varDeclList_ = parseVarDeclList_();

        delete varDeclListNode;
        varDeclListNode = new VarDeclList(identifier, varDeclList_);
    }

    return varDeclListNode;
}

VarDeclList_* Parser::parseVarDeclList_() {
    auto varDeclList_Node = new VarDeclList_();

    while (mCurrentToken != Lexer::Token::TOK_EOF and varDeclList_Node->first.find(mCurrentToken) == varDeclList_Node->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::COMMA) {
        Token* comma = parseToken(Lexer::Token::COMMA);
        VarDeclList* varDeclList = parseVarDeclList();

        delete varDeclList_Node;
        varDeclList_Node = new VarDeclList_(comma, varDeclList);
    } else if (mCurrentToken == Lexer::Token::SEMI_COLON) {
    }

    return varDeclList_Node;
}

FnDecl* Parser::parseFnDecl() {
    auto fnDeclNode = new FnDecl();

    while (mCurrentToken != Lexer::Token::TOK_EOF and fnDeclNode->first.find(mCurrentToken) == fnDeclNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::KW_FN) {
        Token* kw_fn = parseToken(Lexer::Token::KW_FN);
        Token* id = parseToken(Lexer::Token::IDENTIFIER);
        Token* lparen = parseToken(Lexer::Token::LPAREN);
        FnParamList* argList = parseFnParamList();
        Token* rparen = parseToken(Lexer::Token::RPAREN);
        Type* type = parseType();
        Segment* segment = parseSegment();

        delete fnDeclNode;
        fnDeclNode = new FnDecl(kw_fn, id, lparen, argList, rparen, type, segment);
    }

    return fnDeclNode;
}

FnParamList* Parser::parseFnParamList() {
    auto fnParamListNode = new FnParamList();

    while (mCurrentToken != Lexer::Token::TOK_EOF and fnParamListNode->first.find(mCurrentToken) == fnParamListNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::LBRACKET or
        (mCurrentToken >= Lexer::Token::KW_INT_32 and mCurrentToken <= Lexer::Token::KW_VOID)) {
        FnParam* fnParam = parseFnParam();
        FnParamList_* fnParamList_ = parseFnParamList_();

        delete fnParamListNode;
        fnParamListNode = new FnParamList(fnParam, fnParamList_);
    } else if (mCurrentToken == Lexer::Token::RPAREN) {
        // epslion do nothing
    }

    return fnParamListNode;
}

FnParam* Parser::parseFnParam() {
    auto fnParamNode = new FnParam();

    while (mCurrentToken != Lexer::Token::TOK_EOF and fnParamNode->first.find(mCurrentToken) == fnParamNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::LBRACKET or
        (mCurrentToken >= Lexer::Token::KW_INT_32 and mCurrentToken <= Lexer::Token::KW_VOID)) {
        Type* type = parseType();
        Token* identifier = parseToken(Lexer::Token::IDENTIFIER);

        delete fnParamNode;
        fnParamNode = new FnParam(type, identifier);
    }

    return fnParamNode;
}

FnParamList_* Parser::parseFnParamList_() {
    auto fnParamList_Node = new FnParamList_();

    while (mCurrentToken != Lexer::Token::TOK_EOF and fnParamList_Node->first.find(mCurrentToken) == fnParamList_Node->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::COMMA) {
        Token* comma = parseToken(Lexer::Token::COMMA);
        FnParamList* fnParamList = parseFnParamList();

        delete fnParamList_Node;
        fnParamList_Node = new FnParamList_(comma, fnParamList);
    } else if (mCurrentToken == Lexer::Token::RPAREN) {
        // epslion do nothing
    }

    return fnParamList_Node;
}

Expression* Parser::parseExpression() {
    auto expressionNode = new Expression();

    while (mCurrentToken != Lexer::Token::TOK_EOF and expressionNode->first.find(mCurrentToken) == expressionNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken != Lexer::Token::TOK_EOF) {
        auto next = peekNextToken();

        if (next >= Lexer::Token::OP_PLUS and next <= Lexer::Token::OP_LT) {
            Term* term = parseTerm();
            Token* bin_op = parseToken(mCurrentToken);
            Expression* expression = parseExpression();

            delete expressionNode;
            expressionNode = new Expression(term, bin_op, expression);
        } else {
            Term* term = parseTerm();

            delete expressionNode;
            expressionNode = new Expression(term);
        }
    }

    return expressionNode;
}

Term* Parser::parseTerm() {
    auto termNode = new Term();

    while (mCurrentToken != Lexer::Token::TOK_EOF and termNode->first.find(mCurrentToken) == termNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::LPAREN) {
        Token* lparen = parseToken(Lexer::Token::LPAREN);
        Expression* expression = parseExpression();
        Token* rparen = parseToken(Lexer::Token::RPAREN);

        delete termNode;
        termNode = new Term(lparen, expression, rparen);

    } else if (mCurrentToken == Lexer::Token::OP_MINUS) {
        Token* op_minus = parseToken(Lexer::Token::OP_MINUS);
        FloatTerm* floatTerm = parseFloatTerm();

        delete termNode;
        termNode = new Term(op_minus, floatTerm);

    } else if (mCurrentToken == Lexer::Token::INT_LITERAL or mCurrentToken == Lexer::Token::FLOAT_LITERAL) {
        FloatTerm* floatTerm = parseFloatTerm();
        delete termNode;
        termNode = new Term(floatTerm);

    } else if (mCurrentToken == Lexer::Token::KW_TRUE or mCurrentToken == Lexer::Token::KW_FALSE or mCurrentToken == Lexer::Token::STR_LITERAL) {
        Token* literal = parseToken(mCurrentToken);
        delete termNode;
        termNode = new Term(literal);

    } else if (mCurrentToken == Lexer::Token::IDENTIFIER) {
        auto nextToken = peekNextToken();
        if (nextToken == Lexer::Token::LBRACKET) {
            ArraySubscript* arraySubscript = parseArraySubscript();
            delete termNode;
            termNode = new Term(arraySubscript);

        } else if (nextToken == Lexer::Token::LPAREN) {
            Call* call = parseCall();
            delete termNode;
            termNode = new Term(call);
        } else {
            Token* identifier = parseToken(Lexer::Token::IDENTIFIER);

            delete termNode;
            termNode = new Term(identifier);
        }
    }

    return termNode;
}

FloatTerm* Parser::parseFloatTerm() {
    auto floatTermNode = new FloatTerm();

    while (mCurrentToken != Lexer::Token::TOK_EOF and floatTermNode->first.find(mCurrentToken) == floatTermNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::INT_LITERAL) {
        Token* int_literal = parseToken(Lexer::Token::INT_LITERAL);
        delete floatTermNode;
        floatTermNode = new FloatTerm(int_literal);
    } else if (mCurrentToken == Lexer::Token::FLOAT_LITERAL) {
        Token* float_literal = parseToken(Lexer::Token::FLOAT_LITERAL);
        delete floatTermNode;
        floatTermNode = new FloatTerm(float_literal);
    }

    return floatTermNode;
}

ArraySubscript* Parser::parseArraySubscript() {
    auto arraySubscriptNode = new ArraySubscript();

    while (mCurrentToken != Lexer::Token::TOK_EOF and arraySubscriptNode->first.find(mCurrentToken) == arraySubscriptNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::IDENTIFIER) {
        Token* identifier = parseToken(Lexer::Token::IDENTIFIER);
        Token* lbracket = parseToken(Lexer::Token::LBRACKET);
        Expression* expression = parseExpression();
        Token* rbracket = parseToken(Lexer::Token::RBRACKET);

        delete arraySubscriptNode;
        arraySubscriptNode = new ArraySubscript(identifier, lbracket, expression, rbracket);
    }

    return arraySubscriptNode;
}

Call* Parser::parseCall() {
    auto callNode = new Call();

    while (mCurrentToken != Lexer::Token::TOK_EOF and callNode->first.find(mCurrentToken) == callNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::IDENTIFIER) {
        Token* identifier = parseToken(Lexer::Token::IDENTIFIER);
        Token* lparen = parseToken(Lexer::Token::LPAREN);
        CallParamList* callParamList = parseCallParamList();
        Token* rparen = parseToken(Lexer::Token::RPAREN);

        delete callNode;
        callNode = new Call(identifier, lparen, callParamList, rparen);
    }

    return callNode;
}

CallParamList* Parser::parseCallParamList() {
    auto callParamListNode = new CallParamList();

    while (mCurrentToken != Lexer::Token::TOK_EOF and callParamListNode->first.find(mCurrentToken) == callParamListNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::LPAREN or mCurrentToken == Lexer::Token::OP_MINUS or
        (mCurrentToken >= Lexer::Token::INT_LITERAL and mCurrentToken <= Lexer::Token::STR_LITERAL) or
        mCurrentToken == Lexer::Token::KW_TRUE or mCurrentToken == Lexer::Token::KW_FALSE or
        mCurrentToken == Lexer::Token::IDENTIFIER) {
        Expression* expression = parseExpression();
        CallParamList_* callParamList_ = parseCallParamList_();

        delete callParamListNode;
        callParamListNode = new CallParamList(expression, callParamList_);
    } else if (mCurrentToken == Lexer::Token::RPAREN) {
        // epsilon do nothing
    }

    return callParamListNode;
}

CallParamList_* Parser::parseCallParamList_() {
    auto callParamList_Node = new CallParamList_();

    while (mCurrentToken != Lexer::Token::TOK_EOF and callParamList_Node->first.find(mCurrentToken) == callParamList_Node->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::COMMA) {
        Token* comma = parseToken(Lexer::Token::COMMA);
        CallParamList* callParamList = parseCallParamList();

        delete callParamList_Node;
        callParamList_Node = new CallParamList_(comma, callParamList);
    } else if (mCurrentToken == Lexer::Token::RPAREN) {
        // epsilon do nothing
    }

    return callParamList_Node;
}

StmtList* Parser::parseStmtList() {
    auto stmtListNode = new StmtList();

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
        Stmt* stmt = parseStmt();
        StmtList* stmtList = parseStmtList();

        delete stmtListNode;
        stmtListNode = new StmtList(stmt, stmtList);
    }

    else if (mCurrentToken == Lexer::Token::RBRACE) {
        // do nothing
    }

    return stmtListNode;
}

Stmt* Parser::parseStmt() {
    auto stmtNode = new Stmt();

    while (mCurrentToken != Lexer::Token::TOK_EOF and stmtNode->first.find(mCurrentToken) == stmtNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::LBRACE) {
        Segment* segment = parseSegment();

        delete stmtNode;
        stmtNode = new Stmt(segment);
    }

    else if (mCurrentToken == Lexer::Token::KW_IF) {
        ConditionalStmt* conditionalStmt = parseConditionalStmt();

        delete stmtNode;
        stmtNode = new Stmt(conditionalStmt);
    }

    else if (mCurrentToken == Lexer::Token::KW_FOR or mCurrentToken == Lexer::Token::KW_WHILE) {
        LoopStmt* loopStmt = parseLoopStmt();
        delete stmtNode;
        stmtNode = new Stmt(loopStmt);
    }

    else if ((mCurrentToken >= Lexer::Token::KW_INT_32 and mCurrentToken <= Lexer::Token::KW_VOID) or
             mCurrentToken == Lexer::Token::LBRACKET) {
        VarDecl* vardecl = parseVarDecl();
        Token* semi_colon = parseToken(Lexer::Token::SEMI_COLON);

        delete stmtNode;
        stmtNode = new Stmt(vardecl, semi_colon);
    }

    else if (mCurrentToken == Lexer::Token::KW_RETURN) {
        ReturnStmt* returnStmt = parseReturnStmt();
        Token* semi_colon = parseToken(Lexer::Token::SEMI_COLON);

        delete stmtNode;
        stmtNode = new Stmt(returnStmt, semi_colon);
    }

    else if (mCurrentToken == Lexer::Token::IDENTIFIER) {
        if (peekNextToken() == Lexer::Token::ASSIGN) {
            AssignmentStmt* assignment_stmt = parseAssignmentStmt();
            Token* semi_colon = parseToken(Lexer::Token::SEMI_COLON);

            delete stmtNode;
            stmtNode = new Stmt(assignment_stmt, semi_colon);
        } else {
            Expression* expression = parseExpression();
            Token* semi_colon = parseToken(Lexer::Token::SEMI_COLON);

            delete stmtNode;
            stmtNode = new Stmt(expression, semi_colon);
        }
    }

    else if (mCurrentToken == Lexer::Token::LPAREN or
             mCurrentToken == Lexer::Token::OP_MINUS or
             mCurrentToken == Lexer::Token::INT_LITERAL or
             mCurrentToken == Lexer::Token::FLOAT_LITERAL or
             mCurrentToken == Lexer::Token::STR_LITERAL or
             mCurrentToken == Lexer::Token::KW_TRUE or
             mCurrentToken == Lexer::Token::KW_FALSE) {
        Expression* exp = parseExpression();
        Token* semi_colon = parseToken(Lexer::Token::SEMI_COLON);

        delete stmtNode;
        stmtNode = new Stmt(exp, semi_colon);
    }

    return stmtNode;
}

AssignmentStmt* Parser::parseAssignmentStmt() {
    auto assignmentStmtNode = new AssignmentStmt();

    while (mCurrentToken != Lexer::Token::TOK_EOF and assignmentStmtNode->first.find(mCurrentToken) == assignmentStmtNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::IDENTIFIER) {
        Token* id = parseToken(Lexer::Token::IDENTIFIER);
        Token* assign = parseToken(Lexer::Token::ASSIGN);
        Expression* expression = parseExpression();

        delete assignmentStmtNode;
        assignmentStmtNode = new AssignmentStmt(id, assign, expression);
    }

    return assignmentStmtNode;
}

ConditionalStmt* Parser::parseConditionalStmt() {
    auto conditionalStmtNode = new ConditionalStmt();

    while (mCurrentToken != Lexer::Token::TOK_EOF and conditionalStmtNode->first.find(mCurrentToken) == conditionalStmtNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::KW_IF) {
        IfBlock* ifBlock = parseIfBlock();
        ElseBlock* elseBlock = parseElseBlock();

        delete conditionalStmtNode;
        conditionalStmtNode = new ConditionalStmt(ifBlock, elseBlock);
    }

    return conditionalStmtNode;
}

LoopStmt* Parser::parseLoopStmt() {
    auto loopStmtNode = new LoopStmt();

    while (mCurrentToken != Lexer::Token::TOK_EOF and loopStmtNode->first.find(mCurrentToken) == loopStmtNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::KW_FOR) {
        ForLoop* forLoop = parseForLoop();
        delete loopStmtNode;
        loopStmtNode = new LoopStmt(forLoop);
    }

    else if (mCurrentToken == Lexer::Token::KW_WHILE) {
        WhileLoop* whileLoop = parseWhileLoop();
        delete loopStmtNode;
        loopStmtNode = new LoopStmt(whileLoop);
    }

    return loopStmtNode;
}

ReturnStmt* Parser::parseReturnStmt() {
    auto returnStmtNode = new ReturnStmt();

    while (mCurrentToken != Lexer::Token::TOK_EOF and returnStmtNode->first.find(mCurrentToken) == returnStmtNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::KW_RETURN) {
        Token* kw_return = parseToken(Lexer::Token::KW_RETURN);
        ReturnVar* returnVar = parseReturnVar();

        delete returnStmtNode;
        returnStmtNode = new ReturnStmt(kw_return, returnVar);
    }

    return returnStmtNode;
}

IfBlock* Parser::parseIfBlock() {
    auto ifBlockNode = new IfBlock();

    while (mCurrentToken != Lexer::Token::TOK_EOF and ifBlockNode->first.find(mCurrentToken) == ifBlockNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::KW_IF) {
        Token* kw_if = parseToken(Lexer::Token::KW_IF);
        Expression* exp = parseExpression();
        Segment* segment = parseSegment();

        delete ifBlockNode;
        ifBlockNode = new IfBlock(kw_if, exp, segment);
    }

    return ifBlockNode;
}

ElseBlock* Parser::parseElseBlock() {
    auto elseBlockNode = new ElseBlock();

    while (mCurrentToken != Lexer::Token::TOK_EOF and elseBlockNode->first.find(mCurrentToken) == elseBlockNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::KW_ELSE) {
        Token* kw_else = parseToken(Lexer::Token::KW_ELSE);
        Segment* segment = parseSegment();

        delete elseBlockNode;
        elseBlockNode = new ElseBlock(kw_else, segment);
    } else {
        // epsilon do nothing
    }

    return elseBlockNode;
}

ForLoop* Parser::parseForLoop() {
    auto forLoopNode = new ForLoop();

    while (mCurrentToken != Lexer::Token::TOK_EOF and forLoopNode->first.find(mCurrentToken) == forLoopNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::KW_FOR) {
        Token* kw_for = parseToken(Lexer::Token::KW_FOR);
        Token* id = parseToken(Lexer::Token::IDENTIFIER);
        Token* kw_in = parseToken(Lexer::Token::KW_IN);
        LoopRange* loopRange = parseLoopRange();
        Segment* segment = parseSegment();

        delete forLoopNode;
        forLoopNode = new ForLoop(kw_for, id, kw_in, loopRange, segment);
    }

    return forLoopNode;
}

LoopRange* Parser::parseLoopRange() {
    auto loopRangeNode = new LoopRange();

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
        Expression* start_expression = parseExpression();
        Token* range = parseToken(Lexer::Token::RANGE);
        Expression* end_expression = parseExpression();
        LoopStep* loopStep = parseLoopStep();

        delete loopRangeNode;
        loopRangeNode = new LoopRange(start_expression, range, end_expression, loopStep);
    }

    return loopRangeNode;
}

LoopStep* Parser::parseLoopStep() {
    auto loopStepNode = new LoopStep();

    while (mCurrentToken != Lexer::Token::TOK_EOF and loopStepNode->first.find(mCurrentToken) == loopStepNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::RANGE) {
        Token* range = parseToken(Lexer::Token::RANGE);
        Expression* expression = parseExpression();

        delete loopStepNode;
        loopStepNode = new LoopStep(range, expression);
    }

    else if (mCurrentToken == Lexer::Token::LBRACE) {
        // do nothing
    }

    return loopStepNode;
}

WhileLoop* Parser::parseWhileLoop() {
    auto whileLoopNode = new WhileLoop();

    while (mCurrentToken != Lexer::Token::TOK_EOF and whileLoopNode->first.find(mCurrentToken) == whileLoopNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::KW_WHILE) {
        Token* kw_while = parseToken(Lexer::Token::KW_WHILE);
        Expression* exp = parseExpression();
        Segment* segment = parseSegment();

        delete whileLoopNode;
        whileLoopNode = new WhileLoop(kw_while, exp, segment);
    }

    return whileLoopNode;
}

ReturnVar* Parser::parseReturnVar() {
    auto returnVarNode = new ReturnVar();

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
        Expression* exp = parseExpression();

        delete returnVarNode;
        returnVarNode = new ReturnVar(exp);
    }

    else if (mCurrentToken == Lexer::Token::SEMI_COLON) {
        // do nothing
    }

    return returnVarNode;
}

Segment* Parser::parseSegment() {
    auto segmentNode = new Segment();

    while (mCurrentToken != Lexer::Token::TOK_EOF and segmentNode->first.find(mCurrentToken) == segmentNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken == Lexer::Token::LBRACE) {
        Token* lbrace = parseToken(Lexer::Token::LBRACE);
        StmtList* stmtList = parseStmtList();
        Token* rbrace = parseToken(Lexer::Token::RBRACE);

        delete segmentNode;
        segmentNode = new Segment(lbrace, stmtList, rbrace);
    }

    return segmentNode;
}

Token* Parser::parseToken(Lexer::Token token) {
    Crust::Token* parent;

    if (mCurrentToken != token) {
        std::cout << "Unknown Error\n";  // FIXME: Use ErrorLogger
    }

    else if (token == Lexer::Token::IDENTIFIER or token == Lexer::Token::STR_LITERAL) {
        parent = new Token(token, mLexer.getCurrentStr());
    }

    else if (token == Lexer::Token::INT_LITERAL) {
        parent = new Token(token, mLexer.getCurrentInt());
    }

    else if (token == Lexer::Token::FLOAT_LITERAL) {
        parent = new Token(token, mLexer.getCurrentFloat());
    }

    else {
        parent = new Token(token);
    }

    mCurrentToken = mLexer.getNextToken();
    return parent;
}

Type* Parser::parseType() {
    auto typeNode = new Type();

    while (mCurrentToken != Lexer::Token::TOK_EOF and typeNode->first.find(mCurrentToken) == typeNode->first.end()) {
        ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::VAR_MISSING_IDENTIFIER, mLexer.GetCurrentLocation());
        mCurrentToken = mLexer.getNextToken();
    }

    if (mCurrentToken >= Lexer::Token::KW_INT_32 and mCurrentToken <= Lexer::Token::KW_VOID) {
        Token* atomic_type = parseToken(mCurrentToken);
        delete typeNode;
        typeNode = new Type(atomic_type);
    } else if (mCurrentToken == Lexer::Token::LBRACKET) {
        Token* lbracket = parseToken(Lexer::Token::LBRACKET);
        Token* int_literal = parseToken(Lexer::Token::INT_LITERAL);
        Token* rbracket = parseToken(Lexer::Token::RBRACKET);
        Type* type = parseType();

        delete typeNode;
        typeNode = new Type(lbracket, int_literal, rbracket, type);
    }

    return typeNode;
}

}  // namespace Crust