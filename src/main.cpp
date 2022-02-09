#include <iostream>
#include <string>

#include "lexer/lexer.hpp"

int main() {
    Crust::Lexer lexer;
    lexer.init("test.crst");

    Crust::Lexer::Token currentToken;
    currentToken = lexer.getNextToken();

    while (currentToken != Crust::Lexer::Token::TOK_EOF) {
        std::cout << "[" << lexer.GetCurrentLocation().getCurrentLine() << ":" << lexer.GetCurrentLocation().getCurrentColumn() << "] Token ID: " << (int)currentToken;

        switch (currentToken) {
            case Crust::Lexer::Token::IDENTIFIER:
                std::cout << " Identifier Name: " << lexer.getCurrentStr() << std::endl;
                break;
            case Crust::Lexer::Token::STR_LITERAL:
                std::cout << " String Literal: " << lexer.getCurrentStr() << std::endl;
                break;

            case Crust::Lexer::Token::INT_LITERAL:
                std::cout << " Int Literal: " << lexer.getCurrentInt() << std::endl;
                break;

            case Crust::Lexer::Token::FLOAT_LITERAL:
                std::cout << " Float Literal: " << lexer.getCurrentFloat() << std::endl;
                break;

            default:
                std::cout << std::endl;

                break;
        }

        currentToken = lexer.getNextToken();
    }

    return 0;
}