#pragma once
#include <string>

#include "../utils/sourceloc.hpp"

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
        KW_CONST,

        // Conditions
        KW_IF,
        KW_ELIF,
        KW_ELSE,

        // Loops
        KW_FOR,
        KW_WHILE,
        KW_BREAK,
        KW_CONTINUE,

        // Functions
        KW_FN,
        KW_RETURN,

        // Modules
        KW_IMPORT,
        KW_EXPORT,

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
        SEMI_COLON,
        COLON,
        COMMA,
        AMP,
        RANGE,
        ASSIGN,
        REFERNCE,
        NAMESPACE,
        LCURLYBRACE,
        RCURLYBRACE,
        LSQUAREBRACKET,
        RSQUAREBRACKET,
        LPAREN,
        RPAREN,

        // Comment
        COMMENT,

        // End of file
        TOK_EOF,

        // Unknown
        UNKNOWN
    };

    Lexer()
        : mCurrentInt{0}, mCurrentFloat{0.0f} {};

    bool init(const std::string& filename);  // TODO: use string_view

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
