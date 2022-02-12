#include <string>
#include <unordered_map>

#include "../lexer/lexer.hpp"

std::unordered_map<Crust::Lexer::Token, std::string> printLogger = {
    {Crust::Lexer::Token::OP_PLUS, "+"},
    {Crust::Lexer::Token::OP_MINUS, "-"},
    {Crust::Lexer::Token::OP_MULT, "*"},
    {Crust::Lexer::Token::OP_DIV, "/"},
    {Crust::Lexer::Token::OP_MOD, "%"},
    {Crust::Lexer::Token::OP_AND, "and"},
    {Crust::Lexer::Token::OP_OR, "or"},
    {Crust::Lexer::Token::OP_GT, ">"},
    {Crust::Lexer::Token::OP_GE, ">="},
    {Crust::Lexer::Token::OP_EQ, "=="},
    {Crust::Lexer::Token::OP_NE, "!="},
    {Crust::Lexer::Token::OP_LE, "<="},
    {Crust::Lexer::Token::OP_LT, "<"},
    {Crust::Lexer::Token::SEMI_COLON, ";"},
    {Crust::Lexer::Token::COLON, ":"},
    {Crust::Lexer::Token::COMMA, ","},
    {Crust::Lexer::Token::AMP, "&"},
    {Crust::Lexer::Token::RANGE, ".."},
    {Crust::Lexer::Token::ASSIGN, "="},
    {Crust::Lexer::Token::LCURLYBRACE, "{"},
    {Crust::Lexer::Token::RCURLYBRACE, "}"},
    {Crust::Lexer::Token::LSQUAREBRACKET, "["},
    {Crust::Lexer::Token::RSQUAREBRACKET, "]"},
    {Crust::Lexer::Token::LPAREN, "("},
    {Crust::Lexer::Token::RPAREN, ")"},
    {Crust::Lexer::Token::COMMENT, "//"}};

std::unordered_map<Crust::Lexer::Token, std::string> printTokenName = {
    {Crust::Lexer::Token::SEMI_COLON, "Terminator"},
    {Crust::Lexer::Token::COLON, "Colon    "},
    {Crust::Lexer::Token::COMMA, "Comma    "},
    {Crust::Lexer::Token::AMP, "Ampersand"},
    {Crust::Lexer::Token::RANGE, "Range    "},
    {Crust::Lexer::Token::ASSIGN, "Assign    "},
    {Crust::Lexer::Token::LCURLYBRACE, "Left brace"},
    {Crust::Lexer::Token::RCURLYBRACE, "Right brace"},
    {Crust::Lexer::Token::LSQUAREBRACKET, "Left bracket"},
    {Crust::Lexer::Token::RSQUAREBRACKET, "Right bracket"},
    {Crust::Lexer::Token::LPAREN, "Left paren"},
    {Crust::Lexer::Token::RPAREN, "Right paren"},
    {Crust::Lexer::Token::COMMENT, "Comment"}};