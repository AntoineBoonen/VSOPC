/*
 * token.cpp
 *
 * by Antoine Boonen
 *
 * This file contains the implementation of the Token class as described in the interface 'token.h'.
 *
 * Created   21/02/21
 * Modified  26/02/21
 */

#include "token.hpp"

#include <sstream>

Token::Token() {
    this->line = -1;
    this->column = -1;
    this->token_class = TokenClass::none;
}

Token::Token(TokenClass token_class, int line, int column, const std::string &token_value) {
    this->token_class = token_class;
    this->line = line;
    this->column = column;
    this->token_value = token_value;
}

int Token::get_line() {
    return line;
}

int Token::get_column() {
    return column;
}

TokenClass Token::get_class() {
    return token_class;
}

std::string Token::get_value() {
    return token_value;
}


bool Token::empty() {
    return (token_class == TokenClass::none);
}

bool Token::compare(const Token &a, const Token &b) {
    if(a.line < b.line) {
        return true;
    } else if (a.line == b.line and a.column < b.column){
        return true;
    } else {
        return false;
    }
}

std::string Token::get_formatted_string() {
    if(token_class == TokenClass::eof) { // Exit if EOF token
        return std::string();
    }

    std::string formatted = std::to_string(line) + "," + std::to_string(column);

    switch(token_class) {
        case TokenClass::comments:
            return std::string();

        case TokenClass::integer_literal:
            formatted += ",integer-literal," + token_value;
            break;

        case TokenClass::keyword:
            formatted += "," + token_value;
            break;

        case TokenClass::type_identifier:
            formatted += ",type-identifier," + token_value;
            break;

        case TokenClass::object_identifier:
            formatted += ",object-identifier," + token_value;
            break;

        case TokenClass::string_literals:
            formatted += ",string-literal," + token_value;
            break;

        case TokenClass::operators:
            formatted += "," + token_value;
            break;

        default:
            formatted += "," + token_value;
            break;
    }
    return formatted;
}
