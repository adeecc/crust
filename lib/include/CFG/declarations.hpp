#pragma once

#include <CFG/cfg.hpp>

namespace Crust {

class ArgList : public CFGNode {
   public:
    ArgList(std::unique_ptr<CFGNode>&& type,
            std::unique_ptr<CFGNode>&& identifier,
            std::unique_ptr<CFGNode>&& comma,
            std::unique_ptr<CFGNode>&& argList) : CFGNode(NodeKind::ARG_LIST, "ARG_LIST") {
        addChildNode(std::move(type));
        addChildNode(std::move(identifier));
        addChildNode(std::move(comma));
        addChildNode(std::move(argList));
    }

    ArgList() : CFGNode(NodeKind::ARG_LIST, "ARG_LIST") {}
};

class FnDecl : public CFGNode {
   public:
    FnDecl() : CFGNode(NodeKind::ERROR, "ERROR") {}

    FnDecl(std::unique_ptr<CFGNode>&& kw_fn,
           std::unique_ptr<CFGNode>&& identifier,
           std::unique_ptr<CFGNode>&& lparen,
           std::unique_ptr<CFGNode>&& argList,
           std::unique_ptr<CFGNode>&& rparen,
           std::unique_ptr<CFGNode>&& type,
           std::unique_ptr<CFGNode>&& lbrace,
           std::unique_ptr<CFGNode>&& stmtList,
           std::unique_ptr<CFGNode>&& rbrace) : CFGNode(NodeKind::FN_DECL, "FN_DECL") {
        addChildNode(std::move(kw_fn));
        addChildNode(std::move(identifier));
        addChildNode(std::move(lparen));
        addChildNode(std::move(argList));
        addChildNode(std::move(rparen));
        addChildNode(std::move(type));
        addChildNode(std::move(lbrace));
        addChildNode(std::move(stmtList));
        addChildNode(std::move(rbrace));
    }
};

class AtomicDecl : public CFGNode {
   public:
    AtomicDecl() : CFGNode(NodeKind::ERROR, "ERROR") {}

    AtomicDecl(std::unique_ptr<CFGNode>&& identifier,
               std::unique_ptr<CFGNode>&& assign,
               std::unique_ptr<CFGNode>&& expression) : CFGNode(NodeKind::ATOMIC_DECL, "ATOMIC_DECL") {
        addChildNode(std::move(identifier));
        addChildNode(std::move(assign));
        addChildNode(std::move(expression));
    }
};

class AtomicDeclList : public CFGNode {
   public:
    AtomicDeclList(std::unique_ptr<CFGNode>&& atomicDecl,
                   std::unique_ptr<CFGNode>&& comma,
                   std::unique_ptr<CFGNode>&& atomicDeclList) : CFGNode(NodeKind::ATOMIC_DECL_LIST, "ATOMIC_DECL_LIST") {
        addChildNode(std::move(atomicDecl));
        addChildNode(std::move(comma));
        addChildNode(std::move(atomicDeclList));
    }

    AtomicDeclList() : CFGNode(NodeKind::ATOMIC_DECL_LIST, "ATOMIC_DECL_LIST") {}
};

class VarDecl : public CFGNode {
   public:
    VarDecl() : CFGNode(NodeKind::ERROR, "ERROR") {}

    VarDecl(std::unique_ptr<CFGNode>&& type,
            std::unique_ptr<CFGNode>&& atomicDeclList) : CFGNode(NodeKind::VAR_DECL, "VAR_DECL") {
        addChildNode(std::move(type));
        addChildNode(std::move(atomicDeclList));
    }
};

class Decl : public CFGNode {
   public:
    Decl() : CFGNode(NodeKind::ERROR, "ERROR") {}

    Decl(std::unique_ptr<CFGNode>&& varDecl, std::unique_ptr<CFGNode>&& semi_colon) : CFGNode(NodeKind::DECL, "DECL") {
        addChildNode(std::move(varDecl));
        addChildNode(std::move(semi_colon));
    }

    Decl(std::unique_ptr<CFGNode>&& fnDecl) : CFGNode(NodeKind::DECL, "DECL") {
        addChildNode(std::move(fnDecl));
    }
};

class DeclList : public CFGNode {
   public:
    DeclList(std::unique_ptr<CFGNode>&& decl,
             std::unique_ptr<CFGNode>&& declList) : CFGNode(NodeKind::DECL_LIST, "DECL_LIST") {
        addChildNode(std::move(decl));
        addChildNode(std::move(declList));
    }

    DeclList() : CFGNode(NodeKind::DECL_LIST, "DECL_LIST") {}
};

class ProgDecl : public CFGNode {
   public:
    ProgDecl() : CFGNode(NodeKind::ERROR, "ERROR") {}

    ProgDecl(std::unique_ptr<CFGNode>&& declList) : CFGNode{NodeKind::PROG_DECL, "PROG_DECL"} {
        addChildNode(std::move(declList));
    }
};

}  // namespace Crust