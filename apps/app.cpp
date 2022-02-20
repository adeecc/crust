#include <crust/lexer.hpp>
#include <crust/utils/printhelper.hpp>
#include <iostream>
#include <string>

int main() {
    Crust::Lexer lexer;
    std::string input_file;

    std::cout << "Enter path to file: ";
    std::cin >> input_file;

    lexer.init(input_file);

    Crust::Lexer::Token currentToken;
    currentToken = lexer.getNextToken();

    while (currentToken != Crust::Lexer::Token::TOK_EOF) {
        std::cout << "[" << lexer.GetCurrentLocation().getCurrentLine() << ":" << lexer.GetCurrentLocation().getCurrentColumn() << "]\t\tToken ID: " << (int)currentToken << "\t\t";

        switch (currentToken) {
            case Crust::Lexer::Token::KW_INT_32... Crust::Lexer::Token::KW_EXPORT:
                std::cout << " Keyword\t\t" << lexer.getCurrentStr() << std::endl;
                break;

            case Crust::Lexer::Token::IDENTIFIER:
                std::cout << " Identifier\t\t" << lexer.getCurrentStr() << std::endl;
                break;

            case Crust::Lexer::Token::STR_LITERAL:
                std::cout << " String Literal\t\t" << lexer.getCurrentStr() << std::endl;
                break;

            case Crust::Lexer::Token::INT_LITERAL:
                std::cout << " Int Literal\t\t" << lexer.getCurrentInt() << std::endl;
                break;

            case Crust::Lexer::Token::FLOAT_LITERAL:
                std::cout << " Float Literal\t\t" << lexer.getCurrentFloat() << std::endl;
                break;

            case Crust::Lexer::Token::OP_PLUS... Crust::Lexer::Token::OP_LT:
                std::cout << " Operator\t\t" << printLogger[currentToken] << std::endl;
                break;

            case Crust::Lexer::Token::SEMI_COLON... Crust::Lexer::Token::COMMENT:
                std::cout << printTokenName[currentToken] << "\t\t" << printLogger[currentToken] << std::endl;
                break;

            case Crust::Lexer::Token::TOK_EOF:
                std::cout << "[EOF] End of File Reached." << std::endl;
                break;

            case Crust::Lexer::Token::UNKNOWN:
                std::cout << "Unknown Token" << std::endl;
                break;

            default:
                std::cout << "Critical Error. State Should be unrechable!" << std::endl;
                break;
        }

        currentToken = lexer.getNextToken();
    }

    return 0;
}