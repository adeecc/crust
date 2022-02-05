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
            break;
    }
}