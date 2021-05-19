/*
 * node.h
 *
 * by Antoine Boonen
 *
 * This file contains the interface of the Node class.
 *
 * Created   15/03/21
 * Modified  20/03/21
 */
#include <iostream>
#include <vector>
#include <map>

#ifndef VSOPCOMPILER_NODE_H
#define VSOPCOMPILER_NODE_H

enum class NodeType {
    // Main types of expression
	program, clazz, field, method, formal, block, if_expr, while_expr, let_expr, assign_expr, 
    unop_expr, binop_expr, call_expr, new_expr, literal_bool, literal_string, literal_int32, 
    literal_unit, object_identifier, none, 

    // Subtypes of expression included within the main types
    any_expr, if_statement, then_statement, else_statement, while_statement, do_statement,
    scope_statement, init_statement, left_statement, right_statement, parent_statement, args
};

enum class DataType { id, parent_id, type, op, literal_value };

class Node {
    private:
    	NodeType type; // The type of node
        int line; // The line index of the beginning of the node
        int column; // The column index of the beginning of the node
        std::map<NodeType, std::vector<Node> > children; // A map of children node           
        std::map<DataType, std::string> data; // A map of strings used to maintain data
        
    public:
        /*
         * Node constructor
         *
         * return:
         *      An empty Node instance.
         */
    	Node();

        /*
         * Node constructor
         *
         * return:
         *      An empty Node instance.
         */
    	Node(NodeType t, int l, int c);

        /*
         * create_* constructors
         *
         * input (depends on the type of node to create):
         *      line - the line index of the beginning of the node.
         *      column - the column index of the beginning of the node.
         *      children [clazzes, fields, methods, formals, exprs, args] - a vector representing the relevant children of the created node.
         *      child [expr, if_expr, then_expr, else_expr, while_expr, 
         *              then_expr, scope_expr, init_expr, left_expr, right_expr] - a node reprensenting the relevant child of the created node.
         *      data [id, parent, op, type, value] - a sctring representing the relevant data of the created node.
         *
         * return:
         *      A Node instance.
         */
        static Node create_program(int line, int column, std::vector<Node> clazzes);
        static Node create_clazz(int line, int column, std::string id, std::string parent, std::vector<Node> fields, std::vector<Node> methods);
        static Node create_field(int line, int column, std::string id, std::string type, Node expr = Node());
        static Node create_method(int line, int column, std::string id, std::vector<Node> formals, std::string type, Node block);
        static Node create_formal(int line, int column, std::string id, std::string type);
        static Node create_block(int line, int column, std::vector<Node> exprs);
        static Node create_if_expr(int line, int column, Node if_expr, Node then_expr, Node else_expr = Node());
        static Node create_while_expr(int line, int column, Node while_expr, Node do_expr);
        static Node create_let_expr(int line, int column, std::string id, std::string type, Node scope_expr, Node init_expr = Node());
        static Node create_assign_expr(int line, int column, std::string id, Node expr);
        static Node create_unop_expr(int line, int column, std::string op, Node expr);
        static Node create_binop_expr(int line, int column, std::string op, Node left_expr, Node right_expr);
        static Node create_call_expr(int line, int column, Node parent_expr, std::string id, std::vector<Node> args);
        static Node create_new_expr(int line, int column, std::string id);
        static Node create_int32(int line, int column, std::string value);
        static Node create_bool(int line, int column, std::string value);
        static Node create_string(int line, int column, std::string value);
        static Node create_unit(int line, int column);
        static Node create_object_identifier(int line, int column, std::string value, std::string type = std::string());

        /*
         * get_children
         *
         * input:
         *      t - the type of the wanted children.
         *
         * return:
         *      the vector of children of type t.
         */
        std::vector<Node> get_children(NodeType t);

        /*
         * get_data
         *
         * input:
         *      t - the type of the wanted data.
         *
         * return:
         *      the data of type t.
         */
        std::string get_data(DataType t);

        /*
         * get_line
         *
         * return:
         *      line - the line of the node.
         */
        int get_line();

        /*
         * get_column
         *
         * return:
         *      column - the column of the node.
         */
        int get_column();

        /*
         * get_type
         *
         * return:
         *      type - the type of the node.
         */
        NodeType get_type();

        /*
         * set_return_type
         *
         * input:
         *      t - a reference to the return type.
         */
        void set_return_type(std::string &t);

        /*
         * set_children
         *
         * input:
         *      t - the type of children to set.
         *      v - the vector of children to set
         */
        void set_children(NodeType t, std::vector<Node> v);

        /*
         * add_clazz
         *
         * input:
         *      c - the clazz node to add to the clazz children list.
         */
        void add_clazz(Node &c);

        /*
         * is_empty
         *
         * return:
         *      true - if the node is empty;
         *      false - otherwise.
         */
        bool is_empty();

        /*
         * is_empty
         * 
         * input:
         *      expanded - true to add the return type of the expression to the formating,
         *                 false otherwise.
         * 
         * the AST content formatted as required for this part of the project is printed.
         */
        void print(bool expanded = false);
};

#endif //VSOPCOMPILER_NODE_H
