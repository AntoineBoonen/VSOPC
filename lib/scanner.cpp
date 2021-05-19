/*
 * scanner.cpp
 *
 * by Antoine Boonen
 *
 * This file contains the implementation of the Scanner class as described in the interface 'scanner.h'.
 *
 * Created   21/02/21
 * Modified  16/04/21
 */

#include "scanner.hpp"

#include <fstream>
#include <algorithm>
#include <map>
#include <iomanip>
#include <sstream>

#define BS  0x08
#define TA  0x09
#define LF  0x0a
#define FF  0x0c
#define CR  0x0d
#define SP  0x20

static const std::vector<char> Whitespaces {
    TA,
    LF,
    FF,
    CR,
    SP,
};

static const std::vector<std::string> Keywords {
    "and",   "extends", "isnull", "string",
    "bool",  "false",   "let",    "then",
    "class", "if",      "new",    "true",
    "do",    "in",      "not",    "unit",
    "else",  "int32",   "self",   "while",
};

static const std::map<std::string, std::string> Operators {
    {"{", "lbrace"}, {",", "comma"}, {".", "dot"},
    {"}", "rbrace"}, {"+", "plus"}, {"=", "equal"},
    {"(", "lpar"}, {"-", "minus"}, {"<", "lower"},
    {")", "rpar"}, {"*", "times"}, {"<=", "lower-equal"},
    {":", "colon"}, {"/", "div"}, {"<-", "assign"},
    {";", "semicolon"}, {"^", "pow"},
};

static const std::map<std::string, char> ASCII_converter {
    {"\\n", LF}, {"\\t", TA}, {"\\b", BS}, {"\\r", CR}, {"\\\\", '\\'}, {"\\\"", '"'}
};

/*
 * format_comments
 *
 * Format a comment string according to the vsop manual.
 *
 * input:
 *      str - a reference to the string to format.
 *
 * output:
 *      formatted    - the formatted comment string.
 *      empty string - when an error occurred according to the manual rule.
 */
static std::string format_comments(const std::string &str);

/*
 * format_string
 *
 * Format a string literal according to the vsop manual.
 *
 * input:
 *      str   - a reference to the string to format.
 *      error - a reference to an Error vector.
 *      l     - the absolute line value of the beginning of the string.
 *      c     - the absolute column value of the beginning of the string.
 *
 * output:
 *      formatted    - the formatted comment string.
 *      empty string - when errors occurred according to the manual rule; all errors are pushed on 'error'.
 */
static std::string format_string(const std::string &str, std::vector<Error> &error, int l, int c);

Scanner::Scanner(const std::string &filename) {
    this->filename = filename;
    this->tokens = std::vector<Token>();
    this->errors = std::vector<Error>();
    this->token_pos = 0;
}

std::vector<Token> Scanner::get_tokens() {
    return tokens;
}

std::vector<Error> Scanner::get_errors() {
    return errors;
}

Token Scanner::get_next_token() {
    while(tokens[token_pos].get_class() == TokenClass::comments) {
        token_pos++;
    }
    if(token_pos > tokens.size()) {
        return Token();
    }
    return tokens[token_pos++];
}

