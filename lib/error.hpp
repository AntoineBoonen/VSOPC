/*
 * error.h
 *
 * by Antoine Boonen
 *
 * This file contains the interface of the Error class.
 *
 * Created   07/04/21
 * Modified  16/04/21
 */

#include <iostream>

#ifndef VSOPCOMPILER_ERROR_H
#define VSOPCOMPILER_ERROR_H

enum class ErrorType {lexical, syntaxical, semantical};

class Error
{
    private:
        static std::string filename; // the filename currently being worked on
        ErrorType type; // the type of error
        int line; // the line of the error in file filename
        int column; // the column of the error in filename
        std::string message; // a detailed message of the error
    public:

        /*
         * Error constructor
         *
         * input:
         *      type - the type of error.
         *      line - the line index of the beginning of the node.
         *      column - the column index of the beginning of the node.
         *
         * return:
         *      An Error instance.
         */
        Error(ErrorType t, int l, int c, std::string m);

        /*
         * print_error_msg
         *
         * Prints a formatted message on the standard error stream.
         */
        void print_error_msg();

        /*
         * set_filename
         *
         * input:
         *      f - the filename.
         */
        static void set_filename(const std::string &f);

        /*
         * Compare
         *
         * input:
         *      e1 - an Error reference.
         *      e2 - an Error reference.         
         * 
         * return:
         *      true  e1 happens before e2 in file filename
         *      false otherwise.
         */
        static bool compare(Error e1, Error e2);
};

#endif //VSOPCOMPILER_ERROR_H