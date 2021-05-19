/*
 * generator.h
 *
 * by Antoine Boonen
 *
 * This file contains the interface of the intermediate code Generator class.
 *
 * Created   13/05/21
 * Modified  18/05/21
 */
#include "node.hpp"

#ifndef VSOPCOMPILER_GENERATOR_H
#define VSOPCOMPILER_GENERATOR_H

class Generator {
    private:
        Node ast; // An abstract tree

    public:
        /*
         * Generator
         *
         * input:
         *      ast - a reference to an abstract syntax tree (Node).        
         *      filename - a reference to the filename to write to.
         * 
         * return:
         *      an instance of Generator.
         */
        Generator(Node &ast, std::string &filename);

        /*
         * generate
         *
         * Generate the LLVM representation of the abstract syntax tree.
         */
        void generate();

        /*
         * print
         *
         * Display the LLVM representation on the standard output stream.
         */
        void print();

        /*
         * save
         *
         * input:
         *      filename - a reference to the filename to write to.
         * 
         * return:
         *      0 - the the LLVM representation has been written in filename successfully;
         *     -1 - otherwise.
         */
        int save(std::string &filename);
};

#endif//VSOPCOMPILER_GENERATOR_H