int Scanner::scan() {

    // Opening file
    std::ifstream file(filename);
    if(!file.is_open()) {
        std::cerr << "Could not open the file." << std::endl;
        return -2;
    }

    char curr = LF, prev = LF;
    int ctr_line = 1, ctr_column = 0;
    bool eof = false;

    while(!eof) {

        // Reinitialize variables
        std::string _value;
        TokenClass _class = TokenClass::none;
        int _line = -1, _column = -1;
        bool correct = true;

        //Read next character
        prev = curr;
        curr = file.get();
        ++ctr_column;

        if(curr >= '0' and curr <= '9') { // Integer-literal
            bool hexa = false;
            _class = TokenClass::integer_literal;
            _line = ctr_line;
            _column = ctr_column;

            if(curr == '0') { // Possible hex value
                prev = curr;
                curr = file.get();
                ++ctr_column;

                if(curr == 'x') { // Hex value confirmed
                    hexa = true;

                    curr = file.get(); // Skip the 'x'
                    ++ctr_column;

                    // Read until the end of the value
                    while(!(file.eof() or std::find(Whitespaces.begin(), Whitespaces.end(), curr) != Whitespaces.end()
                        or Operators.find(std::string{curr}) != Operators.end())) {

                        _value.push_back(curr);
                        curr = file.get();
                        ++ctr_column;
                    }
                    file.seekg(-1, std::ios_base::cur);
                    --ctr_column;

                    // Check if the value is a correct hexadecimal number
                    size_t initial_size = _value.length();
                    size_t end;
                    std::string tmp;
                    try {
                        tmp = std::to_string(std::stoi(_value, &end, 16));

                    } catch (const std::invalid_argument &ia) { // Illegal hexadecimal number
                        errors.emplace_back(ErrorType::lexical, _line, _column,
                                                        "Illegal input as hexadecimal number: " + std::string{_value});
                        correct = false;
                        break;
                    }

                    if(end != initial_size) {
                        errors.emplace_back(ErrorType::lexical, _line, _column,
                                                        "Illegal input in hexadecimal number: " + std::string{_value});
                        correct = false;
                    }
                    _value = tmp;
                }
            }

            if(!hexa) { // Digital value confirmed

                // Read until the end of the value
                while(!(file.eof() or std::find(Whitespaces.begin(), Whitespaces.end(), curr) != Whitespaces.end()
                      or Operators.find(std::string{curr}) != Operators.end())) {

                    _value.push_back(curr);
                    curr = file.get();
                    ++ctr_column;
                }
                file.seekg(-1, std::ios_base::cur);
                --ctr_column;

                // Remove front 0s
                _value = _value.erase(0, _value.find_first_not_of('0'));
                if(_value.empty()) {
                    _value = "0";
                }

                // check if the digit is correct
                for(auto digit : _value) {
                    if(!(digit >= '0' and digit <= '9')) {
                        errors.emplace_back(ErrorType::lexical, _line, _column,
                                                        "Illegal element in number: " + std::string{digit});
                        correct = false;
                    }
                }
            }

        } else if (curr >= 'A' and curr <= 'Z') { // Type-identifier
            _class = TokenClass::type_identifier;
            _line = ctr_line;
            _column = ctr_column;

            while((curr >= 'a' and curr <= 'z') or (curr >= 'A' and curr <= 'Z') or (curr >= '0' and curr <= '9')
                or curr == '_') {

                _value.push_back(curr);
                curr = file.get();
                ++ctr_column;
            }
            file.seekg(-1, std::ios_base::cur);
            --ctr_column;

        } else if (curr >= 'a' and curr <= 'z') { // Objet-identifier or Keyword
            _line = ctr_line;
            _column = ctr_column;

            while((curr >= 'a' and curr <= 'z') or (curr >= 'A' and curr <= 'Z') or (curr >= '0' and curr <= '9')
                or curr == '_') {

                _value.push_back(curr);
                curr = file.get();
                ++ctr_column;
            }
            file.seekg(-1, std::ios_base::cur);
            --ctr_column;

            if(std::find(Keywords.begin(), Keywords.end(), _value) != Keywords.end()) { // Keyword confirmed
                _class = TokenClass::keyword;

            } else { // Object-identifier confirmed
                _class = TokenClass::object_identifier;
            }

        } else if (Operators.find(std::string{curr}) != Operators.end()) { // Operators, line comment or multiple comment
            _class = TokenClass::operators;
            _line = ctr_line;
            _column = ctr_column;

            // Check for possible '<-', '<=' or comment sequence
            prev = curr;
            curr = file.get();
            ++ctr_column;

            auto op = Operators.find(std::string{prev, curr});
            if(op != Operators.end()) { // '<-' or '<=' sequence confirmed
                _value = op->second;

            } else if (prev == '(' and curr == '*') { // Multiple-line comment confirmed
                _class = TokenClass::comments;
                _value.push_back(prev);
                _value.push_back(curr);

                int spacing = 0; // Used to avoid (*) special case
                int depth = 1;
                std::vector<int> line_offset {_line};
                std::vector<int> column_offset {_column};

                // Read until end of file or end of all nested comments
                while(!file.eof() and depth > 0) {
                    prev = curr;
                    curr = file.get();
                    ++spacing;
                    ++ctr_column;

                    if(curr == LF) {
                        ++ctr_line;
                        ctr_column = 0;
                    }
                    _value.push_back(curr);

                    if(prev == '(' and curr == '*') { // Deeper nested comment begins
                        depth += 1;
                        spacing = 0;
                        line_offset.push_back(ctr_line);
                        column_offset.push_back(ctr_column - 1);

                    } else if(prev == '*' and curr == ')' and spacing >= 2) { // Nested comment ends
                        depth -= 1;
                        line_offset.pop_back();
                        column_offset.pop_back();
                    }
                }

                // Comments check
                if(_value[0] == '(' and _value.back() == EOF) {
                    errors.emplace_back(ErrorType::lexical, line_offset.back(), column_offset.back(),
                                                    "Comments not closed before end-of-file.");
                    correct = false;
                }
                _value = format_comments(_value);

            } else if (prev == '/' and curr == '/') { // Single-line comment confirmed
                _class = TokenClass::comments;
                _value.push_back(prev);
                _value.push_back(curr);

                // Read until end of file or line feed
                while(!file.eof() and curr != LF) {
                    curr = file.get();
                    ++ctr_column;

                    _value.push_back(curr);
                }

                // Delete final EOF or LF character
                _value.erase(_value.length() - 1);
                if(curr == LF) { // New line
                    ctr_column = 0;
                    ++ctr_line;
                }

            } else { // Single byte operator
                op = Operators.find(std::string{prev});
                _value = op->second;

                file.seekg(-1, std::ios_base::cur);
                --ctr_column;
            }

        } else if (curr == '"') { // String-literals
            _class = TokenClass::string_literals;
            _line = ctr_line;
            _column = ctr_column;
            _value.push_back(curr);

            // Read until the end of the string or the end of file.
            bool end_str = false;
            while(!file.eof() and !end_str) {
                    prev = curr;
                    curr = file.get();
                    ++ctr_column;

                    // check new line
                    if(curr == LF) {
                        ++ctr_line;
                        ctr_column = 0;
                    }

                    // check for the end of the string
                    if(prev != '\\' and curr == '"') {
                        end_str = true;
                    }
                    _value.push_back(curr);
            }

            std::string tmp = format_string(_value, errors, _line, _column);
            if(!tmp.empty()) { // No error in the string
                _value = tmp;
            } else {
                correct = false;
            }

        } else if (std::find(Whitespaces.begin(), Whitespaces.end(), curr) != Whitespaces.end()) { // Whitespace
            correct = false;

            // New line
            if(curr == LF or curr == FF or curr == CR) {
                ++ctr_line;
                ctr_column = 0;
            }

        } else { // Other sequence
            _class = TokenClass::eof;
            _line = ctr_line;
            _column = ctr_column;
            correct = false;

            if(curr != EOF) { // Unknown sequence
                errors.emplace_back(ErrorType::lexical, _line, _column, "Unknown symbol: " + std::string{curr});
            } else { // End of file
                correct = true;
                _value = "EOF";
                eof = true;
            }
        }

        if(correct) {
            tokens.emplace_back(_class, _line, _column, _value);
        }
    }

    file.close();
    return (errors.empty() ? 0 : -1);
}

