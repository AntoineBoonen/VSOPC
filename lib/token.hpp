/*
 * token.h
 *
 * by Antoine Boonen
 *
 * This file contains the interface of the Token class.
 *
 * Created   21/02/21
 * Modified  26/02/21
 */

#include <iostream>
#include <vector>

#ifndef VSOPCOMPILER_TOKEN_H
#define VSOPCOMPILER_TOKEN_H

enum class TokenClass {comments, integer_literal, keyword, type_identifier, object_identifier,
        string_literals, operators, eof, none};

class Token {
    private:
        int line;  // The line where the Token begins
        int column;  // The columns where the Token begins
        TokenClass token_class;  // The class of token
        std::string token_value;  // The content of the token

    public:
        /*
         * Token constructor
         *
         * return:
         *      An empty Token instance.
         */
        Token();

        /*
         * Token constructor
         *
         * input:
         *      token_class - the class of the token.
         *      line - the absolute line value of the beginning of the string.
         *      column - the absolute column value of the beginning of the string.
         *      token_value - the content of the token.
         *
         * return:
         *      A Token instance.
         */
        Token(TokenClass token_class, int line, int column, const std::string &token_value);

        /*
         * get_line
         *
         * return:
         *      line - the line of the token.
         */
        int get_line();

        /*
         * get_column
         *
         * return:
         *      column - the column of the token.
         */
        int get_column();

        /*
         * get_class
         *
         * return:
         *      class - the class of the token.
         */
        TokenClass get_class();

        /*
         * get_value
         *
         * return:
         *      value - the value of the token.
         */
        std::string get_value();

        /*
         * empty
         *
         * return:
         *      true - if the Token is empty.
         *      false, otherwise.
         */
        bool empty();

        /*
         * empty
         *
         * return:
         *      formatted - the Token content formatted as required for this part of the project;
         *      empty string - if the Token if the EOF token.
         */
        std::string get_formatted_string();

        /*
         * compare
         *
         * input:
         *      a - a reference to a Token.
         *      b - a reference to a Token.
         *
         * return:
         *      true - if the a is before b in the file.
         *      false, otherwise.
         */
        static bool compare(const Token &a, const Token &b);
};

#endif //VSOPCOMPILER_TOKEN_H
