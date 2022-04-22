#pragma once

#include <CFG/cfg.hpp>

namespace Crust {

class ReturnVar : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::LPAREN);
        first.insert(Lexer::Token::OP_MINUS);
        first.insert(Lexer::Token::KW_TRUE);
        first.insert(Lexer::Token::KW_FALSE);
        first.insert(Lexer::Token::STR_LITERAL);
        first.insert(Lexer::Token::IDENTIFIER);
        first.insert(Lexer::Token::FLOAT_LITERAL);
        first.insert(Lexer::Token::INT_LITERAL);
        first.insert(Lexer::Token::SEMI_COLON);
    }

   public:
    ReturnVar(std::unique_ptr<CFGNode>&& expression) : CFGNode{NodeKind::RETURN_VAR, "RETURN_VAR"} {
        addChildNode(std::move(expression));

        generate_first();
    }

    ReturnVar() : CFGNode{NodeKind::RETURN_VAR, "RETURN_VAR"} {
        generate_first();
    }
};

class WhileLoop : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::KW_WHILE);
    }

   public:
    WhileLoop() : CFGNode(NodeKind::ERROR, "ERROR") {}

    WhileLoop(std::unique_ptr<CFGNode>&& kw_while,
              std::unique_ptr<CFGNode>&& expression,
              std::unique_ptr<CFGNode>&& segment) : CFGNode(NodeKind::WHILE_LOOP, "WHILE_LOOP") {
        addChildNode(std::move(kw_while));
        addChildNode(std::move(expression));
        addChildNode(std::move(segment));

        generate_first();
    }
};

class LoopStep : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::RANGE);
        first.insert(Lexer::Token::LBRACE);
    }

   public:
    LoopStep(std::unique_ptr<CFGNode>&& range, std::unique_ptr<CFGNode>&& expression) : CFGNode(NodeKind::LOOP_STEP, "LOOP_STEP") {
        addChildNode(std::move(range));
        addChildNode(std::move(expression));

        generate_first();
    }

    LoopStep() : CFGNode(NodeKind::LOOP_STEP, "LOOP_STEP") {
        generate_first();
    }
};

class LoopRange : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::LPAREN);
        first.insert(Lexer::Token::OP_MINUS);
        first.insert(Lexer::Token::KW_TRUE);
        first.insert(Lexer::Token::KW_FALSE);
        first.insert(Lexer::Token::STR_LITERAL);
        first.insert(Lexer::Token::IDENTIFIER);
        first.insert(Lexer::Token::FLOAT_LITERAL);
        first.insert(Lexer::Token::INT_LITERAL);
    }

   public:
    LoopRange() : CFGNode(NodeKind::ERROR, "ERROR") {}

    LoopRange(std::unique_ptr<CFGNode>&& expression_start,
              std::unique_ptr<CFGNode>&& range,
              std::unique_ptr<CFGNode>&& expression_end,
              std::unique_ptr<CFGNode>&& loopStep) : CFGNode(NodeKind::LOOP_RANGE, "LOOP_RANGE") {
        addChildNode(std::move(expression_start));
        addChildNode(std::move(range));
        addChildNode(std::move(expression_end));
        addChildNode(std::move(loopStep));

        generate_first();
    }
};

class ForLoop : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::KW_FOR);
    }

   public:
    ForLoop() : CFGNode(NodeKind::ERROR, "ERROR") {}

    ForLoop(std::unique_ptr<CFGNode>&& kw_for,
            std::unique_ptr<CFGNode>&& identifier,
            std::unique_ptr<CFGNode>&& kw_in,
            std::unique_ptr<CFGNode>&& loopRange,
            std::unique_ptr<CFGNode>&& segment) : CFGNode(NodeKind::FOR_LOOP, "FOR_LOOP") {
        addChildNode(std::move(kw_for));
        addChildNode(std::move(identifier));
        addChildNode(std::move(kw_in));
        addChildNode(std::move(loopRange));
        addChildNode(std::move(segment));

        generate_first();
    }
};

