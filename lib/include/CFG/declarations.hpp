#pragma once

#include <CFG/cfg.hpp>
#include <parser/lexer.hpp>

namespace Crust {

class FnParamList : public CFGNode {
    void generate_first() {
        for (unsigned t = (unsigned)Lexer::Token::KW_INT_32; t <= (unsigned)Lexer::Token::KW_VOID; ++t) {
            first.insert(static_cast<Lexer::Token>(t));
        }
        first.insert(Lexer::Token::LBRACKET);
        first.insert(Lexer::Token::RPAREN);
    }

   public:
    FnParamList(std::unique_ptr<CFGNode>&& fnParam,
                std::unique_ptr<CFGNode>&& fnParamList_) : CFGNode(NodeKind::FN_PARAM_LIST, "FN_PARAM_LIST") {
        addChildNode(std::move(fnParam));
        addChildNode(std::move(fnParamList_));

        generate_first();
    }

    FnParamList() : CFGNode(NodeKind::FN_PARAM_LIST, "FN_PARAM_LIST") {

        generate_first();
    }
};

class FnDecl : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::KW_FN);
    }

   public:
    FnDecl() : CFGNode(NodeKind::ERROR, "ERROR") {}

    FnDecl(std::unique_ptr<CFGNode>&& kw_fn,
           std::unique_ptr<CFGNode>&& identifier,
           std::unique_ptr<CFGNode>&& lparen,
           std::unique_ptr<CFGNode>&& fnParamList,
           std::unique_ptr<CFGNode>&& rparen,
           std::unique_ptr<CFGNode>&& type,
           std::unique_ptr<CFGNode>&& segment) : CFGNode(NodeKind::FN_DECL, "FN_DECL") {
        addChildNode(std::move(kw_fn));
        addChildNode(std::move(identifier));
        addChildNode(std::move(lparen));
        addChildNode(std::move(fnParamList));
        addChildNode(std::move(rparen));
        addChildNode(std::move(type));
        addChildNode(std::move(segment));

        generate_first();
    }
};

class VarDeclList_ : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::COMMA);
        first.insert(Lexer::Token::SEMI_COLON);
    }

   public:
    VarDeclList_(std::unique_ptr<CFGNode>&& comma,
                 std::unique_ptr<CFGNode>&& varDeclList) : CFGNode(NodeKind::VAR_DECL_LIST_, "VAR_DECL_LIST_") {
        addChildNode(std::move(comma));
        addChildNode(std::move(varDeclList));

        generate_first();
    }

    VarDeclList_() : CFGNode(NodeKind::VAR_DECL_LIST_, "VAR_DECL_LIST_") {
        generate_first();
    }
};

class VarDeclList : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::IDENTIFIER);
    }

   public:
    VarDeclList() : CFGNode(NodeKind::ERROR, "ERROR") {}

    VarDeclList(std::unique_ptr<CFGNode>&& identifier,
                std::unique_ptr<CFGNode>&& varDeclList_) : CFGNode(NodeKind::VAR_DECL_LIST, "VAR_DECL_LIST") {
        addChildNode(std::move(identifier));
        addChildNode(std::move(varDeclList_));

        generate_first();
    }
};

class VarDecl : public CFGNode {
    void generate_first() {
        for (unsigned t = (unsigned)Lexer::Token::KW_INT_32; t <= (unsigned)Lexer::Token::KW_VOID; ++t) {
            first.insert(static_cast<Lexer::Token>(t));
        }
        first.insert(Lexer::Token::LBRACKET);
    }

   public:
    VarDecl() : CFGNode(NodeKind::ERROR, "ERROR") {}

    VarDecl(std::unique_ptr<CFGNode>&& type,
            std::unique_ptr<CFGNode>&& varDeclList) : CFGNode(NodeKind::VAR_DECL, "VAR_DECL") {
        addChildNode(std::move(type));
        addChildNode(std::move(varDeclList));

        generate_first();
    }
};

class Decl : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::KW_FN);
        for (unsigned t = (unsigned)Lexer::Token::KW_INT_32; t <= (unsigned)Lexer::Token::KW_VOID; ++t) {
            first.insert(static_cast<Lexer::Token>(t));
        }
        first.insert(Lexer::Token::LBRACKET);
    }

   public:
    Decl() : CFGNode(NodeKind::ERROR, "ERROR") {}

    Decl(std::unique_ptr<CFGNode>&& varDecl, std::unique_ptr<CFGNode>&& semi_colon) : CFGNode(NodeKind::DECL, "DECL") {
        addChildNode(std::move(varDecl));
        addChildNode(std::move(semi_colon));

        generate_first();
    }

    Decl(std::unique_ptr<CFGNode>&& fnDecl) : CFGNode(NodeKind::DECL, "DECL") {
        addChildNode(std::move(fnDecl));

        generate_first();
    }
};

class DeclList : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::KW_FN);
        for (unsigned t = (unsigned)Lexer::Token::KW_INT_32; t <= (unsigned)Lexer::Token::KW_VOID; ++t) {
            first.insert(static_cast<Lexer::Token>(t));
        }
        first.insert(Lexer::Token::LBRACKET);
    }

   public:
    DeclList(std::unique_ptr<CFGNode>&& decl,
             std::unique_ptr<CFGNode>&& declList) : CFGNode(NodeKind::DECL_LIST, "DECL_LIST") {
        addChildNode(std::move(decl));
        addChildNode(std::move(declList));

        generate_first();
    }

    DeclList() : CFGNode(NodeKind::DECL_LIST, "DECL_LIST") {
        generate_first();
    }
};

class ProgDecl : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::KW_FN);
        for (unsigned t = (unsigned)Lexer::Token::KW_INT_32; t <= (unsigned)Lexer::Token::KW_VOID; ++t) {
            first.insert(static_cast<Lexer::Token>(t));
        }
        first.insert(Lexer::Token::LBRACKET);
    }

   public:
    ProgDecl() : CFGNode(NodeKind::ERROR, "ERROR") {}

    ProgDecl(std::unique_ptr<CFGNode>&& declList) : CFGNode{NodeKind::PROG_DECL, "PROG_DECL"} {
        addChildNode(std::move(declList));
        generate_first();
    }
};

}  // namespace Crust