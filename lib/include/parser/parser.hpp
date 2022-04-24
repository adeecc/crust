#pragma once

#include <CFG/cfg.hpp>
#include <CFG/declarations.hpp>
#include <CFG/expressions.hpp>
#include <CFG/misc.hpp>
#include <CFG/statements.hpp>
#include <memory>
#include <parser/lexer.hpp>

namespace Crust {

class Parser {
   public:
    explicit Parser() : mCurrentToken{Lexer::Token::TOK_SOF} {}
    ~Parser() = default;  // Not optimal? Do I need to add the other 1/3

    CFGNode* parseProgram(const std::string& filename);

   private:
    void skipToNextSemiColon();
    Lexer::Token peekNextToken();

   private:
    ProgDecl* parseProgramDecl();

    DeclList* parseDeclList();
    Decl* parseDecl();

    VarDecl* parseVarDecl();
    VarDeclList* parseVarDeclList();
    VarDeclList_* parseVarDeclList_();

    FnDecl* parseFnDecl();
    FnParamList* parseFnParamList();
    FnParamList_* parseFnParamList_();
    FnParam* parseFnParam();

    Expression* parseExpression();

    Term* parseTerm();
    FloatTerm* parseFloatTerm();
    ArraySubscript* parseArraySubscript();
    Call* parseCall();
    CallParamList* parseCallParamList();
    CallParamList_* parseCallParamList_();

    StmtList* parseStmtList();
    Stmt* parseStmt();
    ConditionalStmt* parseConditionalStmt();
    AssignmentStmt* parseAssignmentStmt();
    LoopStmt* parseLoopStmt();
    ReturnStmt* parseReturnStmt();

    IfBlock* parseIfBlock();
    ElseBlock* parseElseBlock();

    ForLoop* parseForLoop();
    LoopRange* parseLoopRange();
    LoopStep* parseLoopStep();

    WhileLoop* parseWhileLoop();

    ReturnVar* parseReturnVar();

    Segment* parseSegment();
    Token* parseToken(Lexer::Token token);
    Type* parseType();

   private:
    Lexer mLexer;
    Lexer::Token mCurrentToken;
};
}  // namespace Crust