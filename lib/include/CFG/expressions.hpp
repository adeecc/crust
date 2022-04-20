#pragma once

#include <CFG/cfg.hpp>

namespace Crust {

class FormalArgList : public CFGNode {
   public:
    FormalArgList(std::unique_ptr<CFGNode>&& expression, std::unique_ptr<CFGNode>&& comma, std::unique_ptr<CFGNode>&& formalArgList) : CFGNode(NodeKind::FORMAL_ARG_LIST) {
        addChildNode(std::move(expression));
        addChildNode(std::move(comma));
        addChildNode(std::move(formalArgList));
    }

    FormalArgList() : CFGNode(NodeKind::FORMAL_ARG_LIST) {}
};

class Call : public CFGNode {
   public:
    Call(std::unique_ptr<CFGNode>&& lparen, std::unique_ptr<CFGNode>&& formalArgList, std::unique_ptr<CFGNode>&& rparen) : CFGNode(NodeKind::CALL) {
        addChildNode(std::move(lparen));
        addChildNode(std::move(formalArgList));
        addChildNode(std::move(rparen));
    }

    Call() : CFGNode(NodeKind::CALL) {}
};

class IdOrCallTerm : public CFGNode {
   public:
    IdOrCallTerm() : CFGNode(NodeKind::ERROR) {}

    IdOrCallTerm(std::unique_ptr<CFGNode>&& identifier, std::unique_ptr<CFGNode>&& call) : CFGNode(NodeKind::ID_OR_CALL_TERM) {
        addChildNode(std::move(identifier));
        addChildNode(std::move(call));
    }
};

class Term : public CFGNode {
   public:
    Term() : CFGNode(NodeKind::ERROR) {}

    Term(std::unique_ptr<CFGNode>&& lparen, std::unique_ptr<CFGNode>&& expression, std::unique_ptr<CFGNode>&& rparen) : CFGNode(NodeKind::TERM) {
        addChildNode(std::move(lparen));
        addChildNode(std::move(expression));
        addChildNode(std::move(rparen));
    }

    Term(std::unique_ptr<CFGNode>&& literal) : CFGNode(NodeKind::TERM) {
        addChildNode(std::move(literal));
    }

    Term(std::unique_ptr<IdOrCallTerm>&& idOrCallTerm) : CFGNode(NodeKind::TERM) {
        addChildNode(std::move(idOrCallTerm));
    }
};

class ExpressionRHS : public CFGNode {
    ExpressionRHS(std::unique_ptr<CFGNode>&& bin_op, std::unique_ptr<CFGNode>&& expression) : CFGNode(NodeKind::EXPRESSION_RHS) {
        addChildNode(std::move(bin_op));
        addChildNode(std::move(expression));
    }

    ExpressionRHS() : CFGNode(NodeKind::EXPRESSION_RHS) {}
};

class Expression : public CFGNode {
   public:
    Expression() : CFGNode(NodeKind::ERROR) {}

    Expression(std::unique_ptr<CFGNode>&& term, std::unique_ptr<CFGNode>&& expression_rhs) : CFGNode(NodeKind::EXPRESSION) {
        addChildNode(std::move(term));
        addChildNode(std::move(expression_rhs));
    }
};

}  // namespace Crust