#include "lexer.hpp"

#include <algorithm>
#include <fstream>
#include <iterator>

#include "../utils/errorlogger.hpp"

using namespace Crust;

bool Lexer::init(const std::string& filename) {
    std::ifstream stream(filename);
    if (stream) {
        mBuffer.assign(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
        mBufferIt = mBuffer.begin();
        mSrcLoc.init();
        return true;
    }
    return false;
}

Lexer::Token Lexer::getNextToken() {
    while (mBufferIt != mBuffer.end() && isspace(*mBufferIt)) {
        mSrcLoc.advance(*mBufferIt == '\n');
        ++mBufferIt;
    }

    // End of file, we're done
    if (mBufferIt == mBuffer.end())
        return Token::TOK_EOF;

    const char currentChar = *mBufferIt;

    switch (currentChar) {
        case ',':
            ++mBufferIt;
            return Token::COMMA;
        case ':':
            ++mBufferIt;
            return Token::COLON;
        case ';':
            ++mBufferIt;
            return Token::SEMI_COLON;
        case '&':
            ++mBufferIt;
            return Token::AMP;
        case '{':
            ++mBufferIt;
            return Token::LCURLYBRACE;
        case '[':
            ++mBufferIt;
            return Token::LSQUAREBRACKET;
        case '(':
            ++mBufferIt;
            return Token::LPAREN;
        case '}':
            ++mBufferIt;
            return Token::RCURLYBRACE;
        case ']':
            ++mBufferIt;
            return Token::RSQUAREBRACKET;
        case ')':
            ++mBufferIt;
            return Token::RPAREN;
        case '=':  // TODO: Add test to verify that a distinction is made between assignment and equality
            if (*(++mBufferIt) == '=') {
                ++mBufferIt;
                return Token::OP_EQ;
            } else {
                return Token::ASSIGN;
            }

        case '+':
            ++mBufferIt;
            return Token::OP_PLUS;
        case '*':
            ++mBufferIt;
            return Token::OP_MULT;
        case '%':
            ++mBufferIt;
            return Token::OP_MOD;
        case '-':
            ++mBufferIt;
            return Token::OP_MINUS;
        case '/':
            if (*(++mBufferIt) == '/') {
                // A comment covers a whole line
                while (mBufferIt != mBuffer.end() && *mBufferIt != '\n')
                    ++mBufferIt;
                return Token::COMMENT;
            } else {
                return Token::OP_DIV;
            }

        case '>':
            if (*(++mBufferIt) == '=') {
                ++mBufferIt;
                return Token::OP_GE;
            } else {
                return Token::OP_GT;
            }
        case '<':
            if (*(++mBufferIt) == '=') {
                ++mBufferIt;
                return Token::OP_LE;
            } else {
                return Token::OP_LT;
            }
        case '!':
            if (*(++mBufferIt) == '=') {
                ++mBufferIt;
                return Token::OP_NE;
            } else {
                return Token::UNKNOWN;  // TODO: Add the Not operator?
            }

        case '\"':
            mCurrentStr.clear();
            while ((++mBufferIt != mBuffer.end()) && (*mBufferIt != '\n') && (*mBufferIt != '\"'))
                mCurrentStr += *mBufferIt;

            if (mBufferIt == mBuffer.end()) {  // Buffer Ended before closing string
                ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::MISSING_CLOSING_QUOTE, mSrcLoc);
                return Token::UNKNOWN;
            } else if (*mBufferIt == '\n') {  // Line Ended before closing string
                ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::NEW_LINE_IN_LITERAL, mSrcLoc);

                while ((mBufferIt != mBuffer.end()) && (*mBufferIt != '\"'))
                    ++mBufferIt;

                ++mBufferIt;

                return Token::UNKNOWN;
            } else {  // String Found!
                ++mBufferIt;
                return Token::STR_LITERAL;
            }

        default:
            if (isalpha(currentChar) or currentChar == '_') {
                mCurrentStr = currentChar;
                while (++mBufferIt != mBuffer.end() and (isalnum(*mBufferIt) or *mBufferIt == '_')) {
                    mCurrentStr += *mBufferIt;
                    mSrcLoc.advance();
                }

                return tokenizeCurrentStr();
            }

            else if (isdigit(currentChar)) {
                std::string numberStr;
                do {
                    numberStr += *mBufferIt;
                } while (isdigit(*(++mBufferIt)));

                if (*mBufferIt == '.') {
                    numberStr += *mBufferIt;
                    ++mBufferIt;

                    do {
                        numberStr += *mBufferIt;
                    } while (isdigit(*(++mBufferIt)));

                    mCurrentFloat = std::stof(numberStr);
                    return Token::FLOAT_LITERAL;
                }

                else if (isalpha(*mBufferIt)) {
                    ErrorLogger::printErrorAtLocation(ErrorLogger::ErrorType::NUMBER_BAD_SUFFIX, mSrcLoc);

                    while ((mBufferIt != mBuffer.end()) and (*mBufferIt != ';'))
                        ++mBufferIt;

                    if (mBufferIt != mBuffer.end())
                        ++mBufferIt;

                    return Token::UNKNOWN;
                }

                else {
                    mCurrentInt = std::stoi(numberStr);
                    return Token::INT_LITERAL;
                }
            }
    }

    return Token::UNKNOWN;
}

Lexer::Token Lexer::tokenizeCurrentStr() {
    if (mCurrentStr == "i32")
        return Token::KW_INT_32;
    else if (mCurrentStr == "i64")
        return Token::KW_INT_64;
    else if (mCurrentStr == "u32")
        return Token::KW_UINT_32;
    else if (mCurrentStr == "u64")
        return Token::KW_UINT_64;
    else if (mCurrentStr == "f32")
        return Token::KW_FLOAT_32;
    else if (mCurrentStr == "f64")
        return Token::KW_FLOAT_64;
    else if (mCurrentStr == "string")
        return Token::KW_STRING;
    else if (mCurrentStr == "bool")
        return Token::KW_BOOL;
    else if (mCurrentStr == "void")
        return Token::KW_VOID;
    else if (mCurrentStr == "true")
        return Token::KW_TRUE;
    else if (mCurrentStr == "false")
        return Token::KW_FALSE;
    else if (mCurrentStr == "let")
        return Token::KW_LET;
    else if (mCurrentStr == "const")
        return Token::KW_CONST;
    else if (mCurrentStr == "if")
        return Token::KW_IF;
    else if (mCurrentStr == "elif")
        return Token::KW_ELIF;
    else if (mCurrentStr == "else")
        return Token::KW_ELSE;
    else if (mCurrentStr == "for")
        return Token::KW_FOR;
    else if (mCurrentStr == "while")
        return Token::KW_WHILE;
    else if (mCurrentStr == "break")
        return Token::KW_BREAK;
    else if (mCurrentStr == "continue")
        return Token::KW_CONTINUE;
    else if (mCurrentStr == "fn")
        return Token::KW_FN;
    else if (mCurrentStr == "return")
        return Token::KW_RETURN;
    else if (mCurrentStr == "import")
        return Token::KW_IMPORT;
    else if (mCurrentStr == "export")
        return Token::KW_EXPORT;
    else if (mCurrentStr == "and")
        return Token::OP_AND;
    else if (mCurrentStr == "or")
        return Token::OP_OR;
    else
        return Token::IDENTIFIER;
}