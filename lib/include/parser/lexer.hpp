#pragma once
#include <common/sourceloc.hpp>
#include <string>

namespace Crust {
class Lexer {
   private:
    // Common::Type mCurrentType; /*!< Current type recognized by the lexer */
    SourceLocation mSrcLoc; /*!< Information about the current location of the lexer in the file */
    int mCurrentInt;
    float mCurrentFloat;
    std::string mCurrentStr;
    std::string mBuffer;                   /*!< The lexer buffer */
    std::string::const_iterator mBufferIt; /*!< Iterator of the lexer buffer */

   public:
    enum class Token : unsigned int {
        // Datatypes
        KW_INT_32 = 100,
        KW_INT_64,
        KW_UINT_32,
        KW_UINT_64,
        KW_FLOAT_32,
        KW_FLOAT_64,
        KW_STRING,
        KW_BOOL,
        KW_VOID,
        KW_TRUE,
        KW_FALSE,

        // Declarations
        KW_LET,

        // Conditions
        KW_IF,
        KW_ELIF,
        KW_ELSE,

        // Loops
        KW_FOR,
        KW_IN,
        KW_WHILE,

        // Functions
        KW_FN,
        KW_RETURN,

        // Literals
        INT_LITERAL,
        FLOAT_LITERAL,
        STR_LITERAL,

        // Identifier
        IDENTIFIER,

        // Operators
        OP_PLUS,
        OP_MINUS,
        OP_MULT,
        OP_DIV,
        OP_MOD,
        OP_AND,
        OP_OR,
        OP_GT,
        OP_GE,
        OP_EQ,
        OP_NE,
        OP_LE,
        OP_LT,

        // Misc
        DOT,
        SEMI_COLON,
        COLON,
        COMMA,
        AMP,
        RANGE,
        ASSIGN,
        NAMESPACE,
        LBRACE,
        RBRACE,
        LBRACKET,
        RBRACKET,
        LPAREN,
        RPAREN,

        // Comment
        COMMENT,

        // Start of file
        TOK_SOF,

        // End of file
        TOK_EOF,

        // Unknown
        UNKNOWN
    };

    Lexer()
        : mCurrentInt{0}, mCurrentFloat{0.0f} {};

    bool init(const std::string& filename);

    Token getNextTokenAndComment();
    Token getNextToken();


    //  Common::Type GetCurrentType() const { return mCurrentType; }

    const SourceLocation GetCurrentLocation() const { return mSrcLoc; }

    int getCurrentInt() const { return mCurrentInt; }
    float getCurrentFloat() const { return mCurrentFloat; }

    const std::string& getCurrentStr() const { return mCurrentStr; }

   private:
    Token tokenizeCurrentStr();
    char advance();
};
}  // namespace Crust
