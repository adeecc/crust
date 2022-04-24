#pragma once

#include <CFG/cfg.hpp>
#include <parser/lexer.hpp>

namespace Crust {

class FnParamList_ : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::COMMA);
        first.insert(Lexer::Token::RPAREN);
    }

   public:
    FnParamList_(CFGNode* comma,
                 CFGNode* fnParamList) : CFGNode(NodeKind::FN_PARAM_LIST_, "FN_PARAM_LIST_") {
        addChildNode(comma);
        addChildNode(fnParamList);

        generate_first();
    }

    FnParamList_() : CFGNode(NodeKind::FN_PARAM_LIST_, "FN_PARAM_LIST_") {
        generate_first();
    }
};

class FnParam : public CFGNode {
    void generate_first() {
        for (unsigned t = (unsigned)Lexer::Token::KW_INT_32; t <= (unsigned)Lexer::Token::KW_VOID; ++t) {
            first.insert(static_cast<Lexer::Token>(t));
        }
        first.insert(Lexer::Token::LBRACKET);
    }

   public:
    FnParam(CFGNode* type,
            CFGNode* identifier) : CFGNode(NodeKind::FN_PARAM, "FN_PARAM") {
        addChildNode(type);
        addChildNode(identifier);

        generate_first();
    }

    FnParam() : CFGNode(NodeKind::ERROR, "ERROR") {
        generate_first();
    }
};

class FnParamList : public CFGNode {
    void generate_first() {
        for (unsigned t = (unsigned)Lexer::Token::KW_INT_32; t <= (unsigned)Lexer::Token::KW_VOID; ++t) {
            first.insert(static_cast<Lexer::Token>(t));
        }
        first.insert(Lexer::Token::LBRACKET);
        first.insert(Lexer::Token::RPAREN);
    }

   public:
    FnParamList(CFGNode* fnParam,
                CFGNode* fnParamList_) : CFGNode(NodeKind::FN_PARAM_LIST, "FN_PARAM_LIST") {
        addChildNode(fnParam);
        addChildNode(fnParamList_);

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
    FnDecl() : CFGNode(NodeKind::ERROR, "ERROR") {
        generate_first();
    }

    FnDecl(CFGNode* kw_fn,
           CFGNode* identifier,
           CFGNode* lparen,
           CFGNode* fnParamList,
           CFGNode* rparen,
           CFGNode* type,
           CFGNode* segment) : CFGNode(NodeKind::FN_DECL, "FN_DECL") {
        addCode(segment->getCode());

        addChildNode(kw_fn);
        addChildNode(identifier);
        addChildNode(lparen);
        addChildNode(fnParamList);
        addChildNode(rparen);
        addChildNode(type);
        addChildNode(segment);

        generate_first();
    }
};

class VarDeclList_ : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::COMMA);
        first.insert(Lexer::Token::SEMI_COLON);
    }

   public:
    VarDeclList_(CFGNode* comma,
                 CFGNode* varDeclList) : CFGNode(NodeKind::VAR_DECL_LIST_, "VAR_DECL_LIST_") {
        addCode(varDeclList->getCode());

        addChildNode(comma);
        addChildNode(varDeclList);

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
    VarDeclList() : CFGNode(NodeKind::ERROR, "ERROR") {
        generate_first();
    }

    VarDeclList(CFGNode* identifier,
                CFGNode* varDeclList_) : CFGNode(NodeKind::VAR_DECL_LIST, "VAR_DECL_LIST") {
        addCode(varDeclList_->getCode());

        addChildNode(identifier);
        addChildNode(varDeclList_);

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
    VarDecl() : CFGNode(NodeKind::ERROR, "ERROR") {
        generate_first();
    }

    VarDecl(CFGNode* type,
            CFGNode* varDeclList) : CFGNode(NodeKind::VAR_DECL, "VAR_DECL") {
        addCode(varDeclList->getCode());

        addChildNode(type);
        addChildNode(varDeclList);

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
    Decl() : CFGNode(NodeKind::ERROR, "ERROR") {
        generate_first();
    }

    Decl(CFGNode* varDecl, CFGNode* semi_colon) : CFGNode(NodeKind::DECL, "DECL") {
        addCode(varDecl->getCode());

        addChildNode(varDecl);
        addChildNode(semi_colon);

        generate_first();
    }

    Decl(CFGNode* fnDecl) : CFGNode(NodeKind::DECL, "DECL") {
        addCode(fnDecl->getCode());
        addChildNode(fnDecl);
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
    DeclList(CFGNode* decl,
             CFGNode* declList) : CFGNode(NodeKind::DECL_LIST, "DECL_LIST") {
        addCode(decl->getCode());
        addCode(declList->getCode());

        addChildNode(decl);
        addChildNode(declList);

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
    ProgDecl() : CFGNode(NodeKind::ERROR, "ERROR") {
        generate_first();
    }

    ProgDecl(CFGNode* declList) : CFGNode{NodeKind::PROG_DECL, "PROG_DECL"} {
        addCode(declList->getCode());
        addChildNode(declList);
        generate_first();
    }
};

}  // namespace Crust