class ElseBlock : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::KW_ELSE);
        first.insert(Lexer::Token::LBRACE);
        for (unsigned t = (unsigned)Lexer::Token::KW_INT_32; t <= (unsigned)Lexer::Token::KW_VOID; ++t) {
            first.insert(static_cast<Lexer::Token>(t));
        }
        first.insert(Lexer::Token::LBRACKET);
        first.insert(Lexer::Token::KW_RETURN);
        first.insert(Lexer::Token::IDENTIFIER);
        first.insert(Lexer::Token::LPAREN);
        first.insert(Lexer::Token::OP_MINUS);
        first.insert(Lexer::Token::KW_TRUE);
        first.insert(Lexer::Token::KW_FALSE);
        first.insert(Lexer::Token::STR_LITERAL);
        first.insert(Lexer::Token::KW_IF);
        first.insert(Lexer::Token::KW_FOR);
        first.insert(Lexer::Token::KW_WHILE);
        first.insert(Lexer::Token::FLOAT_LITERAL);
        first.insert(Lexer::Token::INT_LITERAL);
        first.insert(Lexer::Token::RBRACE);
    }

   public:
    ElseBlock(std::unique_ptr<CFGNode>&& kw_else,
              std::unique_ptr<CFGNode>&& segment) : CFGNode(NodeKind::ELSE_BLOCK, "ELSE_BLOCK") {
        addChildNode(std::move(kw_else));
        addChildNode(std::move(segment));

        generate_first();
    }

    ElseBlock() : CFGNode{NodeKind::ELSE_BLOCK, "ELSE_BLOCK"} {
        generate_first();
    }
};

class ElifBlock : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::KW_ELIF);
    }

   public:
    ElifBlock() : CFGNode(NodeKind::ERROR, "ERROR") {}

    ElifBlock(std::unique_ptr<CFGNode>&& kw_elif,
              std::unique_ptr<CFGNode>&& expression,
              std::unique_ptr<CFGNode>&& segment) : CFGNode(NodeKind::ELIF_BLOCK, "ELIF_BLOCK") {
        addChildNode(std::move(kw_elif));
        addChildNode(std::move(expression));
        addChildNode(std::move(segment));

        generate_first();
    }
};

class ElifBlocks : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::KW_ELIF);
        first.insert(Lexer::Token::KW_ELSE);
        first.insert(Lexer::Token::LBRACE);
        for (unsigned t = (unsigned)Lexer::Token::KW_INT_32; t <= (unsigned)Lexer::Token::KW_VOID; ++t) {
            first.insert(static_cast<Lexer::Token>(t));
        }
        first.insert(Lexer::Token::LBRACKET);
        first.insert(Lexer::Token::KW_RETURN);
        first.insert(Lexer::Token::IDENTIFIER);
        first.insert(Lexer::Token::LPAREN);
        first.insert(Lexer::Token::OP_MINUS);
        first.insert(Lexer::Token::KW_TRUE);
        first.insert(Lexer::Token::KW_FALSE);
        first.insert(Lexer::Token::STR_LITERAL);
        first.insert(Lexer::Token::KW_IF);
        first.insert(Lexer::Token::KW_FOR);
        first.insert(Lexer::Token::KW_WHILE);
        first.insert(Lexer::Token::FLOAT_LITERAL);
        first.insert(Lexer::Token::INT_LITERAL);
        first.insert(Lexer::Token::RBRACE);
    }

   public:
    ElifBlocks(std::unique_ptr<CFGNode>&& elif_block,
               std::unique_ptr<CFGNode>&& elif_blocks) : CFGNode{NodeKind::ELIF_BLOCKS, "ELIF_BLOCKS"} {
        addChildNode(std::move(elif_block));
        addChildNode(std::move(elif_blocks));

        generate_first();
    }

    ElifBlocks() : CFGNode{NodeKind::ELIF_BLOCKS, "ELIF_BLOCKS"} {
        generate_first();
    }
};

class IfBlock : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::KW_IF);
    }

   public:
    IfBlock() : CFGNode(NodeKind::ERROR, "ERROR") {}

    IfBlock(std::unique_ptr<CFGNode>&& kw_if,
            std::unique_ptr<CFGNode>&& expression,
            std::unique_ptr<CFGNode>&& segment) : CFGNode(NodeKind::IF_BLOCK, "IF_BLOCK") {
        addChildNode(std::move(kw_if));
        addChildNode(std::move(expression));
        addChildNode(std::move(segment));

        generate_first();
    }
};

class ReturnStmt : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::KW_RETURN);
    }

   public:
    ReturnStmt() : CFGNode(NodeKind::ERROR, "ERROR") {}

    ReturnStmt(std::unique_ptr<CFGNode>&& kw_return,
               std::unique_ptr<CFGNode>&& returnVar) : CFGNode{NodeKind::RETURN_STMT, "RETURN_STMT"} {
        addChildNode(std::move(kw_return));
        addChildNode(std::move(returnVar));

        generate_first();
    }
};

class LoopStmt : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::LPAREN);
        first.insert(Lexer::Token::OP_MINUS);
        first.insert(Lexer::Token::KW_TRUE);
        first.insert(Lexer::Token::KW_FALSE);
        first.insert(Lexer::Token::STR_LITERAL);
        first.insert(Lexer::Token::IDENTIFIER);
        first.insert(Lexer::Token::FLOAT_LITERAL);
        first.insert(Lexer::Token::INT_LITERAL);
    }

   public:
    LoopStmt() : CFGNode(NodeKind::ERROR, "ERROR") {}

    LoopStmt(std::unique_ptr<CFGNode>&& loopType) : CFGNode{NodeKind::LOOP_STMT, "LOOP_STMT"} {
        addChildNode(std::move(loopType));

        generate_first();
    }
};

