/*
 * main.cpp
 *
 * by Antoine Boonen
 *
 * Created   21/02/21
 * Modified  18/05/21
 */

#include "error.hpp"
#include "scanner.hpp"
#include "token.hpp"
#include "parser.hpp"
#include "checker.hpp"
#include "node.hpp"
#include "generator.hpp"

#include <cstdlib>
#include <iostream>
#include <vector>

#define COMPILER "clang"
#define OPTIONS "-Wno-override-module -o"
#define OBJECT_PATH "/tmp/vsopc/object.o /tmp/vsopc/external.o"
#define MINIMUM_REQUIRED_ARG 1
#define EXT_LEN 5
#define EXT ".vsop"

enum class Run {none, scanner, parser, checker, executable, generator};

/*
 * display_help
 *
 * Display the help interface.
 */
static void display_help();

int main(int argc, char **argv) {

    // Check the number of arguments
    if(argc < MINIMUM_REQUIRED_ARG - 1) {
        std::cerr << "Not enough arguments; use -h or --help" << std::endl;
        return 1;
    }

    std::string filename;
    Run mode = Run::none; 

    // check the quality of the arguments
    for(int i = 1; i < argc; ++i) {

        if (std::string(argv[i]) == "-lex" or std::string(argv[i]) == "-l") {

            // Make sure a file in input
            if (i + 1 < argc) {
                filename = argv[++i];
                mode = Run::scanner;
            } else {
                std::cerr << "-l -lex option requires one argument." << std::endl;
                return -1;
            }
        } else if (std::string(argv[i]) == "-par" or std::string(argv[i]) == "-p") {

            // Make sure a file in input
            if (i + 1 < argc) {
                filename = argv[++i];
                mode = Run::parser;
            } else {
                std::cerr << "-p -par option requires one argument." << std::endl;
                return -1;
            }
        } else if (std::string(argv[i]) == "-sem" or std::string(argv[i]) == "-c") {

            // Make sure a file in input
            if (i + 1 < argc) {
                filename = argv[++i];
                mode = Run::checker;
            } else {
                std::cerr << "-c -sem option requires one argument." << std::endl;
                return -1;
            }
        } else if (std::string(argv[i]) == "-llvm" or std::string(argv[i]) == "-i") {

            // Make sure a file in input
            if (i + 1 < argc) {
                filename = argv[++i];
                mode = Run::generator;
            } else {
                std::cerr << "-i -llvm option requires one argument." << std::endl;
                return -1;
            }
        } else if (std::string(argv[i]) == "-h" || std::string(argv[i]) == "--help") {
            display_help();
            return 0;

        } else {
            filename = argv[1];
            mode = Run::executable;
        }
    }

    // Check the filename extension
    if(filename.size() < EXT_LEN || filename.substr(filename.size() - EXT_LEN).compare(EXT) != 0) {
        std::cerr << "Input file must be of '.vsop' extension." << std::endl;
        return -1;
    }

    std::vector<Token> token_vector;
    std::vector<Error> error_vector;

    Error::set_filename(filename);

    // Create a scanner and read tokens
    Scanner scanner(filename);
    if(scanner.scan() == -2) { // File error
        return -2;
    } 

    token_vector = scanner.get_tokens();
    error_vector = scanner.get_errors();

    // Display the token list if scanner mode
    if(mode == Run::scanner) {
        for (auto it : token_vector) {
            std::string str = it.get_formatted_string();
            if (!str.empty()) {
                std::cout << str << std::endl;
            }
        }

        if(error_vector.empty()) { // Exit 0 if no error occured
            return 0;
        }
    }

    // Checks for syntax error before further continuing
    if(!error_vector.empty()) {
        // Display the errors
        for(auto it : error_vector) {
            it.print_error_msg();
        }
        return -1;
    }

    // Create a Bison parser that will create the AST
    yy::Parser Parser(scanner);
    Parser.parse();

    Node ast = Parser.get_ast();
    error_vector = Parser.get_errors();

    // Display the tree if parser mode
    if(mode == Run::parser) {
        ast.print();

        if(error_vector.empty()) { // Exit 0 if no error occured
            return 0;
        }
    }

    // If error, display the errors
    if(!error_vector.empty()) {
        for (auto &i : error_vector) {
            i.print_error_msg();
        }
        return -1;
    }

    // Create a checker that will create the extanded tree
    Checker checker(ast);
    checker.check();

    Node expanded = checker.get_expanded_AST();
    error_vector = checker.get_errors();

    // Display the expanded tree if parser mode
    if(mode == Run::checker) {
        expanded.print(true);

        if(error_vector.empty()) { // Exit 0 if no error occured
            return 0;
        }
    }

    // If error, display the errors
    if(!error_vector.empty()) {
        for (auto &i : error_vector) {
            i.print_error_msg();
        }
        return -1;
    }

    // LLVM code generation
    Generator llvm_generator(expanded, filename);
    llvm_generator.generate();

    if(mode == Run::generator) {
        // Print on std output stream
        llvm_generator.print();
        return 0;
    } 
    
    // Save LLVM IR in 'filename.ll'
    std::string exec = filename.substr(0, filename.length() - EXT_LEN);
    std::string ll_f = std::string(exec).append(".ll");

    int ret = llvm_generator.save(ll_f);
    if(ret != 0) {
        std::cerr << "Error while writing in " + ll_f << std::endl;
        return -1;
    }

    // Build the executable in 'filename'
    const std::string command = std::string(COMPILER) + " " + OPTIONS + " " + exec + " " + OBJECT_PATH + " " + ll_f;
    std::system(command.c_str());

    return 0;
}

void display_help() {
    std::cout << "This program is the beginning of a long road to build a compiler..." << std::endl;
    std::cout << "Here are the option:" << std::endl;
    std::cout << "\t-lex | -l <path-to-file>\n\t\tParse the file and display the Tokens." << std::endl;
    std::cout << "\t-par | -p <path-to-file>\n\t\tParse the file and display an AST." << std::endl;
    std::cout << "\t-sem | -c <path-to-file>\n\t\tParse the parsing AST and display an expanded AST." << std::endl;
    std::cout << "\t-llvm| -i <path-to-file>\n\t\tGenerate LLVM IR code and display." << std::endl;
    std::cout << "\t<path-to-file>          \n\t\tGenerate an executable." << std::endl;
    std::cout << "\tErrors are displayed onto the standard error stream." << std::endl;
    std::cout << "\n\t-h --help          \tRecursion." << std::endl;
}
