/*
 * node.cpp
 *
 * by Antoine Boonen
 *
 * This file contains the implementation of the Node class as described in the interface 'node.h'.
 *
 * Created   15/03/21
 * Modified  20/03/21
 */
#include "node.hpp"
#include "symbol_table.hpp"

#include <iostream>
#include <string>

Node::Node() {
	type = NodeType::none;
}

Node::Node(NodeType t, int l, int c) {
	type = t;
	line = l;
	column = c;
}

Node Node::create_program(int line, int column, std::vector<Node> clazzes) {
	Node n = Node(NodeType::program, line, column);

	n.children[NodeType::clazz] = clazzes;

	return n;
}

Node Node::create_clazz(int line, int column, std::string id, std::string parent, std::vector<Node> fields, std::vector<Node> methods) {
	Node n = Node(NodeType::clazz, line, column);
	
	n.data[DataType::id] = id;
	n.data[DataType::parent_id] = parent;

	n.children[NodeType::field] = fields;
	n.children[NodeType::method] = methods;

	return n;
}

Node Node::create_field(int line, int column, std::string id, std::string type, Node expr) {
	Node n = Node(NodeType::field, line, column);

	n.data[DataType::id] = id;
	n.data[DataType::type] = type;

	if(!expr.is_empty()) {
		n.children[NodeType::any_expr] = {expr};
	} 
	return n;
}

Node Node::create_method(int line, int column, std::string id, std::vector<Node> formals, std::string type, Node block) {
	Node n = Node(NodeType::method, line, column);

	n.data[DataType::id] = id;
	n.data[DataType::type] = type;

	n.children[NodeType::formal] = formals;
	n.children[NodeType::block] = {block};

	return n;
}

Node Node::create_formal(int line, int column, std::string id, std::string type) {
	Node n = Node(NodeType::formal, line, column);

	n.data[DataType::id] = id;
	n.data[DataType::type] = type;

	return n;
}

Node Node::create_block(int line, int column, std::vector<Node> exprs) {
	Node n = Node(NodeType::block, line, column);

	n.children[NodeType::any_expr] = exprs;

	return n;
}

Node Node::create_if_expr(int line, int column, Node if_expr, Node then_expr, Node else_expr) {
	Node n = Node(NodeType::if_expr, line, column);

	n.children[NodeType::if_statement] = {if_expr};
	n.children[NodeType::then_statement] = {then_expr};
	if(!else_expr.is_empty()) {
		n.children[NodeType::else_statement] = {else_expr};
	} 
	return n;
}

Node Node::create_while_expr(int line, int column, Node while_expr, Node do_expr) {
	Node n = Node(NodeType::while_expr, line, column);

	n.children[NodeType::while_statement] = {while_expr};
	n.children[NodeType::do_statement] = {do_expr};

	return n;
}

Node Node::create_let_expr(int line, int column, std::string id, std::string type, Node scope_expr, Node init_expr) {
	Node n = Node(NodeType::let_expr, line, column);

	n.children[NodeType::object_identifier] = {Node::create_object_identifier(line, column, id, type)};
	n.children[NodeType::scope_statement] = {scope_expr};
	if(!init_expr.is_empty()) {
		n.children[NodeType::init_statement] = {init_expr};
	}
	return n;
}

Node Node::create_assign_expr(int line, int column, std::string id, Node expr) {
	Node n = Node(NodeType::assign_expr, line, column);

	n.data[DataType::id] = id;
	n.children[NodeType::any_expr] = {expr};

	return n;
}

Node Node::create_unop_expr(int line, int column, std::string op, Node expr) {
	Node n = Node(NodeType::unop_expr, line, column);

	n.data[DataType::op] = op;
	n.children[NodeType::any_expr] = {expr};

	return n;
}

Node Node::create_binop_expr(int line, int column, std::string op, Node left_expr, Node right_expr) {
	Node n = Node(NodeType::binop_expr, line, column);

	n.data[DataType::op] = op;
	n.children[NodeType::left_statement] = {left_expr};
	n.children[NodeType::right_statement] = {right_expr};

	return n;
}

