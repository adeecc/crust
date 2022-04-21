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

    std::unique_ptr<CFGNode> parseProgram(const std::string& filename);


   private:
    void skipToNextSemiColon();
    Lexer::Token peekNextToken();

   private:
    std::unique_ptr<ProgDecl> parseProgramDecl();

    std::unique_ptr<DeclList> parseDeclList();
    std::unique_ptr<Decl> parseDecl();

    std::unique_ptr<VarDecl> parseVarDecl();
    std::unique_ptr<AtomicDeclList> parseAtomicDeclList();
    std::unique_ptr<AtomicDecl> parseAtomicDecl();

    std::unique_ptr<FnDecl> parseFnDecl();
    std::unique_ptr<ArgList> parseArgList();

    std::unique_ptr<Expression> parseExpression();
    std::unique_ptr<ExpressionRHS> parseExpressionRHS();

    std::unique_ptr<Term> parseTerm();
    std::unique_ptr<IdOrCallTerm> parseIdOrCallTerm();
    std::unique_ptr<Call> parseCall();
    std::unique_ptr<FormalArgList> parseFormalArgList();

    std::unique_ptr<StmtList> parseStmtList();
    std::unique_ptr<Stmt> parseStmt();
    std::unique_ptr<AssignmentStmt> parseAssignmentStmt();
    std::unique_ptr<ConditionalStmt> parseConditionalStmt();
    std::unique_ptr<LoopStmt> parseLoopStmt();
    std::unique_ptr<ReturnStmt> parseReturnStmt();

    std::unique_ptr<IfBlock> parseIfBlock();
    std::unique_ptr<ElifBlocks> parseElifBlocks();
    std::unique_ptr<ElifBlock> parseElifBlock();
    std::unique_ptr<ElseBlock> parseElseBlock();

    std::unique_ptr<ForLoop> parseForLoop();
    std::unique_ptr<LoopRange> parseLoopRange();
    std::unique_ptr<LoopStep> parseLoopStep();

    std::unique_ptr<WhileLoop> parseWhileLoop();

    std::unique_ptr<ReturnVar> parseReturnVar();

    std::unique_ptr<Token> parseToken(Lexer::Token token);
    std::unique_ptr<Type> parseType();

   private:
    Lexer mLexer;
    Lexer::Token mCurrentToken;
};
}  // namespace Crust