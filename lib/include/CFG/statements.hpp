#pragma once

#include <CFG/cfg.hpp>

namespace Crust {

class ReturnVar : public CFGNode {
   public:
    ReturnVar(std::unique_ptr<CFGNode>&& expression) : CFGNode{NodeKind::RETURN_VAR, "RETURN_VAR"} {
        addChildNode(std::move(expression));
    }

    ReturnVar() : CFGNode{NodeKind::RETURN_VAR, "RETURN_VAR"} {}
};

class WhileLoop : public CFGNode {
   public:
    WhileLoop() : CFGNode(NodeKind::ERROR, "ERROR") {}

    WhileLoop(std::unique_ptr<CFGNode>&& kw_while,
              std::unique_ptr<CFGNode>&& expression,
              std::unique_ptr<CFGNode>&& lbrace,
              std::unique_ptr<CFGNode>&& stmt_list,
              std::unique_ptr<CFGNode>&& rbrace) : CFGNode(NodeKind::WHILE_LOOP, "WHILE_LOOP") {
        addChildNode(std::move(kw_while));
        addChildNode(std::move(expression));
        addChildNode(std::move(lbrace));
        addChildNode(std::move(stmt_list));
        addChildNode(std::move(rbrace));
    }
};

class LoopStep : public CFGNode {
   public:
    LoopStep(std::unique_ptr<CFGNode>&& range, std::unique_ptr<CFGNode>&& int_literal) : CFGNode(NodeKind::LOOP_STEP, "LOOP_STEP") {
        addChildNode(std::move(range));
        addChildNode(std::move(int_literal));
    }

    LoopStep() : CFGNode(NodeKind::LOOP_STEP, "LOOP_STEP") {}
};

class LoopRange : public CFGNode {
   public:
    LoopRange() : CFGNode(NodeKind::ERROR, "ERROR") {}

    LoopRange(std::unique_ptr<CFGNode>&& int_literal_start,
              std::unique_ptr<CFGNode>&& range,
              std::unique_ptr<CFGNode>&& int_literal_end,
              std::unique_ptr<CFGNode>&& loopStep) : CFGNode(NodeKind::LOOP_RANGE, "LOOP_RANGE") {
        addChildNode(std::move(int_literal_start));
        addChildNode(std::move(range));
        addChildNode(std::move(int_literal_end));
        addChildNode(std::move(loopStep));
    }
};

class ForLoop : public CFGNode {
   public:
    ForLoop() : CFGNode(NodeKind::ERROR, "ERROR") {}

    ForLoop(std::unique_ptr<CFGNode>&& kw_for,
            std::unique_ptr<CFGNode>&& identifier,
            std::unique_ptr<CFGNode>&& kw_in,
            std::unique_ptr<CFGNode>&& loopRange,
            std::unique_ptr<CFGNode>&& lbrace,
            std::unique_ptr<CFGNode>&& stmt_list,
            std::unique_ptr<CFGNode>&& rbrace) : CFGNode(NodeKind::FOR_LOOP, "FOR_LOOP") {
        addChildNode(std::move(kw_for));
        addChildNode(std::move(identifier));
        addChildNode(std::move(kw_in));
        addChildNode(std::move(loopRange));
        addChildNode(std::move(lbrace));
        addChildNode(std::move(stmt_list));
        addChildNode(std::move(rbrace));
    }
};

class ElseBlock : public CFGNode {
   public:
    ElseBlock(std::unique_ptr<CFGNode>&& kw_else,
              std::unique_ptr<CFGNode>&& lbrace,
              std::unique_ptr<CFGNode>&& stmt_list,
              std::unique_ptr<CFGNode>&& rbrace) : CFGNode(NodeKind::ELSE_BLOCK, "ELSE_BLOCK") {
        addChildNode(std::move(kw_else));
        addChildNode(std::move(lbrace));
        addChildNode(std::move(stmt_list));
        addChildNode(std::move(rbrace));
    }

    ElseBlock() : CFGNode{NodeKind::ELSE_BLOCK, "ELSE_BLOCK"} {}
};

class ElifBlock : public CFGNode {
   public:
    ElifBlock() : CFGNode(NodeKind::ERROR, "ERROR") {}

    ElifBlock(std::unique_ptr<CFGNode>&& kw_elif,
              std::unique_ptr<CFGNode>&& expression,
              std::unique_ptr<CFGNode>&& lbrace,
              std::unique_ptr<CFGNode>&& stmt_list,
              std::unique_ptr<CFGNode>&& rbrace) : CFGNode(NodeKind::ELIF_BLOCK, "ELIF_BLOCK") {
        addChildNode(std::move(kw_elif));
        addChildNode(std::move(expression));
        addChildNode(std::move(lbrace));
        addChildNode(std::move(stmt_list));
        addChildNode(std::move(rbrace));
    }
};

class ElifBlocks : public CFGNode {
   public:
    ElifBlocks(std::unique_ptr<CFGNode>&& elif_block, std::unique_ptr<CFGNode>&& elif_blocks) : CFGNode{NodeKind::ELIF_BLOCKS, "ELIF_BLOCKS"} {
        addChildNode(std::move(elif_block));
        addChildNode(std::move(elif_blocks));
    }

