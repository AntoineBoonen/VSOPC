/*
 * scanner.h
 *
 * by Antoine Boonen
 *
 * This file contains the interface of the Scanner class.
 *
 * Created   21/02/21
 * Modified  16/04/21
 */
 
#include "token.hpp"
#include "error.hpp"

#include <iostream>
#include <vector>

#ifndef VSOPCOMPILER_SCANNER_H
#define VSOPCOMPILER_SCANNER_H


class Scanner {
    private:
        std::string filename; // The file from which to read.
        std::vector<Token> tokens; // A vector of correct tokens.
        std::vector<Error> errors; // A vector of errors.
        size_t token_pos; // The position of the token read by the parser.

    public:
        /*
         * Scanner constructor
         *
         * input:
         *      filename - a string reference containing the path to the file to read from.
         *
         * return:
         *      A Scanner instance.
         */
        explicit Scanner(const std::string &filename);

        /*
         * parser
         *
         * input:
         *      token_vector       - a reference to a Token vector.
         *      error_token_vector - a reference to an error Token vector.
         *
         * return:
         *      -2 - An error occurred when opening the file.
         *      -1 - An error occurred parsing the file according to the vsop rules.
         *       0 - Otherwise;
         */
        int scan();

        /*
         * get_tokens
         *
         * output:
         *      token_vector    - a vector of correct token.
         */
        std::vector<Token> get_tokens();

        /*
         * get_errors
         *
         * output:
         *      error_vector    - a vector of errors.
         */
        std::vector<Error> get_errors();

        /*
         * get_next_token
         *
         * output:
         *      token       - the next token of the token vector.
         */
        Token get_next_token();
};


#endif //VSOPCOMPILER_SCANNER_H
