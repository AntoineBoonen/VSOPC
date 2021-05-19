/*
 * checker.cpp
 *
 * by Antoine Boonen
 *
 * This file contains the implementation of the sementic Checker class as described in the interface 'checker.h'.
 *
 * Created   08/04/21
 * Modified  18/05/21
 */
#include "checker.hpp"
#include "symbol_table.hpp"

#include <iostream>
#include <algorithm>
#include <string>

Checker::Checker(Node ast) {
    expAST = ast;
}

Node Checker::get_expanded_AST() {
    return expAST;
}

std::vector<Error> Checker::get_errors() {
    std::sort(error_vector.begin(), error_vector.end(), Error::compare);
    return error_vector;
}

int Checker::check() {
    SymbolTable* s = SymbolTable::getInstance();

    // Populate the symbol table with clazz definition
    for(auto &clazz : expAST.get_children(NodeType::clazz)) {
        Node ret = s->add_clazz_to_definition(clazz);
        if(!ret.is_empty()) {
            error_vector.emplace_back(ErrorType::semantical, clazz.get_line(), 
                clazz.get_column(), clazz.get_data(DataType::id) + " has already be defined at " 
                + std::to_string(ret.get_line()) + ":" + std::to_string(ret.get_column())); 
        }
    }

    // check for cycles
    if(s->cyclic_clazz_definition() != 0) {
        for(auto &node : s->get_cycle()) {
            // report error for all node referencing a cycle
            error_vector.emplace_back(ErrorType::semantical, node.get_line(), node.get_column(), 
                "Class cannot extend " + node.get_data(DataType::parent_id) + " because it references a cycle");
        }
    }

    // Check for undefined class inheritance 
    std::vector<Node> undef_parent = s->get_undefined_parents();
    for(auto &clazz : undef_parent) {
        error_vector.emplace_back(ErrorType::semantical, clazz.get_line(), clazz.get_column(),
            clazz.get_data(DataType::id) + " attempts to inherits an undefined class: " + clazz.get_data(DataType::parent_id));
    }

    // Build symbol table with fileds and methods
    build();

    // Main class, main() method check
    Node main = s->get_main_method();
    if(main.is_empty()) {
        error_vector.emplace_back(ErrorType::semantical, 1, 1, "Class 'Main' should be defined");
    } else {
        std::vector<Node> formals = main.get_children(NodeType::formal);
        if(!formals.empty()) {
            error_vector.emplace_back(ErrorType::semantical, formals.begin()->get_line(), formals.begin()->get_column(),
                "Method 'main' should not contain any formal");
        }

        if(main.get_data(DataType::type).compare("int32") != 0) {
            error_vector.emplace_back(ErrorType::semantical, main.get_line(), main.get_column(), 
                "Method 'main' should return with type 'int32'");
        }
    }

    // Check dynamic types corresponds to static types and checks function/variable used have been defined
    expAST = check_typecast(expAST);

    return error_vector.empty() ? 0 : -1;
}

