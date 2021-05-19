/*
 * symbol_table.cpp
 *
 * by Antoine Boonen
 *
 * This file contains the implementation of the SymbolTable as described in the interface 'symbol_table.h'.
 *
 * Created   05/04/21
 * Modified  18/05/21
 */
#include "symbol_table.hpp"

#include <algorithm>
#include <exception>
#include <iterator>
#include <string>

#define METHOD "method"
#define FIELD  "field"

SymbolTable* SymbolTable::instance = 0;

SymbolTable* SymbolTable::getInstance() {
    
    if(instance == 0) {
        instance = new SymbolTable();
    }
    return instance;
}

SymbolTable::SymbolTable() {
    // Add Object class and methods to the list of symbols
    Node object = Node::create_clazz(0, 0, "Object", "Object", {}, {
        Node::create_method(0, 0, "print", 
            {Node::create_formal(0, 0, "s", "string")}, "Object", 
            {Node::create_block(0, 0, {})}),
        Node::create_method(0, 0, "printBool", 
            {Node::create_formal(0, 0, "b", "bool")}, "Object", 
            {Node::create_block(0, 0, {})}),
        Node::create_method(0, 0, "printInt32", 
            {Node::create_formal(0, 0, "i", "int32")}, "Object", 
            {Node::create_block(0, 0, {})}),
        Node::create_method(0, 0, "inputLine", 
            {}, "string", {Node::create_block(0, 0, {})}),
        Node::create_method(0, 0, "inputBool", 
            {}, "bool", {Node::create_block(0, 0, {})}),
        Node::create_method(0, 0, "inputInt32", 
            {}, "int32", {Node::create_block(0, 0, {})})
    });
    std::string obj_type = std::string("Object");
    object.set_return_type(obj_type);

    clazz_definition["Object"] = object;
    for(auto &method : object.get_children(NodeType::method)) {
        symbol_table["Object"][METHOD][method.get_data(DataType::id)] = method;
    }
}

Node SymbolTable::add_clazz_to_definition(Node clazz) {
    std::string id = clazz.get_data(DataType::id);

    if(!clazz_definition[id].is_empty()) { // class already defined
        return clazz_definition[id];
    }

    clazz_definition[id] = clazz;

    return Node();
}

int SymbolTable::cyclic_clazz_definition() {
    clazz_in_cycle.clear();

    std::vector<std::string> to_check;
    for(auto &clazz : clazz_definition) {
        to_check.push_back(clazz.first);
    }

    for(auto &clazz : to_check) {
        std::map<std::string, bool> in_stack = std::map<std::string, bool>();

        std::string cur = clazz;
        // Loop until object (no cycle), or cur has already been iterated (cycle)
        while(cur.compare("Object") != 0 && in_stack[cur] == false/* && std::find(to_check.begin(), to_check.end(), cur) != to_check.end()*/) {

            in_stack[cur] = true;
            cur = clazz_definition[cur].get_data(DataType::parent_id);
        }

        if(cur.compare("Object") != 0 && in_stack[cur] == true) {
            // Cycle is present
            for(auto &node : in_stack) {
                clazz_in_cycle.push_back(clazz_definition[node.first]);
                to_check.erase(std::remove(to_check.begin(), to_check.end(), node.first), to_check.end());
            }
        } 
    }
    return clazz_in_cycle.empty() ? 0 : -1;
}

std::vector<Node> SymbolTable::get_undefined_parents() {
    std::vector<Node> undef;

    for(auto &clazz : clazz_definition) {
        std::string parent_clazz = clazz.second.get_data(DataType::parent_id);
        if(clazz_definition[parent_clazz].is_empty()) {
            undef.push_back(clazz.second);
        }
    }
    return undef;
} 

std::vector<Node> SymbolTable::get_cycle() {
    return clazz_in_cycle;
}

std::vector<Node> SymbolTable::get_defined_clazzes(bool with_Object) {
    std::vector<Node> clazzes;
    for(auto &clazz : clazz_definition) {
        if(clazz.first.compare("Object") != 0) {
            clazzes.emplace_back(clazz.second);
        }
    }
    if(with_Object) {
        clazzes.emplace_back(clazz_definition["Object"]);
    }
    return clazzes;
}

Node SymbolTable::get_clazz(std::string clazz) {
    return clazz_definition[clazz];
}

std::vector<Node> SymbolTable::get_fields_for(std::string clazz) {
    std::vector<Node> fields;
    
    for(auto &field : symbol_table[clazz][FIELD]) {
        fields.push_back(field.second);
    }
    
    return fields;
}

std::vector<Node> SymbolTable::get_methods_for(std::string clazz) {
    std::vector<Node> methods;

    for(auto &method : symbol_table[clazz][METHOD]) {
        methods.push_back(method.second);
    }
    
    return methods;
}