Node Node::create_call_expr(int line, int column, Node parent_expr, std::string id, std::vector<Node> args) {
	Node n = Node(NodeType::call_expr, line, column);

	n.data[DataType::id] = id;
	n.children[NodeType::parent_statement] = {parent_expr};
	n.children[NodeType::args] = args;

	return n;
}

Node Node::create_new_expr(int line, int column, std::string id) {
	Node n = Node(NodeType::new_expr, line, column);

	n.data[DataType::id] = id;
	return n;
}

Node Node::create_int32(int line, int column, std::string value) {
	Node n = Node(NodeType::literal_int32, line, column);

	n.data[DataType::literal_value] = value;
	n.data[DataType::type] = "int32";
	return n;
}

Node Node::create_bool(int line, int column, std::string value) {
	Node n = Node(NodeType::literal_bool, line, column);

	n.data[DataType::literal_value] = value;
	n.data[DataType::type] = "bool";
	return n;
}

Node Node::create_string(int line, int column, std::string value) {
	Node n = Node(NodeType::literal_string, line, column);

	n.data[DataType::literal_value] = value;
	n.data[DataType::type] = "string";
	return n;
}

Node Node::create_unit(int line, int column) {
	Node n = Node(NodeType::literal_unit, line, column);

	n.data[DataType::literal_value] = "()";
	n.data[DataType::type] = "unit";
	return n;
}

Node Node::create_object_identifier(int line, int column, std::string value, std::string type) {
	Node n = Node(NodeType::object_identifier, line, column);

	n.data[DataType::literal_value] = value;
	n.data[DataType::type] = type;
	return n;
}

void Node::add_clazz(Node &c) {
	if(c.type != NodeType::clazz) {
		return;
	}
	children[NodeType::clazz].push_back(c);
}

std::vector<Node> Node::get_children(NodeType t) {
	return children[t];
}

std::string Node::get_data(DataType t) {
	return data[t];
}

int Node::get_line() {
	return line;
}

int Node::get_column() {
	return column;
}

NodeType Node::get_type() {
	return type;
}

void Node::set_children(NodeType t, std::vector<Node> v) {
	children[t] = v;
}

void Node::set_return_type(std::string &t) {
	data[DataType::type] = t;
}

bool Node::is_empty() {
	return (type == NodeType::none);
}