std::string format_comments(const std::string &str) {
    std::string formatted;
    char tmp;

    // Comment formatting
    for(auto curr : str) {
        if(std::find(Whitespaces.begin(), Whitespaces.end(), curr) != Whitespaces.end()) {
            tmp = SP;
        } else {
            tmp = curr;
        }

        if(formatted.empty() or !(formatted[formatted.length() - 1] == SP and tmp == SP)) {
            formatted.push_back(tmp);
        }
    }
    return formatted;
}

static std::string format_string(const std::string &str, std::vector<Error> &error, int l, int c) {

    // String check
    if(str.back() == EOF) {
        error.emplace_back(ErrorType::lexical, l, c, "String not closed before end-of-file.");
        return std::string();
    }

    std::string formatted;
    char prev = ' ';
    int column_offset = c;
    int line_offset = 0;
    bool is_error = false;

    for(auto curr = str.begin(); curr != str.end(); curr++) {
        ++column_offset;

        if(prev != '\\' and *curr == LF) { // Row LF confirmed
            error.emplace_back(ErrorType::lexical, l + line_offset, column_offset - 1,
                               "Row line feed is illegal in strings.");
            is_error = true;
            ++line_offset;
            column_offset = 1;

        } else if(prev == '\\' and *curr != LF and *curr != 'x') { // Deal with special escape sequence

            auto ascii = ASCII_converter.find(std::string{prev, *curr});
            if(ascii == ASCII_converter.end()) { // Unrecognized escape sequence
                error.emplace_back(ErrorType::lexical, l + line_offset, column_offset - 2,
                                   "Ascii character not accepted in strings: \\" + std::string{*curr});
                is_error = true;

            } else { // Correct escape sequence
                std::stringstream stream;
                stream << "x" << std::setw(2) << std::setfill('0') << std::hex << int(ascii->second);
                formatted.append(stream.str());
            }

            if(*curr == '\\') { // Skip to deal with special double backslash case
                curr++;
                column_offset++;
                formatted.push_back(*curr);
            }

        } else if(prev == '\\' and *curr == LF) { // Correct new line sequence
            formatted.erase(formatted.length() - 1); // Remove the backslash symbol
            ++curr; // skip the LF

            column_offset = 1;
            ++line_offset;

            // Skip all spaces and tabulations
            while(*curr == SP or *curr == TA) {
                ++curr;
                ++column_offset;
            }
            --curr;
            --column_offset;

        } else if (prev == '\\' and *curr == 'x') { // Row escape sequence confirmed
            formatted.push_back(*curr);

            // check whether the next two characters make a correct hexadecimal value
            if (str.end() - curr >= 2) {
                if (!((*(curr + 1) >= '0' and *(curr + 1) <= '9') or (*(curr + 1) >= 'A' and *(curr + 1) <= 'F')
                   or (*(curr + 1) >= 'a' and *(curr + 1) <= 'f')) or !((*(curr + 2) >= '0' and *(curr + 2) <= '9')
                   or (*(curr + 2) >= 'A' and *(curr + 2) <= 'F') or (*(curr + 2) >= 'a' and *(curr + 2) <= 'f'))) {

                    error.emplace_back(ErrorType::lexical, l + line_offset, column_offset - 2,
                                       "Illegal input as string sequence in hexadecimal value: \\x" +
                                       std::string{*(curr + 1), *(curr + 2)});
                    is_error = true;
                }
            } else {
                error.emplace_back(ErrorType::lexical, l + line_offset, column_offset - 2,
                                   "Illegal construction string sequence in hexadecimal value.");
                is_error = true;
            }

        } else { // Any other character
            formatted.push_back(*curr);
        }
        prev = *curr;
    }
    return (is_error ? std::string() : formatted);
}
