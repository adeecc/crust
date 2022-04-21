#pragma once

#include <CFG/cfg.hpp>
#include <parser/lexer.hpp>
#include <string>

namespace Crust {

class Token : public CFGNode {
   public:
    Token() : CFGNode(NodeKind::ERROR, "ERROR") {}

    Token(Lexer::Token token) : CFGNode(NodeKind::TOKEN), mToken(token) {
        mName = "TOKEN_" + Lexer::token_to_str[(size_t)mToken];
    }

    Token(Lexer::Token token, std::string identifier_val) : CFGNode(NodeKind::TOKEN), mToken(token) {
        mName = "TOKEN_" + Lexer::token_to_str[(size_t)mToken] + "(" + identifier_val + ")";
    }

    Token(Lexer::Token token, int int_literal) : CFGNode(NodeKind::TOKEN), mToken(token) {
        mName = "TOKEN_" + Lexer::token_to_str[(size_t)mToken] + "(" + std::to_string(int_literal) + ")";
    }

    Token(Lexer::Token token, float float_literal) : CFGNode(NodeKind::TOKEN), mToken(token) {
        mName = "TOKEN_" + Lexer::token_to_str[(size_t)mToken] + "(" + std::to_string(float_literal) + ")";
    }

    Lexer::Token getToken() const { return mToken; }

   private:
    Lexer::Token mToken;
};

class Type : public CFGNode {
   public:
    Type() : CFGNode(NodeKind::ERROR, "ERROR") {}

    Type(std::unique_ptr<CFGNode>&& atomic_type) : CFGNode(NodeKind::TYPE, "TYPE") {
        addChildNode(std::move(atomic_type));
    }

    Type(std::unique_ptr<CFGNode>&& lbracket,
         std::unique_ptr<CFGNode>&& int_literal,
         std::unique_ptr<CFGNode>&& rbracket,
         std::unique_ptr<CFGNode>&& type) : CFGNode(NodeKind::TYPE, "TYPE") {
        addChildNode(std::move(lbracket));
        addChildNode(std::move(int_literal));
        addChildNode(std::move(rbracket));
        addChildNode(std::move(type));
    }
};

}  // namespace Crust