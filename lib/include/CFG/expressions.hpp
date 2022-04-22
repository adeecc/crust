#pragma once

#include <CFG/cfg.hpp>

namespace Crust {

class CallParamList_ : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::COMMA);
        first.insert(Lexer::Token::RPAREN);
    }

   public:
    CallParamList_(std::unique_ptr<CFGNode>&& comma,
                   std::unique_ptr<CFGNode>&& CallParamList) : CFGNode(NodeKind::CALL_PARAM_LIST_, "CALL_PARAM_LIST_") {
        addChildNode(std::move(comma));
        addChildNode(std::move(CallParamList));

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
    CallParamList(std::unique_ptr<CFGNode>&& expression,
                  std::unique_ptr<CFGNode>&& CallParamList_) : CFGNode(NodeKind::CALL_PARAM_LIST, "CALL_PARAM_LIST") {
        addChildNode(std::move(expression));
        addChildNode(std::move(CallParamList_));

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
    Call(std::unique_ptr<CFGNode>&& identifier,
         std::unique_ptr<CFGNode>&& lparen,
         std::unique_ptr<CFGNode>&& callParamList,
         std::unique_ptr<CFGNode>&& rparen) : CFGNode(NodeKind::CALL, "CALL") {
        addChildNode(std::move(identifier));
        addChildNode(std::move(lparen));
        addChildNode(std::move(callParamList));
        addChildNode(std::move(rparen));

        generate_first();
    }
};

class ArraySubscript : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::IDENTIFIER);
    }

   public:
    ArraySubscript() : CFGNode(NodeKind::ERROR, "ERROR") {}

    ArraySubscript(std::unique_ptr<CFGNode>&& identifier,
                   std::unique_ptr<CFGNode>&& lbracket,
                   std::unique_ptr<CFGNode>&& expression,
                   std::unique_ptr<CFGNode>&& rbracket) : CFGNode(NodeKind::ARRAY_SUBSCRIPT, "ARRAY_SUBSCRIPT") {
        addChildNode(std::move(identifier));
        addChildNode(std::move(lbracket));
        addChildNode(std::move(expression));
        addChildNode(std::move(rbracket));

        generate_first();
    }
};

class FloatTerm : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::FLOAT_LITERAL);
        first.insert(Lexer::Token::INT_LITERAL);
    }

   public:
    FloatTerm() : CFGNode(NodeKind::ERROR, "ERROR") {}

    FloatTerm(std::unique_ptr<CFGNode>&& literal) : CFGNode(NodeKind::FLOAT_TERM, "FLOAT_TERM") {
        addChildNode(std::move(literal));

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
    Term() : CFGNode(NodeKind::ERROR, "ERROR") {}

    Term(std::unique_ptr<CFGNode>&& lparen,
         std::unique_ptr<CFGNode>&& expression,
         std::unique_ptr<CFGNode>&& rparen) : CFGNode(NodeKind::TERM, "TERM") {
        addChildNode(std::move(lparen));
        addChildNode(std::move(expression));
        addChildNode(std::move(rparen));

        generate_first();
    }

    Term(std::unique_ptr<CFGNode>&& val) : CFGNode(NodeKind::TERM, "TERM") {
        addChildNode(std::move(val));

        generate_first();
    }

    Term(std::unique_ptr<CFGNode>&& op_minus,
         std::unique_ptr<CFGNode>&& val) : CFGNode(NodeKind::TERM, "TERM") {
        addChildNode(std::move(op_minus));
        addChildNode(std::move(val));

        generate_first();
    }
};

class ExpressionRHS : public CFGNode {
    void generate_first() {
        for (unsigned t = (unsigned)Lexer::Token::OP_PLUS; t <= (unsigned)Lexer::Token::OP_LT; ++t) {
            first.insert(static_cast<Lexer::Token>(t));
        }
        first.insert(Lexer::Token::RPAREN);
        first.insert(Lexer::Token::RBRACKET);
        first.insert(Lexer::Token::COMMA);
        first.insert(Lexer::Token::SEMI_COLON);
        first.insert(Lexer::Token::LBRACE);
        first.insert(Lexer::Token::RANGE);
    }

   public:
    ExpressionRHS(std::unique_ptr<CFGNode>&& bin_op,
                  std::unique_ptr<CFGNode>&& expression) : CFGNode(NodeKind::EXPRESSION_RHS, "EXPRESSION_RHS") {
        addChildNode(std::move(bin_op));
        addChildNode(std::move(expression));

        generate_first();
    }

    ExpressionRHS() : CFGNode(NodeKind::EXPRESSION_RHS, "EXPRESSION_RHS") {
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
    Expression() : CFGNode(NodeKind::ERROR, "ERROR") {}

    Expression(std::unique_ptr<CFGNode>&& term,
               std::unique_ptr<CFGNode>&& expression_rhs) : CFGNode(NodeKind::EXPRESSION, "EXPRESSION") {
        addChildNode(std::move(term));
        addChildNode(std::move(expression_rhs));

        generate_first();
    }
};

}  // namespace Crust