    ElifBlocks() : CFGNode{NodeKind::ELIF_BLOCKS, "ELIF_BLOCKS"} {}
};

class IfBlock : public CFGNode {
   public:
    IfBlock() : CFGNode(NodeKind::ERROR, "ERROR") {}

    IfBlock(std::unique_ptr<CFGNode>&& kw_if,
            std::unique_ptr<CFGNode>&& expression,
            std::unique_ptr<CFGNode>&& lbrace,
            std::unique_ptr<CFGNode>&& stmt_list,
            std::unique_ptr<CFGNode>&& rbrace) : CFGNode(NodeKind::IF_BLOCK, "IF_BLOCK") {
        addChildNode(std::move(kw_if));
        addChildNode(std::move(expression));
        addChildNode(std::move(lbrace));
        addChildNode(std::move(stmt_list));
        addChildNode(std::move(rbrace));
    }
};

class ReturnStmt : public CFGNode {
   public:
    ReturnStmt() : CFGNode(NodeKind::ERROR, "ERROR") {}

    ReturnStmt(std::unique_ptr<CFGNode>&& kw_return, std::unique_ptr<CFGNode>&& returnVar) : CFGNode{NodeKind::RETURN_STMT, "RETURN_STMT"} {
        addChildNode(std::move(kw_return));
        addChildNode(std::move(returnVar));
    }
};

class LoopStmt : public CFGNode {
   public:
    LoopStmt() : CFGNode(NodeKind::ERROR, "ERROR") {}

    LoopStmt(std::unique_ptr<CFGNode>&& loopType) : CFGNode{NodeKind::LOOP_STMT, "LOOP_STMT"} {
        addChildNode(std::move(loopType));
    }
};

class ConditionalStmt : public CFGNode {
   public:
    ConditionalStmt() : CFGNode(NodeKind::ERROR, "ERROR") {}

    ConditionalStmt(std::unique_ptr<CFGNode>&& if_block, std::unique_ptr<CFGNode>&& elif_blocks, std::unique_ptr<CFGNode>&& else_block) : CFGNode{NodeKind::CONDITIONAL_STMT} {
        addChildNode(std::move(if_block));
        addChildNode(std::move(elif_blocks));
        addChildNode(std::move(else_block));
    }
};

class AssignmentStmt : public CFGNode {
   public:
    AssignmentStmt() : CFGNode(NodeKind::ERROR, "ERROR") {}

    AssignmentStmt(std::unique_ptr<CFGNode>&& identifier, std::unique_ptr<CFGNode>&& assign, std::unique_ptr<CFGNode>&& expression) : CFGNode{NodeKind::ASSIGNMENT_STMT, "ASSIGNMENT_STMT"} {
        addChildNode(std::move(identifier));
        addChildNode(std::move(assign));
        addChildNode(std::move(expression));
    }
};

class Stmt : public CFGNode {
   public:
    Stmt() : CFGNode(NodeKind::ERROR, "ERROR") {}

    Stmt(std::unique_ptr<CFGNode>&& stmtNode) : CFGNode(NodeKind::STMT, "STMT") {
        addChildNode(std::move(stmtNode));
    }

    Stmt(std::unique_ptr<CFGNode>&& stmtNode, std::unique_ptr<CFGNode>&& semi_colon) : CFGNode(NodeKind::STMT, "STMT") {
        addChildNode(std::move(stmtNode));
        addChildNode(std::move(semi_colon));
    }

    Stmt(std::unique_ptr<CFGNode>&& lbrace, std::unique_ptr<CFGNode>&& stmtNode, std::unique_ptr<CFGNode>&& rbrace) : CFGNode(NodeKind::STMT, "STMT") {
        addChildNode(std::move(lbrace));
        addChildNode(std::move(stmtNode));
        addChildNode(std::move(rbrace));
    }
};

class StmtList : public CFGNode {
   public:
    StmtList(std::unique_ptr<CFGNode>&& stmt,
             std::unique_ptr<CFGNode>&& stmtList) : CFGNode(NodeKind::STMT_LIST, "STMT_LIST") {
        addChildNode(std::move(stmt));
        addChildNode(std::move(stmtList));
    }

    StmtList() : CFGNode(NodeKind::STMT_LIST, "STMT_LIST") {}
};

}  // namespace Crust