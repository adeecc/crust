#pragma once

#include <parser/lexer.hpp>
#include <string>
#include <unordered_map>

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
    {Crust::Lexer::Token::LBRACE, "{"},
    {Crust::Lexer::Token::RBRACE, "}"},
    {Crust::Lexer::Token::LBRACKET, "["},
    {Crust::Lexer::Token::RBRACKET, "]"},
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
    {Crust::Lexer::Token::LBRACE, "Left brace"},
    {Crust::Lexer::Token::RBRACE, "Right brace"},
    {Crust::Lexer::Token::LBRACKET, "Left bracket"},
    {Crust::Lexer::Token::RBRACKET, "Right bracket"},
    {Crust::Lexer::Token::LPAREN, "Left paren"},
    {Crust::Lexer::Token::RPAREN, "Right paren"},
    {Crust::Lexer::Token::COMMENT, "Comment"}};