void Checker::build() {
    SymbolTable* s = SymbolTable::getInstance();

    // Build a symbol table for each clazz individually
    for(auto &clazz : s->get_defined_clazzes()) {

        // Get class scope and add to class definition
        std::string scope = clazz.get_data(DataType::id);

        for(auto &field : clazz.get_children(NodeType::field)) {
            Node ret = s->add_field_to_clazz(scope, field);
            if(!ret.is_empty()) { // If field is redefined within clazz
                error_vector.emplace_back(ErrorType::semantical, clazz.get_line(), clazz.get_column(), 
                    field.get_data(DataType::id) + " has already be defined at " + std::to_string(ret.get_line()) + 
                    ":" + std::to_string(ret.get_column()));
            } 
        }

        for(auto &method : clazz.get_children(NodeType::method)) {
            Node ret = s->add_method_to_clazz(scope, method);
            if(!ret.is_empty()) { // If method is redefined within clazz
                error_vector.emplace_back(ErrorType::semantical, clazz.get_line(), clazz.get_column(), 
                    method.get_data(DataType::id) + " has already be defined at " + std::to_string(ret.get_line()) + 
                    ":" + std::to_string(ret.get_column()));
            }
        }
    }

    // check inherited methods and fields redefinition
    for(auto &clazz : s->get_defined_clazzes()) {
        std::string parent = clazz.get_data(DataType::parent_id);

        for(auto &field : clazz.get_children(NodeType::field)) {
            Node ret = s->find_field(parent, field.get_data(DataType::id));
            if(!ret.is_empty()) {
                error_vector.emplace_back(ErrorType::semantical, field.get_line(), field.get_column(), 
                    field.get_data(DataType::id) + " cannot be redefined, defined first at " + std::to_string(ret.get_line()) + 
                    ":" + std::to_string(ret.get_column()));
            }
        }

        for(auto &method : clazz.get_children(NodeType::method)) {
            Node ret = s->find_method(parent, method.get_data(DataType::id));
            if(!ret.is_empty()) {
        
                if(ret.get_data(DataType::type).compare(method.get_data(DataType::type)) != 0) {
                    // check if return type is the same
                    error_vector.emplace_back(ErrorType::semantical, method.get_line(), method.get_column(), 
                        method.get_data(DataType::id) + " cannot be redefined, defined first at " + std::to_string(ret.get_line()) + 
                        ":" + std::to_string(ret.get_column()));

                } else {
                    // check if formals are the same
                    if(ret.get_children(NodeType::formal).size() != method.get_children(NodeType::formal).size()) {
                        error_vector.emplace_back(ErrorType::semantical, method.get_line(), method.get_column(), 
                            method.get_data(DataType::id) + " cannot be redefined, defined first at " + std::to_string(ret.get_line()) + 
                            ":" + std::to_string(ret.get_column()));
                    } else {
                        for(size_t i = 0; i < ret.get_children(NodeType::formal).size(); ++i) {
                            if(!(ret.get_children(NodeType::formal)[i].get_data(DataType::id).compare(
                                method.get_children(NodeType::formal)[i].get_data(DataType::id)) == 0 &&
                                ret.get_children(NodeType::formal)[i].get_data(DataType::type).compare(
                                method.get_children(NodeType::formal)[i].get_data(DataType::type)) == 0)) {

                                    error_vector.emplace_back(ErrorType::semantical, method.get_line(), method.get_column(), 
                                        method.get_data(DataType::id) + " cannot be redefined, defined first at " + std::to_string(ret.get_line()) + 
                                        ":" + std::to_string(ret.get_column()));
                                }
                        }
                    }
                }
            }
        }
    }
}

