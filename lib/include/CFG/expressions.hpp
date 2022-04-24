#pragma once

#include <CFG/cfg.hpp>
#include <CFG/misc.hpp>
#include <TAC/Quad.hpp>

namespace Crust {

class CallParamList_ : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::COMMA);
        first.insert(Lexer::Token::RPAREN);
    }

   public:
    CallParamList_(CFGNode* comma,
                   CFGNode* CallParamList) : CFGNode(NodeKind::CALL_PARAM_LIST_, "CALL_PARAM_LIST_") {
        addCode(CallParamList->getCode());

        addChildNode(comma);
        addChildNode(CallParamList);

        generate_first();
    }

    CallParamList_() : CFGNode(NodeKind::CALL_PARAM_LIST_, "CALL_PARAM_LIST_") {
        generate_first();
    }
};

class CallParamList : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::LPAREN);
        first.insert(Lexer::Token::OP_MINUS);
        first.insert(Lexer::Token::KW_TRUE);
        first.insert(Lexer::Token::KW_FALSE);
        first.insert(Lexer::Token::STR_LITERAL);
        first.insert(Lexer::Token::IDENTIFIER);
        first.insert(Lexer::Token::FLOAT_LITERAL);
        first.insert(Lexer::Token::INT_LITERAL);
        first.insert(Lexer::Token::RPAREN);
    }

   public:
    CallParamList(CFGNode* expression,
                  CFGNode* CallParamList_) : CFGNode(NodeKind::CALL_PARAM_LIST, "CALL_PARAM_LIST") {
        addChildNode(expression);
        addChildNode(CallParamList_);

        addCode(expression->getCode());
        addCode(CallParamList_->getCode());

        generate_first();
    }

    CallParamList() : CFGNode(NodeKind::CALL_PARAM_LIST, "CALL_PARAM_LIST") {
        generate_first();
    }
};

class Call : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::IDENTIFIER);
    }

   public:
    Call(Token* identifier,
         CFGNode* lparen,
         CFGNode* callParamList,
         CFGNode* rparen) : CFGNode(NodeKind::CALL, "CALL") {
        addChildNode(identifier);
        addChildNode(lparen);
        addChildNode(callParamList);
        addChildNode(rparen);

        addCode(callParamList->getCode());
        addCode(
            new Operator("CALL"),
            new Name(identifier->getVal()),
            nullptr,
            new TempVar());

        generate_first();
    }

    Call() : CFGNode(NodeKind::ERROR, "ERROR") {
        generate_first();
    }
};

class ArraySubscript : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::IDENTIFIER);
    }

   public:
    ArraySubscript() : CFGNode(NodeKind::ERROR, "ERROR") {
        generate_first();
    }

    ArraySubscript(Token* identifier,
                   CFGNode* lbracket,
                   CFGNode* expression,
                   CFGNode* rbracket) : CFGNode(NodeKind::ARRAY_SUBSCRIPT, "ARRAY_SUBSCRIPT") {
        addCode(expression->getCode());
        addCode(
            new Operator("SUBSCRIPT"),
            new Name(identifier->getVal()),
            expression->getCode().back().getRes(),
            new TempVar());

        addChildNode(identifier);
        addChildNode(lbracket);
        addChildNode(expression);
        addChildNode(rbracket);

        generate_first();
    }
};

class FloatTerm : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::FLOAT_LITERAL);
        first.insert(Lexer::Token::INT_LITERAL);
    }

   public:
    FloatTerm() : CFGNode(NodeKind::ERROR, "ERROR") {
        generate_first();
    }

    FloatTerm(Token* literal) : CFGNode(NodeKind::FLOAT_TERM, "FLOAT_TERM") {
        addCode(
            new Operator(Lexer::Token::ASSIGN),
            new Name(literal->getVal()),
            nullptr,
            new TempVar());
        addChildNode(literal);

        generate_first();
    }
};

class Term : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::LPAREN);
        first.insert(Lexer::Token::OP_MINUS);
        first.insert(Lexer::Token::FLOAT_LITERAL);
        first.insert(Lexer::Token::INT_LITERAL);
        first.insert(Lexer::Token::KW_TRUE);
        first.insert(Lexer::Token::KW_FALSE);
        first.insert(Lexer::Token::STR_LITERAL);
        first.insert(Lexer::Token::IDENTIFIER);
    }

   public:
    Term() : CFGNode(NodeKind::ERROR, "ERROR") {
        generate_first();
    }

    Term(CFGNode* lparen,
         CFGNode* expression,
         CFGNode* rparen) : CFGNode(NodeKind::TERM, "TERM") {
        addCode(expression->getCode());
        addCode(
            new Operator(Lexer::Token::ASSIGN),
            expression->getCode().back().getRes(),
            nullptr,
            new TempVar());

        addChildNode(lparen);
        addChildNode(expression);
        addChildNode(rparen);

        generate_first();
    }

    Term(Token* literal_or_id) : CFGNode(NodeKind::TERM, "TERM") {
        addCode(
            new Operator(Lexer::Token::ASSIGN),
            new Name(literal_or_id->getVal()),
            nullptr,
            new TempVar());

        addChildNode(literal_or_id);
        generate_first();
    }

    Term(CFGNode* termNode) : CFGNode(NodeKind::TERM, "TERM") {
        addCode(termNode->getCode());

        addChildNode(termNode);
        generate_first();
    }

    Term(CFGNode* op_minus,
         CFGNode* val) : CFGNode(NodeKind::TERM, "TERM") {
        addCode(val->getCode());
        addCode(
            new Operator("UNARY_MINUS"),
            val->getCode().back().getRes(),
            nullptr,
            new TempVar());

        addChildNode(op_minus);
        addChildNode(val);

        generate_first();
    }
};

class Expression : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::LPAREN);
        first.insert(Lexer::Token::OP_MINUS);
        first.insert(Lexer::Token::KW_TRUE);
        first.insert(Lexer::Token::KW_FALSE);
        first.insert(Lexer::Token::STR_LITERAL);
        first.insert(Lexer::Token::FLOAT_LITERAL);
        first.insert(Lexer::Token::INT_LITERAL);
        first.insert(Lexer::Token::IDENTIFIER);
    }

   public:
    Expression() : CFGNode(NodeKind::ERROR, "ERROR") {
        generate_first();
    }

    Expression(CFGNode* term,
               Token* bin_op,
               CFGNode* expression) : CFGNode(NodeKind::EXPRESSION, "EXPRESSION") {
        addCode(term->getCode());
        addCode(expression->getCode());

        addCode(
            new Operator(bin_op->getToken()),
            term->getCode().back().getRes(),
            expression->getCode().back().getRes(),
            new TempVar());

        addChildNode(term);
        addChildNode(bin_op);
        addChildNode(expression);

        generate_first();
    }

    Expression(CFGNode* term) : CFGNode(NodeKind::EXPRESSION, "EXPRESSION") {
        addChildNode(term);
        addCode(term->getCode());

        generate_first();
    }
};

}  // namespace Crust