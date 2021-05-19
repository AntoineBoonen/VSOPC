/*
 * symbol_table.h
 *
 * by Antoine Boonen
 *
 * This file contains the interface of the SymbolTable class.
 *
 * Created   05/04/21
 * Modified  18/05/21
 */

#include "node.hpp"
#include "error.hpp"

#include <iostream>
#include <vector>
#include <map>

#ifndef VSOPCOMPILER_SYMBOL_H
#define VSOPCOMPILER_SYMBOL_H

class SymbolTable // SymbolTable is a Singleton
{
    private:
        static SymbolTable *instance; // The SymbolTable instance
        SymbolTable(); // SymbolTable constructor
        
        std::map<std::string, Node> clazz_definition; // A < class name - Node > mapping
        std::vector<Node> clazz_in_cycle; // A vector of Node in a cycle
        std::map<std::string, std::map<std::string, std::map<std::string, Node> > > symbol_table; // A < class name - method|field - id - Node > mapping

    public:
        /*
         * getInstance
         *
         * return:
         *      the instance of SymbolTable.
         */
        static SymbolTable *getInstance();

        /*
         * add_clazz_to_definition
         *
         * input:
         *      clazz - the clazz node to add to the definition mapping.
         * 
         * return:
         *      a node of the previously defined clazz (if any)
         *      an empty node otherwise  
         */
        Node add_clazz_to_definition(Node clazz);

        /*
         * add_method_to_clazz
         *
         * input:
         *      clazz - the clazz to add to the Node to.
         *      n - the method Node to add.
         * 
         * return:
         *      a node of the previously defined method for that clazz (if any)
         *      an empty node otherwise  
         */
        Node add_method_to_clazz(std::string clazz, Node n);

        /*
         * add_field_to_clazz
         *
         * input:
         *      clazz - the clazz to add to the Node to.
         *      n - the field Node to add.
         * 
         * return:
         *      a node of the previously defined field for that clazz (if any)
         *      an empty node otherwise  
         */
        Node add_field_to_clazz(std::string clazz, Node n);

        /*
         * cyclic_clazz_definition
         *
         * return:
         *      0  - no cycle in class definition,
         *     -1  - otherwise.
         */
        int cyclic_clazz_definition();

        /*
         * get_cycle
         *
         * return:
         *      a vector of the nodes referencing a cycle
         */
        std::vector<Node> get_cycle();

        /*
         * get_undefined_parents
         *
         * return:
         *      a vector of the clazz nodes referencing an undefined class.
         */
        std::vector<Node> get_undefined_parents();

        /*
         * get_defined_clazzes
         *
         * return:
         *      a vector of the clazz nodes referencing a defined class.
         */
        std::vector<Node> get_defined_clazzes(bool with_Object = false);

        /*
         * get_clazz
         *
         * input:
         *      clazz - the wanted clazz.
         * 
         * return:
         *      the Node representing the clazz in the abstart syntax tree.
         */
        Node get_clazz(std::string clazz);

        /*
         * get_fields_for
         *
         * input:
         *      clazz - the wanted clazz.
         * 
         * return:
         *      a vector of Node representing the fields declared (not inherited) for the input clazz.
         */
        std::vector<Node> get_fields_for(std::string clazz);

        /*
         * get_methods_for
         *
         * input:
         *      clazz - the wanted clazz.
         * 
         * return:
         *      a vector of Node representing the methods declared (not inherited) for the input clazz.
         */
        std::vector<Node> get_methods_for(std::string clazz);

        /*
         * get_main_method
         *
         * return:
         *      a clazz node named 'Main' if any,
         *      an empty node otherwise. 
         */
        Node get_main_method();
    
        /*
         * get_all_ancestors
         *
         * input:
         *      a - the clazz to get the ancestors for.
         * 
         * return:
         *      a vector of ancestor classes in order of inheritance (child before parent).
         */
        std::vector<std::string> get_all_ancestors(std::string a);

        /*
         * find_common_ancestor
         *
         * input:
         *      a b - the classes to get the common ancestor for.
         * 
         * return:
         *      the common ancestor if any,
         *      an empty string otherwise (happens when loop).
         */
        std::string find_common_ancestor(std::string a, std::string b);

        /*
         * find_field
         *
         * input:
         *      cur_clazz - the current class scope.
         *      id - the name of the field.
         * 
         * return:
         *      The field node in the scope if any,
         *      an empty Node otherwise.
         */
        Node find_field(std::string cur_clazz, std::string id);

        /*
         * find_method
         *
         * input:
         *      cur_clazz - the current class scope.
         *      id - the name of the method.
         * 
         * return:
         *      The method node in the scope if any,
         *      an empty Node otherwise.
         */
        Node find_method(std::string cur_clazz, std::string id);

        /*
         * is_parent_of_child
         *
         * input:
         *      parent - the parent class.
         *      child - the child class.
         * 
         * return:
         *      true if parent is an ancestor of child,
         *      false otherwise.
         */
        bool is_parent_of_child(std::string parent, std::string child);

        /*
         * is_defined_clazz
         *
         * input:
         *      type - the class.
         * 
         * return:
         *      true if class has been defined,
         *      false otherwise.
         */
        bool is_defined_clazz(std::string type);

        /*
         * update_clazz
         *
         * input:
         *      n - the clazz Node.
         * 
         * Update the clazz representation in the data structures.
         */
        void update_clazz(Node &n);

        /*
         * update_method
         *
         * input:
         *      clazz - the clazz to update.
         *      n - a reference to the method Node.
         * 
         * Update the method representation for the clazz in the data structures.
         */
        void update_method(std::string clazz, Node &n);

        /*
         * update_field
         *
         * input:
         *      clazz - the clazz to update.
         *      n - a reference to the field Node.
         * 
         * Update the field representation for the clazz in the data structures.
         */
        void update_field(std::string clazz, Node &n);

        /*
         * is_parent_of_child
         *
         * Print the symbol_table content.
         */
        void print_symbol_table();
};

#endif //VSOPCOMPILER_SYMBOL_H