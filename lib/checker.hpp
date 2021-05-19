/*
 * checker.h
 *
 * by Antoine Boonen
 *
 * This file contains the interface of the sementic Checker class.
 *
 * Created   08/04/21
 * Modified  16/04/21
 */
#include "node.hpp"
#include "error.hpp"

#include <vector>
#include <map>

#ifndef VSOPCOMPILER_CHECKER_H
#define VSOPCOMPILER_CHECKER_H

class Checker {
    private:
        Node expAST; // A reference to the root of an AST
        std::vector<Error> error_vector; // A vector of errors

        /*
         * build
         *
         * Populate the symbol table
         */
        void build(); 

        /*
         * check typecast
         *
         * input:
         *      n - the AST to check.
         *      local_var - a < defined variable - type > mapping.
         *      curr_clazz - the current clazz.
         *      ints - true if we are in an instantiation context, false otherwise.
         *
         * return:
         *      n - a checked AST with each sub node type set/modified.
         */
        Node check_typecast(Node &n, std::map<std::string, std::string> local_var = std::map<std::string, std::string>(), std::string cur_clazz = std::string(), bool inst = false);

    public:
        /*
         * Checker constructor
         *
         * input:
         *      ast - a reference to an AST.
         * 
         * return:
         *      A Checker instance.
         */
        Checker(Node ast);

        /*
         * check
         *
         * return:
         *      0  the AST check was complete with no semantical error,
         *     -1  semantical errors found in the AST.
         */
        int check();

        /*
         * get_expanded_AST
         *
         * return:
         *      the checked AST.
         */
        Node get_expanded_AST();

        /*
         * Node constructor
         *
         * return:
         *      A vector of semantical errors.
         */
        std::vector<Error> get_errors();
};

#endif //VSOPCOMPILER_CHECKER_H