Node Checker::check_typecast(Node &n, std::map<std::string, std::string> local_var, std::string cur_clazz, bool inst) {
    std::string ret_type, d_type, d_type2;
    Node node, expr, expr2;
    std::vector<Node> vec_node = std::vector<Node>();
    SymbolTable *s = SymbolTable::getInstance();

    switch (n.get_type()) {
        case NodeType::program:
            for(auto &clazz : n.get_children(NodeType::clazz)) {
                cur_clazz = clazz.get_data(DataType::id);
                node = check_typecast(clazz, local_var, cur_clazz, inst);

                vec_node.push_back(node);
            }
            n.set_children(NodeType::clazz, vec_node);

            ret_type = std::string("int32");
            n.set_return_type(ret_type);
            return n;

        case NodeType::clazz:
            for(auto &field : n.get_children(NodeType::field)) {
                node = check_typecast(field, local_var, cur_clazz, true);
                vec_node.push_back(node);
            }
            n.set_children(NodeType::field, vec_node);
            vec_node.clear();

            for(auto &method : n.get_children(NodeType::method)) {
                node = check_typecast(method, local_var, cur_clazz, false);
                vec_node.push_back(node);
            }
            n.set_children(NodeType::method, vec_node);

            ret_type = n.get_data(DataType::id);
            n.set_return_type(ret_type);
            s->update_clazz(n);
            return n;

        case NodeType::field:
            ret_type = n.get_data(DataType::type);

            // check user input type is legit
            if(ret_type.compare("int32") != 0 && ret_type.compare("string") != 0 && ret_type.compare("unit") != 0 &&
                ret_type.compare("bool") != 0 && !s->is_defined_clazz(ret_type)) {
                    error_vector.emplace_back(ErrorType::semantical, n.get_line(), n.get_column(), 
                        ret_type + " is undefined");

                    ret_type = std::string("unit");
            }

            // Check if initialization block return type matches the expected type 
            if(!n.get_children(NodeType::any_expr).empty()) {
                expr = *n.get_children(NodeType::any_expr).begin();
                node = check_typecast(expr, local_var, cur_clazz, true);
                n.set_children(NodeType::any_expr, {node});
                d_type = node.get_data(DataType::type);

                if(d_type.compare(ret_type) != 0 && !s->is_parent_of_child(ret_type, d_type)) {
                    error_vector.emplace_back(ErrorType::semantical, expr.get_line(), expr.get_column(), 
                        "Field is type " + ret_type + " but initializer is type " + d_type);
                }
            } 
            
            n.set_return_type(ret_type);
            s->update_field(cur_clazz, n);
            return n;

        case NodeType::method:
            ret_type = n.get_data(DataType::type);

            // check if user input type is legit
            if(ret_type.compare("int32") != 0 && ret_type.compare("string") != 0 && ret_type.compare("unit") != 0 &&
                ret_type.compare("bool") != 0 && !s->is_defined_clazz(ret_type)) {
                    error_vector.emplace_back(ErrorType::semantical, n.get_line(), n.get_column(), 
                        ret_type + " is undefined");

                    ret_type = std::string("unit");
            }

            expr = *n.get_children(NodeType::block).begin();

            // Check if formals have correct type and are not redefined
            for(auto &formal : n.get_children(NodeType::formal)) {
                auto form = local_var[formal.get_data(DataType::id)];

                if(!form.empty()) {
                    error_vector.emplace_back(ErrorType::semantical, formal.get_line(), formal.get_column(), 
                        formal.get_data(DataType::id) + " has already been defined.");
                } else {
                    auto type = formal.get_data(DataType::type);

                    if(type.compare("int32") != 0 && type.compare("string") != 0 && type.compare("unit") != 0 &&
                        type.compare("bool") != 0 && !s->is_defined_clazz(type)) {
                            error_vector.emplace_back(ErrorType::semantical, formal.get_line(), formal.get_column(), 
                                ret_type + " is undefined");
                    } else {
                        local_var[formal.get_data(DataType::id)] = formal.get_data(DataType::type);
                    }
                }
            }

            node = check_typecast(expr, local_var, cur_clazz, inst);
            n.set_children(NodeType::block, {node});
            d_type = node.get_data(DataType::type);

            if(d_type.compare(ret_type) != 0 && !s->is_parent_of_child(ret_type, d_type)) {
                error_vector.emplace_back(ErrorType::semantical, expr.get_children(NodeType::any_expr).back().get_line(), 
                expr.get_children(NodeType::any_expr).back().get_column(), "Method is type " + ret_type + " but block is type " + d_type);
            }
            n.set_return_type(ret_type);
            s->update_method(cur_clazz, n);
            return n;
            
        case NodeType::block:

            for(auto &e : n.get_children(NodeType::any_expr)) {
                node = check_typecast(e, local_var, cur_clazz, inst);
                vec_node.push_back(node);
            }
            n.set_children(NodeType::any_expr, vec_node);
            d_type = n.get_children(NodeType::any_expr).back().get_data(DataType::type);
            n.set_return_type(d_type);
            return n;

        case NodeType::if_expr:
            expr = *n.get_children(NodeType::if_statement).begin();
            node = check_typecast(expr, local_var, cur_clazz, inst);
            n.set_children(NodeType::if_statement, {node});
            d_type = node.get_data(DataType::type);

            if(d_type.compare("bool") != 0) {
                error_vector.emplace_back(ErrorType::semantical, expr.get_line(), expr.get_column(), 
                    "Condition is type " + d_type + " but should be type bool");
            }
            expr = *n.get_children(NodeType::then_statement).begin();
            node = check_typecast(expr, local_var, cur_clazz,inst);
            n.set_children(NodeType::then_statement, {node});
            d_type = node.get_data(DataType::type);

            if(!n.get_children(NodeType::else_statement).empty()) {
                expr2 = *n.get_children(NodeType::else_statement).begin();
                node = check_typecast(expr2, local_var, cur_clazz, inst);
                n.set_children(NodeType::else_statement, {node});
                d_type2 = node.get_data(DataType::type);
            } else {
                d_type2 = std::string("unit");
            }

            if(s->is_defined_clazz(d_type) && s->is_defined_clazz(d_type2)) {
                // Both branches are of type class 

                    auto ancestor = s->find_common_ancestor(d_type, d_type2);
                    if(ancestor.empty()) {
                        error_vector.emplace_back(ErrorType::semantical, expr.get_line(), expr.get_column(), 
                            "Expressions do not possess a common ancestor.");
                        d_type = std::string("unit");

                    } else {
                        d_type = ancestor;
                    }
                    n.set_return_type(d_type);
                    return n;
            } 
            
            // One of the two branch is unit
            if(d_type.compare("unit") == 0 or d_type2.compare("unit") == 0) {
                std::string type = std::string("unit");
                n.set_return_type(type);
                return n;
            }
            
            // The types don't match
            if(d_type.compare(d_type2) != 0) { 
                error_vector.emplace_back(ErrorType::semantical, expr2.get_line(), expr2.get_column(), 
                    "Expressions should return the same type but one returns " + d_type + "and the other " + d_type2);
            }
            n.set_return_type(d_type);

            return n;

        case NodeType::while_expr:
            expr = *n.get_children(NodeType::while_statement).begin();
            node = check_typecast(expr, local_var, cur_clazz, inst);
            n.set_children(NodeType::while_statement, {node});
            d_type = node.get_data(DataType::type);

            if(d_type.compare("bool") != 0) {
                error_vector.emplace_back(ErrorType::semantical, expr.get_line(), expr.get_column(), 
                    "Condition is type '" + d_type + "' but should be type bool");
            }

            expr = *n.get_children(NodeType::do_statement).begin();
            node = check_typecast(expr, local_var, cur_clazz, inst);
            n.set_children(NodeType::do_statement, {node});

            ret_type = std::string("unit");
            n.set_return_type(ret_type);
            return n;

        case NodeType::let_expr:
            expr = *n.get_children(NodeType::object_identifier).begin();
            if(expr.get_data(DataType::literal_value).compare("self") == 0) {
                error_vector.emplace_back(ErrorType::semantical, expr.get_line(), expr.get_column(), 
                    "Self cannot be bound identifier.");
            }
            
            expr = *n.get_children(NodeType::object_identifier).begin();
            ret_type = expr.get_data(DataType::type);

            // Check if user input type is legit
            if(ret_type.compare("int32") != 0 && ret_type.compare("string") != 0 && ret_type.compare("unit") != 0 &&
                ret_type.compare("bool") != 0 && !s->is_defined_clazz(ret_type)) {
                    error_vector.emplace_back(ErrorType::semantical, expr.get_line(), expr.get_column(), 
                        ret_type + "is undefined");

                    ret_type = std::string("unit");
            }

            // Check if the initilize statement type matches the expected one
            if(!n.get_children(NodeType::init_statement).empty()) {
                expr2 = *n.get_children(NodeType::init_statement).begin();
                node = check_typecast(expr2, local_var, cur_clazz, inst);
                n.set_children(NodeType::init_statement, {node});
                d_type = node.get_data(DataType::type);

                if(d_type.compare(ret_type) != 0 && !s->is_parent_of_child(ret_type, d_type)) {
                    error_vector.emplace_back(ErrorType::semantical, expr2.get_line(), expr2.get_column(), 
                        "Initializer is type " + d_type + " but should be type " + ret_type);
                }
            } 

            // Add to the local variable map
            local_var[expr.get_data(DataType::literal_value)] = ret_type;

            expr = *n.get_children(NodeType::scope_statement).begin();
            node = check_typecast(expr, local_var, cur_clazz, inst);
            n.set_children(NodeType::scope_statement, {node});
            d_type = node.get_data(DataType::type);

            n.set_return_type(d_type);
            return n;

        case NodeType::assign_expr:
            // Check for self assign
            if(n.get_data(DataType::id).compare("self") == 0) {
                error_vector.emplace_back(ErrorType::semantical, n.get_line(), n.get_column(), 
                    "Cannot assign to self");
            }

            // Get the object identifier expected type from local variable table
            ret_type = local_var[n.get_data(DataType::id)];
            if(ret_type.empty()) {

                // If none found, check in local and inhereted fields
                node = s->find_field(cur_clazz, n.get_data(DataType::id));
                if(!node.is_empty() && !node.get_data(DataType::type).empty()) {
                    ret_type = node.get_data(DataType::type);
                } else {
                    // If none found, error
                    error_vector.emplace_back(ErrorType::semantical, n.get_line(), n.get_column(), 
                        n.get_data(DataType::id) + "has not been defined");
                    ret_type = std::string("unit");
                }
            }
            expr = *n.get_children(NodeType::any_expr).begin();
            node = check_typecast(expr, local_var, cur_clazz, inst);
            n.set_children(NodeType::any_expr, {node});
            d_type = node.get_data(DataType::type);

            if(d_type.compare(ret_type) != 0 && !s->is_parent_of_child(ret_type, d_type)) {
                error_vector.emplace_back(ErrorType::semantical, expr.get_line(), expr.get_column(), 
                    "Expression should be of type " + ret_type + " but is of type " + d_type);
            }
            n.set_return_type(ret_type);
            return n;

        case NodeType::unop_expr:
            expr = *n.get_children(NodeType::any_expr).begin();
            node = check_typecast(expr, local_var, cur_clazz, inst);
            n.set_children(NodeType::any_expr, {node});
            d_type = node.get_data(DataType::type);

            if(n.get_data(DataType::op).compare("-") == 0) {
                if(d_type.compare("int32") != 0) {
                    error_vector.emplace_back(ErrorType::semantical, node.get_line(), node.get_column(), 
                        "Expression should be of type int32 but is of type " + d_type);
                }
                ret_type = std::string("int32");

            } else if(n.get_data(DataType::op).compare("isnull") == 0) {
                if(!s->is_defined_clazz(d_type)) { // Check if class 

                    error_vector.emplace_back(ErrorType::semantical, node.get_line(), node.get_column(), 
                        "Expression should be an class identifier but is of type " + d_type);
                }
                ret_type = std::string("bool");

            } else {
                if(d_type.compare("bool") != 0) {
                    error_vector.emplace_back(ErrorType::semantical, node.get_line(), node.get_column(), 
                        "Expression should be of type bool but is of type " + d_type);
                }
                ret_type = std::string("bool");
            }
            n.set_return_type(ret_type);
            return n;

        case NodeType::binop_expr:
            expr = *n.get_children(NodeType::left_statement).begin();
            node = check_typecast(expr, local_var, cur_clazz, inst);
            n.set_children(NodeType::left_statement, {node});
            d_type = node.get_data(DataType::type);

            expr2 = *n.get_children(NodeType::right_statement).begin();
            node = check_typecast(expr2, local_var, cur_clazz, inst);
            n.set_children(NodeType::right_statement, {node});
            d_type2 = node.get_data(DataType::type);

            if(n.get_data(DataType::op).compare("=") == 0) {
                if(d_type.compare(d_type2) != 0 && !s->is_parent_of_child(d_type, d_type2) &&
                    !s->is_parent_of_child(d_type2, d_type)) {
                    error_vector.emplace_back(ErrorType::semantical, n.get_line(), n.get_column(), 
                        "Expression should have the same type but one is " + d_type + " and the other " + d_type2);
                }
                ret_type = std::string("bool");
                n.set_return_type(ret_type);
                return n;
            }

            if(n.get_data(DataType::op).compare("and") == 0) {
                if(d_type.compare("bool") != 0) {
                    error_vector.emplace_back(ErrorType::semantical, expr.get_line(), expr.get_column(), 
                        "Expression should be of type bool but is of type " + d_type);
                }
                if(d_type2.compare("bool") != 0) {
                    error_vector.emplace_back(ErrorType::semantical, expr2.get_line(), expr2.get_column(), 
                        "Expression should be of type bool but is of type " + d_type2);
                }
                ret_type = std::string("bool");
                n.set_return_type(ret_type);
                return n;
            }

            if(d_type.compare("int32") != 0) {
                error_vector.emplace_back(ErrorType::semantical, expr.get_line(), expr.get_column(), 
                    "Expression should be of type int32 but is of type " + d_type);
            }

            if(d_type2.compare("int32") != 0) {
                error_vector.emplace_back(ErrorType::semantical, expr2.get_line(), expr2.get_column(), 
                    "Expression should be of type int32 but is of type " + d_type2);
            }

            ret_type = std::string("int32");

            if(n.get_data(DataType::op).compare("<") == 0 || n.get_data(DataType::op).compare("<=") == 0) {
                ret_type = std::string("bool");
            }
            n.set_return_type(ret_type);
            return n;

        case NodeType::call_expr:
            expr = *n.get_children(NodeType::parent_statement).begin();
            node = check_typecast(expr, local_var, cur_clazz, inst);
            n.set_children(NodeType::parent_statement, {node});

            // Check the method has been defined in scope
            node = s->find_method(node.get_data(DataType::type), n.get_data(DataType::id));
            d_type = node.get_data(DataType::type);

            if(d_type.empty()) {
                error_vector.emplace_back(ErrorType::semantical, n.get_line(), n.get_column(), 
                    n.get_data(DataType::id) + " has not been defined in scope");
                d_type = std::string("unit");
            }
            n.set_return_type(d_type);

            // Set args dynamic values
            for(auto &arg : n.get_children(NodeType::args)) {
                expr = check_typecast(arg, local_var, cur_clazz, inst);
                vec_node.push_back(expr);
            }
            n.set_children(NodeType::args, vec_node);

            // Check args/formals equity
            if(n.get_children(NodeType::args).size() != node.get_children(NodeType::formal).size()) {

                error_vector.emplace_back(ErrorType::semantical, n.get_children(NodeType::args).begin()->get_line(),
                    n.get_children(NodeType::args).begin()->get_line(), "Argument mismatch: " + n.get_data(DataType::id) 
                    + " requires " + std::to_string(node.get_children(NodeType::formal).size()) + " arguments but has " + 
                    std::to_string(n.get_children(NodeType::args).size()));
            
            } else {
                for(size_t i = 0; i < n.get_children(NodeType::args).size(); ++i) {
                    expr = vec_node[i];
                    d_type = expr.get_data(DataType::type);

                    expr2 = node.get_children(NodeType::formal)[i];
                    ret_type = expr2.get_data(DataType::type);

                    if(d_type.compare(ret_type) != 0 && !s->is_parent_of_child(ret_type, d_type)) {
                            error_vector.emplace_back(ErrorType::semantical, expr.get_line(), expr.get_column(), 
                            "Argument should be " + ret_type + " but is of type " + d_type);
                        }
                }
            }
            return n;

        case NodeType::new_expr:
            ret_type = n.get_data(DataType::id);
            if(!s->is_defined_clazz(ret_type)) {
                error_vector.emplace_back(ErrorType::semantical, n.get_line(), n.get_column(), 
                    ret_type + " is undefined");

                ret_type = std::string("unit");
            }

            n.set_return_type(ret_type);
            return n;

        case NodeType::object_identifier:
            if(inst && n.get_data(DataType::literal_value).compare("self") == 0) {
                error_vector.emplace_back(ErrorType::semantical, n.get_line(), n.get_column(), 
                    "Reference to self element in instantiation is not allowed");
            }

            // First check if object has a static type
            ret_type = n.get_data(DataType::type);
    
            if(ret_type.empty()) {
                // If none, look into the local variables
                ret_type = local_var[n.get_data(DataType::literal_value)]; 

                // If none and we are in clazz instantiation, the object identifier is not defined in scope.
                if(inst && ret_type.empty()) {
                    error_vector.emplace_back(ErrorType::semantical, n.get_line(), n.get_column(), 
                        "Reference to self element in instantiation is not allowed");
                }
                
                if(ret_type.empty()) {
                    node = s->find_field(cur_clazz, n.get_data(DataType::literal_value));
                    if(!node.is_empty() && !node.get_data(DataType::type).empty()) {
                        ret_type = node.get_data(DataType::type);

                    } else {
                        error_vector.emplace_back(ErrorType::semantical, n.get_line(), n.get_column(), 
                            n.get_data(DataType::literal_value) + " has not been defined");
                        ret_type = std::string("unit");
                    }
                }
            } else {
                if(ret_type.compare("int32") != 0 && ret_type.compare("string") != 0 && ret_type.compare("unit") != 0 &&
                    ret_type.compare("bool") != 0 && !s->is_defined_clazz(ret_type)) {
                        error_vector.emplace_back(ErrorType::semantical, n.get_line(), n.get_column(), 
                            ret_type + "is undefined");

                        ret_type = std::string("unit");
                }
            }
            n.set_return_type(ret_type);
            return n;

        default:
            ret_type = n.get_data(DataType::type);
            n.set_return_type(ret_type);
            return n;
    }
}
