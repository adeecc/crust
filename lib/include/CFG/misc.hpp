#pragma once

#include <CFG/cfg.hpp>
#include <parser/lexer.hpp>

namespace Crust {

class Token : public CFGNode {
   public:
    Token() : CFGNode(NodeKind::ERROR) {}

    Token(Lexer::Token token) : CFGNode(NodeKind::TOKEN), mToken(token) {}

    Lexer::Token getToken() const { return mToken; }

   private:
    Lexer::Token mToken;
};

class Type : public CFGNode {
   public:
    Type() : CFGNode(NodeKind::ERROR) {}

    Type(std::unique_ptr<CFGNode>&& atomic_type) : CFGNode(NodeKind::TYPE) {
        addChildNode(std::move(atomic_type));
    }

    Type(std::unique_ptr<CFGNode>&& lbracket,
         std::unique_ptr<CFGNode>&& int_literal,
         std::unique_ptr<CFGNode>&& rbracket,
         std::unique_ptr<CFGNode>&& type) : CFGNode(NodeKind::TYPE) {
        addChildNode(std::move(lbracket));
        addChildNode(std::move(int_literal));
        addChildNode(std::move(rbracket));
        addChildNode(std::move(type));
    }
};

}  // namespace Crust