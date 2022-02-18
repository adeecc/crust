#include <gtest/gtest.h>

#include <crust/lexer.hpp>
#include <filesystem>
#include <string>

namespace Crust {

class LexerTest : public ::testing::Test {
   protected:
    void SetUp() override {
        std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;
    }

    void useFile(const std::string& filename) {
        mLexer.init(filename);
    }

    void checkToken(Lexer::Token expectedToken) {
        mCurrentToken = mLexer.getNextToken();
        EXPECT_EQ(mCurrentToken, expectedToken) << "Recived Token: " << (int)mCurrentToken << std::endl;
    }

    void checkIntLiteral(int expectedLiteral) {
        checkToken(Lexer::Token::INT_LITERAL);
        EXPECT_EQ(mLexer.getCurrentInt(), expectedLiteral);
    }

    void checkFloatLiteral(float expectedLiteral) {
        checkToken(Lexer::Token::FLOAT_LITERAL);
        EXPECT_EQ(mLexer.getCurrentFloat(), expectedLiteral);
    }

    void checkStringLiteral(const std::string& expectedLiteral) {
        checkToken(Lexer::Token::STR_LITERAL);
        EXPECT_EQ(mLexer.getCurrentStr(), expectedLiteral);
    }

    void checkIdentifier(const std::string& expectedIdentifier) {
        checkToken(Lexer::Token::IDENTIFIER);
        EXPECT_EQ(mLexer.getCurrentStr(), expectedIdentifier);
    }

    // Members
    Lexer mLexer;
    Lexer::Token mCurrentToken;
};

TEST_F(LexerTest, ReturnsOnEmptyFile) {
    useFile("source_code/empty.crst");
    checkToken(Lexer::Token::TOK_EOF);
}

TEST_F(LexerTest, ReturnsCorrectKeywords) {
    useFile("source_code/keywords.crst");

    checkToken(Lexer::Token::KW_INT_32);
    checkToken(Lexer::Token::KW_INT_64);
    checkToken(Lexer::Token::KW_UINT_32);
    checkToken(Lexer::Token::KW_UINT_64);
    checkToken(Lexer::Token::KW_FLOAT_32);
    checkToken(Lexer::Token::KW_FLOAT_64);
    checkToken(Lexer::Token::KW_STRING);
    checkToken(Lexer::Token::KW_BOOL);
    checkToken(Lexer::Token::KW_VOID);
    checkToken(Lexer::Token::KW_TRUE);
    checkToken(Lexer::Token::KW_FALSE);
    checkToken(Lexer::Token::KW_LET);
    checkToken(Lexer::Token::KW_CONST);
    checkToken(Lexer::Token::KW_IF);
    checkToken(Lexer::Token::KW_ELIF);
    checkToken(Lexer::Token::KW_ELSE);
    checkToken(Lexer::Token::KW_FOR);
    checkToken(Lexer::Token::KW_WHILE);
    checkToken(Lexer::Token::KW_BREAK);
    checkToken(Lexer::Token::KW_CONTINUE);
    checkToken(Lexer::Token::KW_FN);
    checkToken(Lexer::Token::KW_RETURN);
    checkToken(Lexer::Token::KW_IMPORT);
    checkToken(Lexer::Token::KW_EXPORT);

    checkToken(Lexer::Token::TOK_EOF);
}

TEST_F(LexerTest, ReturnsCorrectLiterals) {
    useFile("source_code/literals.crst");
    checkIntLiteral(1);
    checkIntLiteral(132);
    checkFloatLiteral(69.35);
    checkFloatLiteral(420.);
    checkStringLiteral("Hello");
    checkStringLiteral("Never gonna give you up");

    checkToken(Lexer::Token::TOK_EOF);
}

TEST_F(LexerTest, ReturnsCorrectIdentifiers) {
    useFile("source_code/identifiers.crst");

    checkIdentifier("fid");
    checkIdentifier("foo");
    checkIdentifier("barr");
    checkIdentifier("lett");
    checkIdentifier("forr");
    checkIdentifier("fnimport");

    checkToken(Lexer::Token::TOK_EOF);
}

TEST_F(LexerTest, ReturnsCorrectOperators) {
    useFile("source_code/operators.crst");

    // Base Examples
    checkToken(Lexer::Token::OP_PLUS);
    checkToken(Lexer::Token::OP_MINUS);
    checkToken(Lexer::Token::OP_MULT);
    checkToken(Lexer::Token::OP_DIV);
    checkToken(Lexer::Token::OP_MOD);
    checkToken(Lexer::Token::OP_AND);
    checkToken(Lexer::Token::OP_OR);
    checkToken(Lexer::Token::OP_GT);
    checkToken(Lexer::Token::OP_GE);
    checkToken(Lexer::Token::OP_EQ);
    checkToken(Lexer::Token::OP_NE);
    checkToken(Lexer::Token::OP_LE);
    checkToken(Lexer::Token::OP_LT);

    // Mix and Match Symbols
    checkToken(Lexer::Token::OP_GE);
    checkToken(Lexer::Token::ASSIGN);

    checkToken(Lexer::Token::ASSIGN);
    checkToken(Lexer::Token::OP_GE);

    checkToken(Lexer::Token::OP_LE);
    checkToken(Lexer::Token::ASSIGN);

    checkToken(Lexer::Token::ASSIGN);
    checkToken(Lexer::Token::OP_LE);

    checkToken(Lexer::Token::OP_EQ);
    checkToken(Lexer::Token::ASSIGN);

    checkToken(Lexer::Token::ASSIGN);
    checkToken(Lexer::Token::OP_NE);

    checkToken(Lexer::Token::OP_NE);
    checkToken(Lexer::Token::ASSIGN);

    checkToken(Lexer::Token::TOK_EOF);
}

TEST_F(LexerTest, ReturnsCorrectMiscSymbols) {
    useFile("source_code/misc_symbols.crst");

    // Base Examples
    checkToken(Lexer::Token::DOT);
    checkToken(Lexer::Token::SEMI_COLON);
    checkToken(Lexer::Token::COLON);
    checkToken(Lexer::Token::COMMA);
    checkToken(Lexer::Token::AMP);
    checkToken(Lexer::Token::RANGE);
    checkToken(Lexer::Token::ASSIGN);
    checkToken(Lexer::Token::NAMESPACE);
    checkToken(Lexer::Token::LCURLYBRACE);
    checkToken(Lexer::Token::RCURLYBRACE);
    checkToken(Lexer::Token::LSQUAREBRACKET);
    checkToken(Lexer::Token::RSQUAREBRACKET);
    checkToken(Lexer::Token::LPAREN);
    checkToken(Lexer::Token::RPAREN);

    // Mix and Match Symbols
    checkToken(Lexer::Token::RANGE);
    checkToken(Lexer::Token::DOT);

    checkToken(Lexer::Token::RANGE);
    checkToken(Lexer::Token::RANGE);

    checkToken(Lexer::Token::NAMESPACE);
    checkToken(Lexer::Token::COLON);

    checkToken(Lexer::Token::NAMESPACE);
    checkToken(Lexer::Token::NAMESPACE);
}

TEST_F(LexerTest, ReturnsCorrectComments) {
    useFile("source_code/comments.crst");

    checkToken(Lexer::Token::COMMENT);
    checkToken(Lexer::Token::COMMENT);
    checkToken(Lexer::Token::COMMENT);
    checkToken(Lexer::Token::COMMENT);
    checkToken(Lexer::Token::COMMENT);
    checkToken(Lexer::Token::COMMENT);

    checkToken(Lexer::Token::TOK_EOF);
}

}  // namespace Crust