void Node::print(bool expanded) {
	switch (type) {
		case NodeType::program:
			std::cout << "[";
			for (auto i = children[NodeType::clazz].begin(); i != children[NodeType::clazz].end(); ++i) {
				i->print(expanded);
				if(i != children[NodeType::clazz].end() - 1) {
					std::cout << "," << std::endl;
				}
			}
			std::cout << "]" << std::endl;
			break;

		case NodeType::clazz:
			std::cout << "Class(" << data[DataType::id] << ", " << data[DataType::parent_id] << ",\n\t[";
			
			for (auto i = children[NodeType::field].begin(); i != children[NodeType::field].end(); ++i) {
				i->print(expanded);
				if(i != children[NodeType::field].end() - 1) {
					std::cout << ",\n\t";
				}
			}
			std::cout << "], \n\t[";
			for (auto i = children[NodeType::method].begin(); i != children[NodeType::method].end(); ++i) {
				i->print(expanded);
				if(i != children[NodeType::method].end() - 1) {
					std::cout << ",\n\t";
				}
			}
			std::cout << "]\n      )";
			break;

		case NodeType::field:
			std::cout << "Field(" << data[DataType::id] << ", " << data[DataType::type];
			if(!children[NodeType::any_expr].empty()) {
				std::cout << ", ";
				children[NodeType::any_expr].begin()->print(expanded);
			}
			std::cout << ")";
			break;

		case NodeType::method:
			std::cout << "Method(" << data[DataType::id] << ", [";
			for (auto i = children[NodeType::formal].begin(); i != children[NodeType::formal].end(); ++i) {
				i->print(expanded);
				if(i != children[NodeType::formal].end() - 1) {
					std::cout << ", ";
				}
			}
			std::cout << "]" << ", " << data[DataType::type] << ", ";
			children[NodeType::block].begin()->print(expanded);
			std::cout << ")"; 
			break;

		case NodeType::formal:
			std::cout << data[DataType::id] << " : " << data[DataType::type];
			break;

		case NodeType::block:
			std::cout << "[";
			for (auto i = children[NodeType::any_expr].begin(); i != children[NodeType::any_expr].end(); ++i) {
				i->print(expanded);
				if(i != children[NodeType::any_expr].end() - 1) {
					std::cout << ", ";
				}
			}
			std::cout << "]";

			if(expanded && !data[DataType::type].empty()) {
				std::cout << " : " << data[DataType::type];
			}
			break;

		case NodeType::if_expr:
			std::cout << "If(";
			children[NodeType::if_statement].begin()->print(expanded);
			std::cout << ", ";
			children[NodeType::then_statement].begin()->print(expanded);
			if(!children[NodeType::else_statement].empty()) {
				std::cout << ", ";
				children[NodeType::else_statement].begin()->print(expanded);
			}
			std::cout << ")";
			
			if(expanded && !data[DataType::type].empty()) {
				std::cout << " : " << data[DataType::type];
			}
			break;

		case NodeType::while_expr:
			std::cout << "While(";
			children[NodeType::while_statement].begin()->print(expanded);
			std::cout << ", ";
			children[NodeType::do_statement].begin()->print(expanded);
			std::cout << ")";

			if(expanded && !data[DataType::type].empty()) {
				std::cout << " : " << data[DataType::type];
			}
			break;

		case NodeType::let_expr:
			std::cout << "Let(" << children[NodeType::object_identifier].begin()->data[DataType::literal_value]
				<< ", " << children[NodeType::object_identifier].begin()->data[DataType::type] << ", ";
			if(!children[NodeType::init_statement].empty()) {
				children[NodeType::init_statement].begin()->print(expanded);
				std::cout << ", ";
			} 
			children[NodeType::scope_statement].begin()->print(expanded);
			std::cout << ")";

			if(expanded) {
				std::cout << " : " << data[DataType::type];
			}
			break;

		case NodeType::assign_expr:
			std::cout << "Assign(" << data[DataType::id] << ", ";
			children[NodeType::any_expr].begin()->print(expanded);
			std::cout << ")";

			if(expanded && !data[DataType::type].empty()) {
				std::cout << " : " << data[DataType::type];
			}
			break;

		case NodeType::unop_expr:
			std::cout << "UnOp(" << data[DataType::op] << ", ";
			children[NodeType::any_expr].begin()->print(expanded);
			std::cout << ")";

			if(expanded && !data[DataType::type].empty()) {
				std::cout << " : " << data[DataType::type];
			}
			break;

		case NodeType::binop_expr:
			std::cout << "BinOp(" << data[DataType::op] << ", ";
			children[NodeType::left_statement].begin()->print(expanded);
			std::cout << ", ";
			children[NodeType::right_statement].begin()->print(expanded);
			std::cout << ")";

			if(expanded && !data[DataType::type].empty()) {
				std::cout << " : " << data[DataType::type];
			}
			break;

		case NodeType::call_expr:
			std::cout << "Call(";

			children[NodeType::parent_statement].begin()->print(expanded);
			//std::cout << " : " << parent_node.get_data(DataType::type);

			std::cout << ", " << data[DataType::id] << ", [";

			for(auto i = children[NodeType::args].begin(); i != children[NodeType::args].end(); ++i) {
				i->print(expanded);
				if(i != children[NodeType::args].end() - 1) {
					std::cout << ", ";
				}
			}
			std::cout << "])";

			if(expanded && !data[DataType::type].empty()) {
				std::cout << " : " << data[DataType::type];
			}
			break;

		case NodeType::new_expr:
			std::cout << "New(" << data[DataType::id] << ")";

			if(expanded && !data[DataType::type].empty()) {
				std::cout << " : " << data[DataType::type];
			}
			break;

		default:
			std::cout << data[DataType::literal_value];

			if(expanded && !data[DataType::type].empty()) {
				std::cout << " : " << data[DataType::type];
			}
			break;
	}
}
