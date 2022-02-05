#include <iostream>
#include <string>

#include "lexer/lexer.hpp"

int main() {
    std::cout << "Enter Filename: ";
    std::string filename;
    std::cin >> filename;

    Crust::Lexer lexer;
    lexer.init(filename);

    Crust::Lexer::Token currentToken;
    currentToken = lexer.getNextToken();

    while (currentToken != Crust::Lexer::Token::TOK_EOF) {
        std::cout << "[" << lexer.GetCurrentLocation().getCurrentLine() << "]" << (int)currentToken << std::endl;
        currentToken = lexer.getNextToken();
    }

    return 0;
}