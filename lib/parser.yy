/*
 * parser.yy
 *
 * by Antoine Boonen
 *
 * This file contains the Parser configuration used by for Bison.
 *
 * Created   16/03/21
 * Modified  16/04/21
 */

/*** Bison declarations ***/
%skeleton "lalr1.cc"
%require "3.2"
%define api.parser.class { Parser }
%define parse.error verbose
%define api.token.constructor
%define api.value.type variant
%locations

%code requires {
	#include "scanner.hpp"
	#include "token.hpp"
	#include "error.hpp"
	#include "node.hpp"

	extern void push_ctx();
	extern void pop_ctx();
} // End code requires

%param {Scanner &scanner}

%code { 
	#include <iostream>
	#include <vector>

	namespace yy { 
		Parser::symbol_type yylex(Scanner &scanner);
	}

	int ctx = 0;
	Node ast;
	std::vector<Error> error_vector = std::vector<Error>();
	std::string cur_clazz = "Object";

	std::vector<std::vector<Node>> fields_vec = {std::vector<Node>()};
	std::vector<std::vector<Node>> methods_vec = {std::vector<Node>()};
	std::vector<std::vector<Node>> formals_vec = {std::vector<Node>()};
	std::vector<std::vector<Node>> block_vec = {std::vector<Node>()};
	std::vector<std::vector<Node>> args_vec = {std::vector<Node>()};
} // End code

// Tokens 
%token 			END 			0

%token <std::string> INTEGER_LIT
%token <std::string> STRING_LIT
%token <std::string> TYPE_ID
%token <std::string> OBJ_ID

%token 		   AND_KWORD 		"and"
%token         BOOL_KWORD 		"bool"
%token         CLASS_KWORD 		"class"
%token         DO_KWORD 		"do"
%token         ELSE_KWORD		"else"
%token         EXTENDS_KWORD	"extends"
%token         FALSE_KWORD		"false"
%token         IF_KWORD			"if"
%token         IN_KWORD 		"in"
%token         INT32_KWORD 		"int32"
%token         ISNULL_KWORD		"isnull"
%token         LET_KWORD		"let"
%token         NEW_KWORD		"new"
%token         NOT_KWORD		"not"
%token         SELF_KWORD 		"self"
%token         STRING_KWORD 	"string"
%token         THEN_KWORD 		"then"
%token         TRUE_KWORD 		"true"
%token         UNIT_KWORD 		"unit"
%token         WHILE_KWORD 		"while"

%token 			LBRACE			"{"
%token 			RBRACE			"}"
%token 			LPAR			"("
%token 			RPAR			")"
%token 			COLON			":"
%token 			SEMICOLON		";"
%token 			COMMA			","
%token 			PLUS 			"+"
%token 			MINUS 			"-"
%token 			TIMES			"*"
%token 			DIV				"/"
%token 			POW				"^"
%token 			DOT				"."
%token 			EQUAL			"="
%token 			LOWER			"<"
%token 			LEQ				"<="
%token 			ASSIGN			"<-"
// End of tokens

// Associative rules 
%right			"<-"			// 9
%left		    "and"			// 8
%right			"not"			// 7
%nonassoc 		"<" "<=" "=" 	// 6
%left		    "+" "-"  		// 5
%left		    "*" "/" 		// 4
%right			"isnull" UMIN  	// 3
%right			"^"				// 2
%left 			"."				// 1
// End associative rules 

// Types
%type <Node> 					clazz
%type <Node> 					field
%type <Node> 					method
%type <std::string> 			type
%type <Node> 					form
%type <Node> 					block
%type <Node> 					expr
%type <Node> 					literal
%type <std::string> 			boolean
// End of types

%start program
%% // Grammar rules

program : 		clazz { ast = Node::create_program(1, 1, {$1}); }
|			 	program clazz { ast.add_clazz($2); }

