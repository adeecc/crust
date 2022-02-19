#include <gtest/gtest.h>

#include <crust/lexer.hpp>
#include <string>

namespace Crust {

class LexerTest : public ::testing::Test {
   protected:
    void SetUp() override {
        // std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;
    }

    void useFile(const std::string& filename) {
        mLexer.init("source_code/" + filename);
    }

    void checkToken(Lexer::Token expectedToken) {
        mCurrentToken = mLexer.getNextToken();
        EXPECT_EQ(mCurrentToken, expectedToken) << "[" << mLexer.GetCurrentLocation().getCurrentLine() << ":" << mLexer.GetCurrentLocation().getCurrentColumn() << "]"
                                                << "\tRec.: " << (int)mCurrentToken
                                                << "\tExp.: " << (int)expectedToken << std::endl;
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
    useFile("basic/empty.crst");
    checkToken(Lexer::Token::TOK_EOF);
}

TEST_F(LexerTest, ReturnsCorrectKeywords) {
    useFile("basic/keywords.crst");

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
    useFile("basic/literals.crst");
    checkIntLiteral(1);
    checkIntLiteral(132);
    checkFloatLiteral(69.35);
    checkFloatLiteral(420.);
    checkStringLiteral("Hello");
    checkStringLiteral("Never gonna give you up");

    checkToken(Lexer::Token::TOK_EOF);
}

TEST_F(LexerTest, ReturnsCorrectIdentifiers) {
    useFile("basic/identifiers.crst");

    checkIdentifier("fid");
    checkIdentifier("foo");
    checkIdentifier("barr");
    checkIdentifier("lett");
    checkIdentifier("forr");
    checkIdentifier("fnimport");

    checkToken(Lexer::Token::TOK_EOF);
}

TEST_F(LexerTest, ReturnsCorrectOperators) {
    useFile("basic/operators.crst");

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
    useFile("basic/misc_symbols.crst");

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
    useFile("basic/comments.crst");

    checkToken(Lexer::Token::COMMENT);
    checkToken(Lexer::Token::COMMENT);
    checkToken(Lexer::Token::COMMENT);
    checkToken(Lexer::Token::COMMENT);
    checkToken(Lexer::Token::COMMENT);
    checkToken(Lexer::Token::COMMENT);

    checkToken(Lexer::Token::TOK_EOF);
}

TEST_F(LexerTest, ReturnsSwapTokens) {
    useFile("full/swap.crst");

    checkToken(Lexer::Token::KW_IMPORT);

    checkIdentifier("std");
    checkToken(Lexer::Token::SEMI_COLON);

    checkToken(Lexer::Token::KW_FN);
    checkToken(Lexer::Token::COLON);
    checkToken(Lexer::Token::KW_VOID);

    checkIdentifier("main");
    checkToken(Lexer::Token::LPAREN);
    checkToken(Lexer::Token::RPAREN);

    checkToken(Lexer::Token::LCURLYBRACE);

    checkToken(Lexer::Token::KW_LET);
    checkIdentifier("a");
    checkToken(Lexer::Token::COLON);
    checkToken(Lexer::Token::KW_INT_32);
    checkToken(Lexer::Token::ASSIGN);
    checkIntLiteral(10);

    checkToken(Lexer::Token::COMMA);

    checkIdentifier("b");
    checkToken(Lexer::Token::COLON);
    checkToken(Lexer::Token::KW_INT_32);
    checkToken(Lexer::Token::ASSIGN);
    checkIntLiteral(20);
    checkToken(Lexer::Token::SEMI_COLON);

    checkIdentifier("print");
    checkToken(Lexer::Token::LPAREN);

    checkIdentifier("a");
    checkToken(Lexer::Token::RPAREN);
    checkToken(Lexer::Token::SEMI_COLON);

    checkIdentifier("print");
    checkToken(Lexer::Token::LPAREN);
    checkIdentifier("b");
    checkToken(Lexer::Token::RPAREN);
    checkToken(Lexer::Token::SEMI_COLON);

    checkToken(Lexer::Token::KW_LET);
    checkIdentifier("t");
    checkToken(Lexer::Token::COLON);
    checkToken(Lexer::Token::KW_INT_32);
    checkToken(Lexer::Token::ASSIGN);
    checkIdentifier("a");
    checkToken(Lexer::Token::SEMI_COLON);

    checkIdentifier("a");
    checkToken(Lexer::Token::ASSIGN);
    checkIdentifier("b");
    checkToken(Lexer::Token::SEMI_COLON);

    checkIdentifier("b");
    checkToken(Lexer::Token::ASSIGN);
    checkIdentifier("t");
    checkToken(Lexer::Token::SEMI_COLON);

    checkIdentifier("print");
    checkToken(Lexer::Token::LPAREN);
    checkIdentifier("a");
    checkToken(Lexer::Token::RPAREN);
    checkToken(Lexer::Token::SEMI_COLON);

    checkIdentifier("print");
    checkToken(Lexer::Token::LPAREN);
    checkIdentifier("b");
    checkToken(Lexer::Token::RPAREN);
    checkToken(Lexer::Token::SEMI_COLON);

    checkToken(Lexer::Token::RCURLYBRACE);

    checkToken(Lexer::Token::TOK_EOF);
}

TEST_F(LexerTest, ReturnsFactorialTokens) {
    useFile("full/fact.crst");

    checkToken(Lexer::Token::KW_IMPORT);
    checkIdentifier("std");
    checkToken(Lexer::Token::SEMI_COLON);

    checkToken(Lexer::Token::KW_FN);
    checkToken(Lexer::Token::COLON);
    checkToken(Lexer::Token::KW_INT_32);
    checkIdentifier("factorial");
    checkToken(Lexer::Token::LPAREN);
    checkIdentifier("n");
    checkToken(Lexer::Token::COLON);
    checkToken(Lexer::Token::KW_INT_32);
    checkToken(Lexer::Token::RPAREN);

    checkToken(Lexer::Token::LCURLYBRACE);

    checkToken(Lexer::Token::KW_IF);
    checkIdentifier("n");
    checkToken(Lexer::Token::OP_EQ);
    checkIntLiteral(1);

    checkToken(Lexer::Token::LCURLYBRACE);

    checkToken(Lexer::Token::KW_RETURN);
    checkIntLiteral(1);
    checkToken(Lexer::Token::SEMI_COLON);

    checkToken(Lexer::Token::RCURLYBRACE);

    checkToken(Lexer::Token::KW_RETURN);
    checkIdentifier("n");
    checkToken(Lexer::Token::OP_MULT);
    checkIdentifier("factorial");
    checkToken(Lexer::Token::LPAREN);
    checkIdentifier("n");
    checkToken(Lexer::Token::OP_MINUS);
    checkIntLiteral(1);
    checkToken(Lexer::Token::RPAREN);
    checkToken(Lexer::Token::SEMI_COLON);

    checkToken(Lexer::Token::RCURLYBRACE);

    checkToken(Lexer::Token::KW_FN);    // fn
    checkToken(Lexer::Token::COLON);    // :
    checkToken(Lexer::Token::KW_VOID);  // void
    checkIdentifier("main");
    checkToken(Lexer::Token::LPAREN);  // (
    checkToken(Lexer::Token::RPAREN);  // )

    checkToken(Lexer::Token::LCURLYBRACE);

    checkToken(Lexer::Token::KW_LET);
    checkIdentifier("ans");
    checkToken(Lexer::Token::COLON);
    checkToken(Lexer::Token::KW_INT_32);
    checkToken(Lexer::Token::ASSIGN);
    checkIdentifier("factorial");
    checkToken(Lexer::Token::LPAREN);
    checkIntLiteral(10);
    checkToken(Lexer::Token::RPAREN);
    checkToken(Lexer::Token::SEMI_COLON);

    checkIdentifier("print");
    checkToken(Lexer::Token::LPAREN);
    checkIdentifier("ans");
    checkToken(Lexer::Token::RPAREN);
    checkToken(Lexer::Token::SEMI_COLON);

    checkToken(Lexer::Token::RCURLYBRACE);

    checkToken(Lexer::Token::TOK_EOF);
}

TEST_F(LexerTest, ReturnsCorrectSortTokens) {
    useFile("full/sort.crst");

    checkToken(Lexer::Token::KW_IMPORT);  // import
    checkIdentifier("std");
    checkToken(Lexer::Token::SEMI_COLON);  // ;

    checkToken(Lexer::Token::KW_FN);           // fn
    checkToken(Lexer::Token::COLON);           // :
    checkToken(Lexer::Token::LSQUAREBRACKET);  // [
    checkToken(Lexer::Token::KW_INT_32);       // i32
    checkToken(Lexer::Token::COMMA);           // ,
    checkIntLiteral(6);
    checkToken(Lexer::Token::RSQUAREBRACKET);  // ]

    checkIdentifier("bubbleSort");
    checkToken(Lexer::Token::LPAREN);  // (
    checkIdentifier("A");
    checkToken(Lexer::Token::COLON);           // :
    checkToken(Lexer::Token::LSQUAREBRACKET);  // [
    checkToken(Lexer::Token::KW_INT_32);       // i32
    checkToken(Lexer::Token::COMMA);           // ,
    checkIntLiteral(6);
    checkToken(Lexer::Token::RSQUAREBRACKET);  // ]
    checkToken(Lexer::Token::RPAREN);          // )

    checkToken(Lexer::Token::LCURLYBRACE);  // {

    //     // FUNCTION BODY

    checkToken(Lexer::Token::KW_LET);  // let
    checkIdentifier("n");
    checkToken(Lexer::Token::COLON);      // :
    checkToken(Lexer::Token::KW_INT_32);  // i32
    checkToken(Lexer::Token::ASSIGN);     // =
    checkIntLiteral(6);
    checkToken(Lexer::Token::SEMI_COLON);  // ;

    //     // OUTER FOR LOOP
    checkToken(Lexer::Token::KW_FOR);  // for
    checkIdentifier("i");
    checkToken(Lexer::Token::KW_IN);  // in
    checkIntLiteral(0);
    checkToken(Lexer::Token::RANGE);   // ..
    checkToken(Lexer::Token::LPAREN);  // (
    checkIdentifier("n");
    checkToken(Lexer::Token::OP_MINUS);  // -
    checkIntLiteral(1);
    checkToken(Lexer::Token::RPAREN);  // )

    checkToken(Lexer::Token::LCURLYBRACE);  // {

    checkToken(Lexer::Token::KW_FOR);  // for
    checkIdentifier("j");
    checkToken(Lexer::Token::KW_IN);  // in
    checkIntLiteral(0);
    checkToken(Lexer::Token::RANGE);   // ..
    checkToken(Lexer::Token::LPAREN);  // (
    checkIdentifier("n");
    checkToken(Lexer::Token::OP_MINUS);  // -
    checkIdentifier("i");
    checkToken(Lexer::Token::OP_MINUS);  // -
    checkIntLiteral(1);
    checkToken(Lexer::Token::RPAREN);  // )

    checkToken(Lexer::Token::LCURLYBRACE);  // {

    //     // IF
    checkToken(Lexer::Token::KW_IF);  // if

    checkIdentifier("A");
    checkToken(Lexer::Token::LSQUAREBRACKET);  // [
    checkIdentifier("j");
    checkToken(Lexer::Token::RSQUAREBRACKET);  // ]

    checkToken(Lexer::Token::OP_GT);  // >

    checkIdentifier("A");
    checkToken(Lexer::Token::LSQUAREBRACKET);  // [
    checkIdentifier("j");
    checkToken(Lexer::Token::OP_PLUS);  // +
    checkIntLiteral(1);
    checkToken(Lexer::Token::RSQUAREBRACKET);  // ]

    checkToken(Lexer::Token::LCURLYBRACE);  // {

    checkToken(Lexer::Token::COMMENT);  //

    checkToken(Lexer::Token::KW_LET);  // let
    checkIdentifier("t");
    checkToken(Lexer::Token::COLON);      // :
    checkToken(Lexer::Token::KW_INT_32);  // i32
    checkToken(Lexer::Token::ASSIGN);     // =
    checkIdentifier("A");
    checkToken(Lexer::Token::LSQUAREBRACKET);  // [
    checkIdentifier("j");
    checkToken(Lexer::Token::RSQUAREBRACKET);  // ]
    checkToken(Lexer::Token::SEMI_COLON);      // ;

    checkIdentifier("A");
    checkToken(Lexer::Token::LSQUAREBRACKET);  // [
    checkIdentifier("j");
    checkToken(Lexer::Token::RSQUAREBRACKET);  // ]
    checkToken(Lexer::Token::ASSIGN);          // =
    checkIdentifier("A");
    checkToken(Lexer::Token::LSQUAREBRACKET);  // [
    checkIdentifier("j");
    checkToken(Lexer::Token::OP_PLUS);  // +
    checkIntLiteral(1);
    checkToken(Lexer::Token::RSQUAREBRACKET);  // ]
    checkToken(Lexer::Token::SEMI_COLON);      // ;

    checkIdentifier("A");
    checkToken(Lexer::Token::LSQUAREBRACKET);  // [
    checkIdentifier("j");
    checkToken(Lexer::Token::OP_PLUS);  // +
    checkIntLiteral(1);
    checkToken(Lexer::Token::RSQUAREBRACKET);  // ]
    checkToken(Lexer::Token::ASSIGN);          // =
    checkIdentifier("t");
    checkToken(Lexer::Token::SEMI_COLON);  // ;

    checkToken(Lexer::Token::RCURLYBRACE);  // }

    checkToken(Lexer::Token::RCURLYBRACE);  // }

    checkToken(Lexer::Token::RCURLYBRACE);  // }

    checkToken(Lexer::Token::KW_RETURN);  // return
    checkIdentifier("A");
    checkToken(Lexer::Token::SEMI_COLON);  // ;

    checkToken(Lexer::Token::RCURLYBRACE);  // }

    // // MAIN FUNCTION

    checkToken(Lexer::Token::KW_FN);    // fn
    checkToken(Lexer::Token::COLON);    // :
    checkToken(Lexer::Token::KW_VOID);  // void
    checkIdentifier("main");
    checkToken(Lexer::Token::LPAREN);  // (
    checkToken(Lexer::Token::RPAREN);  // )

    checkToken(Lexer::Token::LCURLYBRACE);  // {

    checkToken(Lexer::Token::KW_LET);  // let
    checkIdentifier("A");
    checkToken(Lexer::Token::COLON);           // :
    checkToken(Lexer::Token::LSQUAREBRACKET);  // [
    checkToken(Lexer::Token::KW_INT_32);       // i32
    checkToken(Lexer::Token::COMMA);           // ,
    checkIntLiteral(6);
    checkToken(Lexer::Token::RSQUAREBRACKET);  // ]
    checkToken(Lexer::Token::ASSIGN);          // =

    checkToken(Lexer::Token::LCURLYBRACE);

    checkIntLiteral(7);
    checkToken(Lexer::Token::COMMA);

    checkIntLiteral(1);
    checkToken(Lexer::Token::COMMA);

    checkIntLiteral(12);
    checkToken(Lexer::Token::COMMA);

    checkIntLiteral(5);
    checkToken(Lexer::Token::COMMA);

    checkIntLiteral(10);
    checkToken(Lexer::Token::COMMA);

    checkIntLiteral(3);

    checkToken(Lexer::Token::RCURLYBRACE);
    checkToken(Lexer::Token::SEMI_COLON);

    checkIdentifier("A");
    checkToken(Lexer::Token::ASSIGN);  // =
    checkIdentifier("bubbleSort");
    checkToken(Lexer::Token::LPAREN);  // (
    checkIdentifier("A");
    checkToken(Lexer::Token::RPAREN);      // )
    checkToken(Lexer::Token::SEMI_COLON);  // ;

    checkIdentifier("print");
    checkToken(Lexer::Token::LPAREN);  // (
    checkIdentifier("A");
    checkToken(Lexer::Token::RPAREN);      // )
    checkToken(Lexer::Token::SEMI_COLON);  // ;

    checkToken(Lexer::Token::RCURLYBRACE);  // }

    checkToken(Lexer::Token::TOK_EOF);
}

TEST_F(LexerTest, CausesSymbolError) {
    useFile("errors/symbol.crst");

    checkToken(Lexer::Token::UNKNOWN);  // ~
    checkToken(Lexer::Token::UNKNOWN);  // !
    checkToken(Lexer::Token::UNKNOWN);  // @
    checkToken(Lexer::Token::UNKNOWN);  // #
    checkToken(Lexer::Token::UNKNOWN);  // $
    checkToken(Lexer::Token::UNKNOWN);  // ^
    checkToken(Lexer::Token::UNKNOWN);  // ?
    checkToken(Lexer::Token::UNKNOWN);  // '

    checkStringLiteral("~!@#$^?'");

    checkToken(Lexer::Token::TOK_EOF);
}

TEST_F(LexerTest, CausesStringError) {
    useFile("errors/string.crst");

    checkToken(Lexer::Token::UNKNOWN);  // Newline in string literal
    checkToken(Lexer::Token::UNKNOWN);  // Missing closing quote

    checkToken(Lexer::Token::TOK_EOF);
}

TEST_F(LexerTest, CausesNumberBadSuffixError) {
    useFile("errors/numbers.crst");

    checkToken(Lexer::Token::UNKNOWN);  // Bad suffix, closed with ;
    checkToken(Lexer::Token::UNKNOWN);  // Bad suffix, closed with ;
    checkToken(Lexer::Token::UNKNOWN);  // Bad suffix, not closed

    checkToken(Lexer::Token::TOK_EOF);
}

}  // namespace Crust