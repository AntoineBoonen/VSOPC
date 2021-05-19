/*
 * error.cpp
 *
 * by Antoine Boonen
 *
 * This file contains the implementation of the Error class as described in the interface 'error.h'.
 *
 * Created   07/04/21
 * Modified  16/04/21
 */
#include "error.hpp"

std::string Error::filename; // Static definition

Error::Error(ErrorType t, int l, int c, std::string m) {
    type = t;
    line = l;
    column = c;
    message = m;
}

void Error::set_filename(const std::string &f) {
    filename = f;
}

bool Error::compare(Error e1, Error e2) {
    if(e1.line != e2.line) {
        return (e1.line < e2.line);
    }
    return (e1.column < e2.column);
}

void Error::print_error_msg() {
    std::cerr << filename << ":" << line << ":" << column << ":";
    switch(type) {
        case ErrorType::lexical:
            std::cerr << " lexical error:";
            break;
        case ErrorType::syntaxical:
            std::cerr << " syntax error:";
            break;
        case ErrorType::semantical:
            std::cerr << " semantic error:";
            break;
        default: // Should not happen
            std::cerr << " other error:";
            break;
    }

    std::cerr << "\n\t" << message << std::endl;
}