clazz : 		"class" TYPE_ID { cur_clazz = $2; } clazzbody { $$ = Node::create_clazz(@1.begin.line, @1.begin.column, $2, "Object", fields_vec[ctx], methods_vec[ctx]); fields_vec[ctx].clear(); methods_vec[ctx].clear(); }
| 				"class" TYPE_ID "extends" TYPE_ID { cur_clazz = $2; } clazzbody { $$ = Node::create_clazz(@1.begin.line, @1.begin.column, $2, $4, fields_vec[ctx], methods_vec[ctx]); fields_vec[ctx].clear(); methods_vec[ctx].clear(); }

clazzbody : 	"{" body "}"

body : 			%empty
|	 			body field { fields_vec[ctx].push_back($2); }
| 				body method { methods_vec[ctx].push_back($2); }

field : 		OBJ_ID ":" type ";" { $$ = Node::create_field(@1.begin.line, @1.begin.column, $1, $3); }
| 				OBJ_ID ":" type "<-" expr ";" { $$ = Node::create_field(@1.begin.line, @1.begin.column, $1, $3, $5); }

method :		OBJ_ID "(" formals ")" ":" type block { $$ = Node::create_method(@1.begin.line, @1.begin.column, $1, formals_vec[ctx], $6, $7); formals_vec[ctx].clear(); }

type : 			TYPE_ID { $$ = $1; }
| 				"int32" { $$ = "int32"; }
| 				"bool" { $$ = "bool"; }
| 				"string" { $$ = "string"; }
| 				"unit" { $$ = "unit"; }

formals :		%empty
|	 			formal

formal : 		form { formals_vec[ctx].push_back($1); }
| 				formal "," form { formals_vec[ctx].push_back($3); }

form : 			OBJ_ID ":" type {$$ = Node::create_formal(@1.begin.line, @1.begin.column, $1, $3); }

block : 		"{" { push_ctx(); } exprs "}" { $$ = Node::create_block(@1.begin.line, @1.begin.column, block_vec[ctx]); pop_ctx(); }

exprs : 		expr { block_vec[ctx].push_back($1); }
| 				exprs ";" expr { block_vec[ctx].push_back($3); }

expr : 			"if" expr "then" expr { $$ = Node::create_if_expr(@1.begin.line, @1.begin.column, $2, $4); }
|        		"if" expr "then" expr "else" expr { $$ = Node::create_if_expr(@1.begin.line, @1.begin.column, $2, $4, $6); }
| 				"while" expr "do" expr { $$ = Node::create_while_expr(@1.begin.line, @1.begin.column, $2, $4); }
| 				"let" OBJ_ID ":" type "in" expr { $$ = Node::create_let_expr(@1.begin.line, @1.begin.column, $2, $4, $6); }
| 				"let" OBJ_ID ":" type "<-" expr "in" expr { $$ = Node::create_let_expr(@1.begin.line, @1.begin.column, $2, $4, $8, $6); }
|				OBJ_ID "<-" expr { $$ = Node::create_assign_expr(@1.begin.line, @1.begin.column, $1, $3); }
|	 			"not" expr { $$ = Node::create_unop_expr(@1.begin.line, @1.begin.column, "not", $2); }
|%prec UMIN 	"-" expr { $$ = Node::create_unop_expr(@1.begin.line, @1.begin.column, "-", $2); }
|	 			"isnull" expr { $$ = Node::create_unop_expr(@1.begin.line, @1.begin.column, "isnull", $2); }
| 				expr "and" expr { $$ = Node::create_binop_expr(@1.begin.line, @1.begin.column, "and", $1, $3); }
| 				expr "=" expr { $$ = Node::create_binop_expr(@1.begin.line, @1.begin.column, "=", $1, $3); }
|	 			expr "<" expr { $$ = Node::create_binop_expr(@1.begin.line, @1.begin.column, "<", $1, $3); }
| 				expr "<=" expr { $$ = Node::create_binop_expr(@1.begin.line, @1.begin.column, "<=", $1, $3); }
| 				expr "+" expr { $$ = Node::create_binop_expr(@1.begin.line, @1.begin.column, "+", $1, $3); }
| 				expr "-" expr { $$ = Node::create_binop_expr(@1.begin.line, @1.begin.column, "-", $1, $3); }
|	 			expr "*" expr { $$ = Node::create_binop_expr(@1.begin.line, @1.begin.column, "*", $1, $3); }
| 				expr "/" expr { $$ = $$ = Node::create_binop_expr(@1.begin.line, @1.begin.column, "/", $1, $3); }
| 				expr "^" expr { $$ = Node::create_binop_expr(@1.begin.line, @1.begin.column, "^", $1, $3); }
|				expr error expr { $$ = Node::create_binop_expr(@1.begin.line, @1.begin.column, "ERROR", $1, $3); }
|				OBJ_ID "(" { push_ctx(); } args ")" { $$ = Node::create_call_expr(@1.begin.line, @1.begin.column, Node::create_object_identifier(@1.begin.line, @1.begin.column, "self", cur_clazz), $1, args_vec[ctx]); pop_ctx(); }
| 				expr "." OBJ_ID "(" { push_ctx(); } args ")" { $$ = Node::create_call_expr(@1.begin.line, @1.begin.column, $1, $3, args_vec[ctx]); pop_ctx(); }
| 				"new" TYPE_ID { $$ = Node::create_new_expr(@1.begin.line, @1.begin.column, $2); }
|	 			OBJ_ID { $$ = Node::create_object_identifier(@1.begin.line, @1.begin.column, $1); }
| 				"self" { $$ = Node::create_object_identifier(@1.begin.line, @1.begin.column, "self", cur_clazz); }
| 				literal { $$ = $1; }
| 				"(" ")" { $$ = Node::create_unit(@1.begin.line, @1.begin.column); }
|	 			"(" expr ")" { $$ = $2; }
| 				block { $$ = $1; }

