#include <iostream>
#include <parser/lexer.hpp>
#include <parser/parser.hpp>
#include <string>
#include <utils/printhelper.hpp>

int main() {
    std::string input_file = "input.gost";
    std::cout << "Parsing file: " << input_file << std::endl;

    Crust::Parser parser;
    auto res = parser.parseProgram(input_file);

    std::cout << *res;

    res->generateDotFile();

    return 0;
}