#pragma once

#include <CFG/cfg.hpp>
#include <parser/lexer.hpp>
#include <string>

namespace Crust {

class Segment : public CFGNode {
    void generate_first() {
        first.insert(Lexer::Token::LBRACE);
    }

   public:
    Segment() : CFGNode(NodeKind::ERROR, "ERROR") {
        generate_first();
    }

    Segment(CFGNode* lbracket,
            CFGNode* stmtList,
            CFGNode* rbracket) : CFGNode(NodeKind::SEGMENT, "SEGMENT") {
        addCode(stmtList->getCode());

        addChildNode(lbracket);
        addChildNode(stmtList);
        addChildNode(rbracket);

        generate_first();
    }
};

class Token : public CFGNode {
   public:
    Token() : CFGNode(NodeKind::ERROR, "ERROR") {
    }

    Token(Lexer::Token token) : CFGNode(NodeKind::TOKEN), mToken(token), mVal(Lexer::token_to_str[(size_t)mToken]) {
        mName = "TOKEN_" + mVal;
    }

    Token(Lexer::Token token, std::string identifier_val) : CFGNode(NodeKind::TOKEN), mToken(token), mVal(identifier_val) {
        std::replace(identifier_val.begin(), identifier_val.end(), ' ', '_');
        mName = "TOKEN_" + Lexer::token_to_str[(size_t)mToken] + "_" + mVal;
    }

    Token(Lexer::Token token, int int_literal) : CFGNode(NodeKind::TOKEN), mToken(token), mVal(std::to_string(int_literal)) {
        mName = "TOKEN_" + Lexer::token_to_str[(size_t)mToken] + "_" + mVal;
    }

    Token(Lexer::Token token, float float_literal) : CFGNode(NodeKind::TOKEN), mToken(token), mVal(std::to_string(float_literal)) {
        mName = "TOKEN_" + Lexer::token_to_str[(size_t)mToken] + "_" + mVal;
    }

    Lexer::Token getToken() const { return mToken; }
    std::string getVal() const { return mVal; }

   private:
    Lexer::Token mToken;
    std::string mVal = "";
};

class Type : public CFGNode {
    void generate_first() {
        for (unsigned t = (unsigned)Lexer::Token::KW_INT_32; t <= (unsigned)Lexer::Token::KW_VOID; ++t) {
            first.insert(static_cast<Lexer::Token>(t));
        }
        first.insert(Lexer::Token::LBRACKET);
    }

   public:
    Type() : CFGNode(NodeKind::ERROR, "ERROR") {
        generate_first();
    }

    Type(CFGNode* atomic_type) : CFGNode(NodeKind::TYPE, "TYPE") {
        addChildNode(atomic_type);

        generate_first();
    }

    Type(CFGNode* lbracket,
         CFGNode* int_literal,
         CFGNode* rbracket,
         CFGNode* type) : CFGNode(NodeKind::TYPE, "TYPE") {
        addChildNode(lbracket);
        addChildNode(int_literal);
        addChildNode(rbracket);
        addChildNode(type);

        generate_first();
    }
};

}  // namespace Crust