class ConditionalStmt : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::KW_IF);
    }

   public:
    ConditionalStmt() : CFGNode(NodeKind::ERROR, "ERROR") {}

    ConditionalStmt(std::unique_ptr<CFGNode>&& if_block,
                    std::unique_ptr<CFGNode>&& elif_blocks,
                    std::unique_ptr<CFGNode>&& else_block) : CFGNode{NodeKind::CONDITIONAL_STMT, "CONDITIONAL_STMT"} {
        addChildNode(std::move(if_block));
        addChildNode(std::move(elif_blocks));
        addChildNode(std::move(else_block));

        generate_first();
    }
};

class AssignmentStmt : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::IDENTIFIER);
    }

   public:
    AssignmentStmt() : CFGNode(NodeKind::ERROR, "ERROR") {}

    AssignmentStmt(std::unique_ptr<CFGNode>&& identifier,
                   std::unique_ptr<CFGNode>&& assign,
                   std::unique_ptr<CFGNode>&& expression) : CFGNode{NodeKind::ASSIGNMENT_STMT, "ASSIGNMENT_STMT"} {
        addChildNode(std::move(identifier));
        addChildNode(std::move(assign));
        addChildNode(std::move(expression));

        generate_first();
    }
};

class Stmt : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::LBRACE);
        first.insert(Lexer::Token::KW_IF);
        first.insert(Lexer::Token::KW_FOR);
        first.insert(Lexer::Token::KW_WHILE);
        for (unsigned t = (unsigned)Lexer::Token::KW_INT_32; t <= (unsigned)Lexer::Token::KW_VOID; ++t) {
            first.insert(static_cast<Lexer::Token>(t));
        }
        first.insert(Lexer::Token::LBRACKET);
        first.insert(Lexer::Token::KW_RETURN);
        first.insert(Lexer::Token::IDENTIFIER);
        first.insert(Lexer::Token::LPAREN);
        first.insert(Lexer::Token::OP_MINUS);
        first.insert(Lexer::Token::KW_TRUE);
        first.insert(Lexer::Token::KW_FALSE);
        first.insert(Lexer::Token::STR_LITERAL);
        first.insert(Lexer::Token::FLOAT_LITERAL);
        first.insert(Lexer::Token::INT_LITERAL);
    }

   public:
    Stmt() : CFGNode(NodeKind::ERROR, "ERROR") {}

    Stmt(std::unique_ptr<CFGNode>&& stmtNode) : CFGNode(NodeKind::STMT, "STMT") {
        addChildNode(std::move(stmtNode));

        generate_first();
    }

    Stmt(std::unique_ptr<CFGNode>&& stmtNode, std::unique_ptr<CFGNode>&& semi_colon) : CFGNode(NodeKind::STMT, "STMT") {
        addChildNode(std::move(stmtNode));
        addChildNode(std::move(semi_colon));

        generate_first();
    }
};

class StmtList : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::LBRACE);
        for (unsigned t = (unsigned)Lexer::Token::KW_INT_32; t <= (unsigned)Lexer::Token::KW_VOID; ++t) {
            first.insert(static_cast<Lexer::Token>(t));
        }
        first.insert(Lexer::Token::LBRACKET);
        first.insert(Lexer::Token::KW_RETURN);
        first.insert(Lexer::Token::IDENTIFIER);
        first.insert(Lexer::Token::LPAREN);
        first.insert(Lexer::Token::OP_MINUS);
        first.insert(Lexer::Token::KW_TRUE);
        first.insert(Lexer::Token::KW_FALSE);
        first.insert(Lexer::Token::STR_LITERAL);
        first.insert(Lexer::Token::KW_IF);
        first.insert(Lexer::Token::KW_FOR);
        first.insert(Lexer::Token::KW_WHILE);
        first.insert(Lexer::Token::FLOAT_LITERAL);
        first.insert(Lexer::Token::INT_LITERAL);
        first.insert(Lexer::Token::RBRACE);
    }

   public:
    StmtList(std::unique_ptr<CFGNode>&& stmt,
             std::unique_ptr<CFGNode>&& stmtList) : CFGNode(NodeKind::STMT_LIST, "STMT_LIST") {
        addChildNode(std::move(stmt));
        addChildNode(std::move(stmtList));

        generate_first();
    }

    StmtList() : CFGNode(NodeKind::STMT_LIST, "STMT_LIST") {
        generate_first();
    }
};

}  // namespace Crust