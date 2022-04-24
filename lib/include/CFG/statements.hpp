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
    ReturnVar(CFGNode* expression) : CFGNode{NodeKind::RETURN_VAR, "RETURN_VAR"} {
        addChildNode(expression);

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
    WhileLoop() : CFGNode(NodeKind::ERROR, "ERROR") {
        generate_first();
    }

    WhileLoop(CFGNode* kw_while,
              CFGNode* expression,
              CFGNode* segment) : CFGNode(NodeKind::WHILE_LOOP, "WHILE_LOOP") {
        addChildNode(kw_while);
        addChildNode(expression);
        addChildNode(segment);

        generate_first();
    }
};

class LoopStep : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::RANGE);
        first.insert(Lexer::Token::LBRACE);
    }

   public:
    LoopStep(CFGNode* range, CFGNode* expression) : CFGNode(NodeKind::LOOP_STEP, "LOOP_STEP") {
        addChildNode(range);
        addChildNode(expression);

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
    LoopRange() : CFGNode(NodeKind::ERROR, "ERROR") {
        generate_first();
    }

    LoopRange(CFGNode* expression_start,
              CFGNode* range,
              CFGNode* expression_end,
              CFGNode* loopStep) : CFGNode(NodeKind::LOOP_RANGE, "LOOP_RANGE") {
        addChildNode(expression_start);
        addChildNode(range);
        addChildNode(expression_end);
        addChildNode(loopStep);

        generate_first();
    }
};

class ForLoop : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::KW_FOR);
    }

   public:
    ForLoop() : CFGNode(NodeKind::ERROR, "ERROR") {
        generate_first();
    }

    ForLoop(CFGNode* kw_for,
            CFGNode* identifier,
            CFGNode* kw_in,
            CFGNode* loopRange,
            CFGNode* segment) : CFGNode(NodeKind::FOR_LOOP, "FOR_LOOP") {
        addChildNode(kw_for);
        addChildNode(identifier);
        addChildNode(kw_in);
        addChildNode(loopRange);
        addChildNode(segment);

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
    ElseBlock(CFGNode* kw_else,
              CFGNode* segment) : CFGNode(NodeKind::ELSE_BLOCK, "ELSE_BLOCK") {
        addCode(segment->getCode());
        addChildNode(kw_else);
        addChildNode(segment);

        generate_first();
    }

    ElseBlock() : CFGNode{NodeKind::ELSE_BLOCK, "ELSE_BLOCK"} {
        generate_first();
    }
};

class IfBlock : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::KW_IF);
    }

   public:
    IfBlock() : CFGNode(NodeKind::ERROR, "ERROR") {
        generate_first();
    }

    IfBlock(CFGNode* kw_if,
            CFGNode* expression,
            CFGNode* segment) : CFGNode(NodeKind::IF_BLOCK, "IF_BLOCK") {
        addChildNode(kw_if);
        addChildNode(expression);
        addChildNode(segment);

        generate_first();
    }

    IfBlock(CFGNode* kw_if,
            CFGNode* expression,
            CFGNode* segment,
            Label* false_label,
            Label* last_label) : CFGNode(NodeKind::IF_BLOCK, "IF_BLOCK"), last_label(last_label), false_label(false_label) {
        addCode(expression->getCode());
        addCode(
            new Operator(Lexer::Token::OP_EQ),
            expression->getCode().back().getRes(),
            new Constant(0),
            false_label);
        addCode(segment->getCode());
        addCode(
            new Operator("JUMP"),
            nullptr,
            nullptr,
            last_label);

        addChildNode(kw_if);
        addChildNode(expression);
        addChildNode(segment);

        generate_first();
    }

    Label *last_label = nullptr, *false_label = nullptr;
};

class ReturnStmt : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::KW_RETURN);
    }

   public:
    ReturnStmt() : CFGNode(NodeKind::ERROR, "ERROR") {
        generate_first();
    }

    ReturnStmt(CFGNode* kw_return,
               CFGNode* returnVar) : CFGNode{NodeKind::RETURN_STMT, "RETURN_STMT"} {
        addChildNode(kw_return);
        addChildNode(returnVar);

        generate_first();
    }
};

class LoopStmt : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::KW_FOR);
        first.insert(Lexer::Token::KW_WHILE);
    }

   public:
    LoopStmt() : CFGNode(NodeKind::ERROR, "ERROR") {
        generate_first();
    }

    LoopStmt(CFGNode* loopType) : CFGNode{NodeKind::LOOP_STMT, "LOOP_STMT"} {
        addChildNode(loopType);

        generate_first();
    }
};

class ConditionalStmt : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::KW_IF);
    }

   public:
    ConditionalStmt() : CFGNode(NodeKind::ERROR, "ERROR") {
        generate_first();
    }

    ConditionalStmt(CFGNode* if_block,
                    CFGNode* else_block) : CFGNode{NodeKind::CONDITIONAL_STMT, "CONDITIONAL_STMT"} {
        auto false_label = new Label();
        auto last_label = new Label();

        auto children = if_block->getChildrenNodes();
        delete if_block;

        if (children.size() != 3) {
            std::cout << "FATAL ERROR: Generating TAC failed. \n";
            addChildNode(if_block);
            addChildNode(else_block);

            generate_first();
            return;
        }

        if_block = new IfBlock(children[0], children[1], children[2], false_label, last_label);

        addCode(if_block->getCode());
        addCode(nullptr, nullptr, nullptr, false_label);
        addCode(else_block->getCode());
        addCode(nullptr, nullptr, nullptr, last_label);

        addChildNode(if_block);
        addChildNode(else_block);

        generate_first();
    }
};

class AssignmentStmt : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::IDENTIFIER);
    }

   public:
    AssignmentStmt() : CFGNode(NodeKind::ERROR, "ERROR") {
        generate_first();
    }

    AssignmentStmt(Token* identifier,
                   CFGNode* assign,
                   CFGNode* expression) : CFGNode{NodeKind::ASSIGNMENT_STMT, "ASSIGNMENT_STMT"} {
        addCode(expression->getCode());
        addCode(
            new Operator(Lexer::Token::ASSIGN),
            expression->getCode().back().getRes(),
            nullptr,
            new Name(identifier->getVal()));

        addChildNode(identifier);
        addChildNode(assign);
        addChildNode(expression);

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
    Stmt() : CFGNode(NodeKind::ERROR, "ERROR") {
        generate_first();
    }

    Stmt(CFGNode* stmtNode) : CFGNode(NodeKind::STMT, "STMT") {
        addCode(stmtNode->getCode());

        addChildNode(stmtNode);

        generate_first();
    }

    Stmt(CFGNode* stmtNode, CFGNode* semi_colon) : CFGNode(NodeKind::STMT, "STMT") {
        addCode(stmtNode->getCode());

        addChildNode(stmtNode);
        addChildNode(semi_colon);

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
    StmtList(CFGNode* stmt,
             CFGNode* stmtList) : CFGNode(NodeKind::STMT_LIST, "STMT_LIST") {
        addCode(stmt->getCode());
        addCode(stmtList->getCode());

        addChildNode(stmt);
        addChildNode(stmtList);

        generate_first();
    }

    StmtList() : CFGNode(NodeKind::STMT_LIST, "STMT_LIST") {
        generate_first();
    }
};

}  // namespace Crust