args : 			%empty
| 				arg

arg : 			expr { args_vec[ctx].push_back($1); }
| 				arg "," expr { args_vec[ctx].push_back($3); }

literal : 		INTEGER_LIT { $$ = Node::create_int32(@1.begin.line, @1.begin.column, $1); }
| 				STRING_LIT { $$ = Node::create_string(@1.begin.line, @1.begin.column, $1); }
| 				boolean { $$ = Node::create_bool(@1.begin.line, @1.begin.column, $1); }

boolean : 		"true" { $$ = "true"; }
| 				"false" { $$ = "false"; }

%% // End grammar rules


yy::Parser::symbol_type yy::yylex(Scanner &scanner) {

	Token t = scanner.get_next_token();

	yy::position pos;
	pos.column = t.get_column();
	pos.line = t.get_line();
	yy::location loc(pos, pos);
	std::string val = t.get_value();

	switch(t.get_class()) {

		case TokenClass::integer_literal:
			return yy::Parser::make_INTEGER_LIT(val, loc);

		case TokenClass::keyword:
			if(val.compare("and") == 0) {
				return yy::Parser::make_AND_KWORD(loc);
			} else if(val.compare("bool") == 0) {
				return yy::Parser::make_BOOL_KWORD(loc);
			} else if(val.compare("class") == 0) {
				return yy::Parser::make_CLASS_KWORD(loc);
			} else if(val.compare("do") == 0) {
				return yy::Parser::make_DO_KWORD(loc);
			} else if(val.compare("else") == 0) {
				return yy::Parser::make_ELSE_KWORD(loc);
			} else if(val.compare("extends") == 0) {
				return yy::Parser::make_EXTENDS_KWORD(loc);
			} else if(val.compare("false") == 0) {
				return yy::Parser::make_FALSE_KWORD(loc);
			} else if(val.compare("if") == 0) {
				return yy::Parser::make_IF_KWORD(loc);
			} else if(val.compare("in") == 0) {
				return yy::Parser::make_IN_KWORD(loc);
			} else if(val.compare("int32") == 0) {
				return yy::Parser::make_INT32_KWORD(loc);
			} else if(val.compare("isnull") == 0) {
				return yy::Parser::make_ISNULL_KWORD(loc);
			} else if(val.compare("let") == 0) {
				return yy::Parser::make_LET_KWORD(loc);
			} else if(val.compare("new") == 0) {
				return yy::Parser::make_NEW_KWORD(loc);
			} else if(val.compare("not") == 0) {
				return yy::Parser::make_NOT_KWORD(loc);
			} else if(val.compare("self") == 0) {
				return yy::Parser::make_SELF_KWORD(loc);
			} else if(val.compare("string") == 0) {
				return yy::Parser::make_STRING_KWORD(loc);
			}	else if(val.compare("then") == 0) {
				return yy::Parser::make_THEN_KWORD(loc);
			} else if(val.compare("true") == 0) {
				return yy::Parser::make_TRUE_KWORD(loc);
			} else if(val.compare("unit") == 0) {
				return yy::Parser::make_UNIT_KWORD(loc);
			} else {
				return yy::Parser::make_WHILE_KWORD(loc);
			}

		case TokenClass::type_identifier:
			return yy::Parser::make_TYPE_ID(val, loc);

		case TokenClass::object_identifier:
			return yy::Parser::make_OBJ_ID(val, loc);

		case TokenClass::string_literals:
			return yy::Parser::make_STRING_LIT(val, loc);

		case TokenClass::operators:
			if(val.compare("lbrace") == 0) {
				return yy::Parser::make_LBRACE(loc);
			} else if(val.compare("rbrace") == 0) {
				return yy::Parser::make_RBRACE(loc);
			} else if(val.compare("lpar") == 0) {
				return yy::Parser::make_LPAR(loc);
			} else if(val.compare("rpar") == 0) {
				return yy::Parser::make_RPAR(loc);
			} else if(val.compare("colon") == 0) {
				return yy::Parser::make_COLON(loc);
			} else if(val.compare("semicolon") == 0) {
				return yy::Parser::make_SEMICOLON(loc);
			} else if(val.compare("comma") == 0) {
				return yy::Parser::make_COMMA(loc);
			} else if(val.compare("plus") == 0) {
				return yy::Parser::make_PLUS(loc);
			} else if(val.compare("minus") == 0) {
				return yy::Parser::make_MINUS(loc);
			} else if(val.compare("times") == 0) {
				return yy::Parser::make_TIMES(loc);
			} else if(val.compare("div") == 0) {
				return yy::Parser::make_DIV(loc);
			} else if(val.compare("pow") == 0) {
				return yy::Parser::make_POW(loc);
			} else if(val.compare("dot") == 0) {
				return yy::Parser::make_DOT(loc);
			} else if(val.compare("equal") == 0) {
				return yy::Parser::make_EQUAL(loc);
			} else if(val.compare("lower") == 0) {
				return yy::Parser::make_LOWER(loc);
			} else if(val.compare("lower-equal") == 0) {
				return yy::Parser::make_LEQ(loc);
			} else {
				return yy::Parser::make_ASSIGN(loc);
			}
			
		case TokenClass::eof:
		default:
			return yy::Parser::make_END(loc);
	}
}

void yy::Parser::error(const location_type &l, const std::string &m) {
	error_vector.emplace_back(ErrorType::syntaxical, l.begin.line, l.begin.column, m);
}

Node yy::Parser::get_ast() {
	return ast;
}

std::vector<Error> yy::Parser::get_errors() {
	return error_vector;
}

void push_ctx() {
	++ctx;
	methods_vec.push_back(std::vector<Node>());
	fields_vec.push_back(std::vector<Node>());
	formals_vec.push_back(std::vector<Node>());
	args_vec.push_back(std::vector<Node>());
	block_vec.push_back(std::vector<Node>());
}

void pop_ctx() {
	--ctx;
	methods_vec.pop_back();
	fields_vec.pop_back();
	formals_vec.pop_back();
	args_vec.pop_back();
	block_vec.pop_back();
}