Node SymbolTable::add_method_to_clazz(std::string clazz, Node n) {
    std::string name = n.get_data(DataType::id);

    if(!symbol_table[clazz][METHOD][name].is_empty()) {
        return symbol_table[clazz][METHOD][name];
    }
    symbol_table[clazz][METHOD][name] = n;
    return Node();
}

Node SymbolTable::add_field_to_clazz(std::string clazz, Node n) {
    std::string name = n.get_data(DataType::id);

    if(!symbol_table[clazz][FIELD][name].is_empty()) {
        return symbol_table[clazz][FIELD][name];
    }
    symbol_table[clazz][FIELD][name] = n;
    return Node();
}

Node SymbolTable::get_main_method() {
    if(symbol_table["Main"].empty()) { 
        // No Main class
        return Node();
    }
    return symbol_table["Main"][METHOD]["main"];
}

std::vector<std::string> SymbolTable::get_all_ancestors(std::string a) {
    if(a.empty() || clazz_definition[a].is_empty()) {
        return std::vector<std::string>();
    }

    std::vector<std::string> ancestors = {a};
    std::vector<std::string> loop;
    for(auto &clazz : clazz_in_cycle) {
        loop.push_back(clazz.get_data(DataType::id));
    }
    std::string curr_clazz = a;

    if(loop.empty()) {
        // Loop until Object if no cycle
        while(curr_clazz.compare("Object") != 0) {
            loop.erase(std::remove(loop.begin(), loop.end(), curr_clazz), loop.end());

            curr_clazz = clazz_definition[curr_clazz].get_data(DataType::parent_id);
            ancestors.push_back(curr_clazz);
        }
    } else {
        // Loop until Object or end of cycle
        while(curr_clazz.compare("Object") != 0 && loop.size() > 0) {
            loop.erase(std::remove(loop.begin(), loop.end(), curr_clazz), loop.end());

            curr_clazz = clazz_definition[curr_clazz].get_data(DataType::parent_id);
            ancestors.push_back(curr_clazz);
        }
    }    
    return ancestors;
}

std::string SymbolTable::find_common_ancestor(std::string a, std::string b) {
    std::vector<std::string> ancestors_a = get_all_ancestors(a);
    std::vector<std::string> ancestors_b = get_all_ancestors(b);
    
    for(auto &i : ancestors_a) {
        for(auto &j : ancestors_b) {
            if(i.compare(j) == 0) {
                return i;
            }
        }
    }
    return std::string();
}

Node SymbolTable::find_field(std::string cur_clazz, std::string id) {
    std::vector<std::string> ancestors = get_all_ancestors(cur_clazz);
    Node ret = Node();

    for(auto &anc : ancestors) {
        ret = symbol_table[anc][FIELD][id];
        if(!ret.is_empty()) {
            break;
        } else {
            symbol_table[anc][FIELD].erase(id);
        }
    }
    return ret;
}

Node SymbolTable::find_method(std::string cur_clazz, std::string id) {

    std::vector<std::string> ancestors = get_all_ancestors(cur_clazz);
    Node ret;

    for(auto &anc : ancestors) {
        ret = symbol_table[anc][METHOD][id];
        if(!ret.is_empty()) {
            return ret;
        } else {
            symbol_table[anc][METHOD].erase(id);
        }
    }
    return Node();   
}

void SymbolTable::print_symbol_table() {
    for(const auto &clazz : symbol_table) {
        std::cout << clazz.first << " contains:" << std::endl;

        for(const auto &field : symbol_table[clazz.first][FIELD]) {
            std::cout << "\tField : " << field.first << std::endl;
        } 

        for(const auto &method : symbol_table[clazz.first][METHOD]) {
            std::cout << "\tMethod: " << method.first << std::endl;
        } 
    }
}

bool SymbolTable::is_parent_of_child(std::string parent, std::string child) {
    std::vector<std::string> ancestors = get_all_ancestors(child);

    return (std::find(ancestors.begin(), ancestors.end(), parent) != ancestors.end());
}

bool SymbolTable::is_defined_clazz(std::string type) {
    bool exist = !clazz_definition[type].is_empty();
    if(!exist) {
        clazz_definition.erase(type);
    }
    return exist;
}

void SymbolTable::update_clazz(Node &n) {
    std::string id = n.get_data(DataType::id);
    clazz_definition[id] = n; 
}

void SymbolTable::update_method(std::string clazz, Node &n) {
    std::string id = n.get_data(DataType::id);
    symbol_table[clazz][METHOD][id] = n; 
}

void SymbolTable::update_field(std::string clazz, Node &n) {
    std::string id = n.get_data(DataType::id);
    symbol_table[clazz][FIELD][id] = n; 
}
