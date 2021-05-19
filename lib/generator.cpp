/*
 * generator.cpp
 *
 * by Antoine Boonen
 *
 * This file contains the implementation of the intermediate code Generator class as described in the interface 'generator.h'.
 *
 * Created   13/05/21
 * Modified  18/05/21
 */

#include "generator.hpp"
#include "symbol_table.hpp"

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/APInt.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/Argument.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/GlobalObject.h"
#include "llvm/IR/GlobalVariable.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Instruction.h"
#include "llvm/Support/AtomicOrdering.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"

#include <cstdlib>
#include <fstream>
#include <map>
#include <memory>
#include <ostream>
#include <string>
#include <system_error>
#include <vector>
#include <iostream>
#include <algorithm>

#define _FUNCTION_PADDING "__"
#define _VTABLE_PADDING "___"
#define _VTABLE "VTable"
#define _NEW_ "_new"
#define _INIT "_init"
#define _MALLOC "malloc"
#define _POWER "power"
#define _EXT_SIZE 5

static std::unique_ptr<llvm::LLVMContext> llvm_context; // Unique pointer to the LLVM context
static std::unique_ptr<llvm::Module> llvm_module; // Unique pointer to the LLVM module
static std::unique_ptr<llvm::IRBuilder<> > llvm_builder; // Unique pointer to the LLVM IR builder
static std::map<std::string, std::map<std::string, int> > field_index_map; // Map used to store indexes of the field definition
static std::map<std::string, std::map<std::string, int> > method_index_map; // Map used to store indexes of the method definition


/*
 * initialize_module
 *
 * input:
 *      filename - a reference to the filename string.        
 * 
 * Initialize the pointers and build the necessary structures.
 */
static void initialize_module(std::string &filename);

/*
 * build_structures
 *
 * Build the structures necessary for LLVM IR.
 */
static void build_structures();

/*
 * codegen
 *
 * input:
 *      n - a reference to an abstract syntax tree.        
 *      clazz_name - the current clazz.
 *      named_value - a map if the local variables with their LLVM Value representation.
 * 
 * return:
 *      value - the LLVM Value representation of n.
 */
static llvm::Value *codegen(Node &n, std::string clazz_name = "", std::map<std::string, llvm::Value * > named_value = std::map<std::string, llvm::Value * >());

/*
 * get_llvm_type
 *
 * input:        
 *      type - a string representing the type to translate.
 * 
 * return:
 *      type - the LLVM Type representation of the input type.
 */
static llvm::Type *get_llvm_type(std::string type);

/*
 * format_string
 *
 * input:
 *      str - the string to format.
 * 
 * Transform the escape sequence in the string into their ASCII representation.
 */
static void format_string(std::string &str);

Generator::Generator(Node &ast, std::string &filename) {
    this->ast = ast;
    initialize_module(filename);
}

void Generator::generate() {
    codegen(ast);
    return;
}

void Generator::print() {
    llvm_module->print(llvm::outs(), nullptr);
}

int Generator::save(std::string &filename) {
    std::error_code error;

    llvm::raw_fd_ostream fd(filename, error);
    llvm_module->print(fd, nullptr);

    return error.value();
}

void initialize_module(std::string &filename) {
    // Initialize the context, the module and builder pointer
    using namespace llvm;
    llvm_context = std::make_unique<LLVMContext>();
    llvm_module = std::make_unique<Module>(filename, *llvm_context);
    llvm_builder = std::make_unique<IRBuilder<>>(*llvm_context);

    // Initialize all classes and vtables structures with defined and inhereted fields and methods.
    build_structures();
}

void build_structures() {
    SymbolTable *s = SymbolTable::getInstance();

    // Declare all class structures
    for(auto &clazz : s->get_defined_clazzes(true)) {
        std::string type = clazz.get_data(DataType::type);
        llvm::StructType::create(* llvm_context, type);
    }

    // Declare all type vtable structures
    for(auto &clazz : s->get_defined_clazzes(true)) {
        std::string type = clazz.get_data(DataType::type);
        llvm::StructType::create(* llvm_context, type + _VTABLE);
    }

    // Declare EXTERNAL 'new' function for Object
    auto new_f = llvm::FunctionType::get(get_llvm_type("Object"), false);
    llvm::Function::Create(new_f, llvm::Function::ExternalLinkage, std::string("Object") + _FUNCTION_PADDING + _NEW_, llvm_module.get());

    // Declare EXTERNAL 'init' function for Object
    auto init_f = llvm::FunctionType::get(get_llvm_type("Object"), {get_llvm_type("Object")}, false);
    llvm::Function::Create(init_f, llvm::Function::ExternalLinkage, std::string("Object") + _FUNCTION_PADDING + _INIT, llvm_module.get());

    // Declare EXTERNAL Object functions
    for(auto &method : s->get_clazz("Object").get_children(NodeType::method)) {

        // Get the formal types
        auto formal_vec = std::vector<llvm::Type *>();
        formal_vec.push_back(get_llvm_type("Object")); // Add pointer to class instance
                                    
        for(auto &formal : method.get_children(NodeType::formal)) {
            formal_vec.push_back(get_llvm_type(formal.get_data(DataType::type)));
        }

        auto f_type = llvm::FunctionType::get(get_llvm_type(method.get_data(DataType::type)), formal_vec, false);
        llvm::Function::Create(f_type, llvm::Function::ExternalLinkage, std::string("Object") + _FUNCTION_PADDING + 
            method.get_data(DataType::id), llvm_module.get());
    } 

    // Declare EXTERNAL malloc function
    auto malloc_type = llvm::FunctionType::get(llvm::PointerType::get(llvm::IntegerType::getInt8Ty(* llvm_context), 0), 
        {llvm::IntegerType::getInt64Ty(* llvm_context)}, false);
    llvm::Function::Create(malloc_type, llvm::Function::ExternalLinkage, _MALLOC, llvm_module.get());

    // Declare EXTERNAL power function
    auto power_type = llvm::FunctionType::get(llvm::IntegerType::getInt64Ty(* llvm_context), 
        {get_llvm_type("int32"), get_llvm_type("int32")}, false);
    llvm::Function::Create(power_type, llvm::Function::ExternalLinkage, _POWER, llvm_module.get());

    // Declare all Class functions (except for Object)
    for(auto &clazz : s->get_defined_clazzes()) {

        std::string id = clazz.get_data(DataType::id);

        // Declare 'new' function
        new_f = llvm::FunctionType::get(get_llvm_type(id), false);
        llvm_module->getOrInsertFunction(id + _FUNCTION_PADDING + _NEW_, new_f);

        // Declare 'init' function
        init_f = llvm::FunctionType::get(get_llvm_type(id), {get_llvm_type(id)}, false);
        llvm_module->getOrInsertFunction(id + _FUNCTION_PADDING + _INIT, init_f);
            
        for(auto &method : s->get_methods_for(id)) {

            // Get the formal types
            auto formal_vec = std::vector<llvm::Type *>();
            formal_vec.push_back(get_llvm_type(id)); // Add pointer to class instance
                                        
            for(auto &formal : method.get_children(NodeType::formal)) {
                formal_vec.push_back(get_llvm_type(formal.get_data(DataType::type)));
            }

            auto f = llvm::FunctionType::get(get_llvm_type(method.get_data(DataType::type)), formal_vec, false);
            llvm_module->getOrInsertFunction(id + _FUNCTION_PADDING + method.get_data(DataType::id), f);
        } 
    }

    // For each class, declare the (inherited) method types
    for(auto &clazz : s->get_defined_clazzes(true)) {
        std::string type = clazz.get_data(DataType::type);
        llvm::StructType *cur = llvm_module->getTypeByName(type + _VTABLE);

        auto type_vec = std::vector<llvm::Type *>();
        auto method_vec = std::vector<llvm::Constant *>();

        std::map<std::string, bool> overriden;
        std::map<std::string, int> overriden_index;
        int index = 0;

        std::vector<std::string> ancestors = s->get_all_ancestors(type);
        std::reverse(ancestors.begin(), ancestors.end());
        for(auto &anc : ancestors) {
            for(auto &method : s->get_clazz(anc).get_children(NodeType::method)) {
                std::string method_id = method.get_data(DataType::id);

                if(overriden[method_id] == false) { // Not overriding any method

                    method_vec.push_back(llvm_module->getFunction(anc + _FUNCTION_PADDING + method_id));
                    auto t = ((llvm::Function *)method_vec.back())->getFunctionType();
                    type_vec.push_back(llvm::PointerType::get(t, 0)); // Pointer to the function definition

                    overriden[method_id] = true;
                    overriden_index[method_id] = index;

                    method_index_map[type][method_id] = index;
                
                } else { // Overriding
                    method_vec[overriden_index[method_id]] = llvm_module->getFunction(anc + _FUNCTION_PADDING + method_id);
                    auto t = ((llvm::Function *)method_vec[overriden_index[method_id]])->getFunctionType();
                    type_vec[overriden_index[method_id]] = llvm::PointerType::get(t, 0); // Pointer to the function definition
                }
                ++index;
            } 
        }
        cur->setBody(type_vec);

        // Create a global constant structure for the vtable
        if(type != "Object") { // Avoid Object global VTABLE redefinition
            auto init = llvm::ConstantStruct::get(cur, method_vec);
            llvm::GlobalVariable *vtable = (llvm::GlobalVariable *)llvm_module->getOrInsertGlobal(type + _VTABLE_PADDING + "vtable", cur);
            vtable->setInitializer(init);
            vtable->setConstant(true);
        } 
    }

    // For each class, declare the (inherited) field types
    for(auto &clazz : s->get_defined_clazzes(true)) {
        std::string type = clazz.get_data(DataType::type);

        llvm::StructType *cur = (llvm::StructType *) llvm_module->getTypeByName(type);

        auto type_vec = std::vector<llvm::Type *>();
        type_vec.push_back(llvm::PointerType::get(llvm_module->getTypeByName(type + _VTABLE), 0)); // Add table pointer

        std::vector<std::string> ancestors = s->get_all_ancestors(type);
        std::reverse(ancestors.begin(), ancestors.end());
        for(auto &anc : ancestors) {

            for(auto &field : s->get_fields_for(anc)) {
                std::string t = field.get_data(DataType::type);
                type_vec.push_back(get_llvm_type(t));
            } 
        }
        cur->setBody(type_vec);
    }

    // Build 'main' method aka LLVM entry point
    llvm_module->getOrInsertFunction("main", llvm::FunctionType::get(get_llvm_type("int32"), false));
    llvm::BasicBlock *main_b = llvm::BasicBlock::Create(* llvm_context, "entry", llvm_module->getFunction("main"));
    llvm_builder->SetInsertPoint(main_b);

        // Call to 'new Main'
    llvm::Function *main = llvm_module->getFunction(std::string("Main") + _FUNCTION_PADDING + _NEW_);
    llvm::Value *main_val = llvm_builder->CreateCall(main);

        // Call to 'Main.main()'
    llvm::Function *main_f = llvm_module->getFunction(std::string("Main") + _FUNCTION_PADDING + "main");
    llvm::Value *main_ret = llvm_builder->CreateCall(main_f, {main_val});

        // Ret
    llvm_builder->CreateRet(main_ret);

    return;
}

llvm::Value *codegen(Node &n, std::string clazz_name, std::map<std::string, llvm::Value * > named_value) {
    SymbolTable *s = SymbolTable::getInstance();

    switch(n.get_type()) {
        case NodeType::program: {
            for(auto &clazz : n.get_children(NodeType::clazz)) {
                codegen(clazz);
            }
            return nullptr;
        }
        case NodeType::clazz: {
            std::string id = n.get_data(DataType::id);
    
            // Define 'new' function
            auto new_f = llvm_module->getFunction(id + _FUNCTION_PADDING + _NEW_);
            llvm::BasicBlock *new_b = llvm::BasicBlock::Create(* llvm_context, "entry", (llvm::Function *) new_f);
            llvm_builder->SetInsertPoint(new_b);

                // Malloc call
            auto malloc_f = llvm_module->getFunction(_MALLOC);
            size_t size = (new llvm::DataLayout(llvm_module.get()))->getTypeAllocSizeInBits(llvm_module->getTypeByName(id));
            std::vector<llvm::Value * > malloc_args = {llvm::ConstantInt::get(llvm::IntegerType::getInt64Ty(* llvm_context), size)};
            auto self = llvm_builder->CreateCall(malloc_f, malloc_args);
            
                // Cast to Parent*
            std::string parent = n.get_data(DataType::parent_id);
            auto parent_ptr = llvm_builder->CreatePointerCast(self, llvm::PointerType::get(llvm_module->getTypeByName(parent), 0));
            
                // Init call of the inherited classes
            auto parent_init = llvm_module->getFunction(parent + _FUNCTION_PADDING + _INIT);
            parent_ptr = llvm_builder->CreateCall((llvm::Function *)parent_init, {parent_ptr});

                // Cast to Child*
            auto child_ptr = llvm_builder->CreatePointerCast(parent_ptr, llvm::PointerType::get(llvm_module->getTypeByName(id), 0));

            auto child_init = llvm_module->getFunction(id + _FUNCTION_PADDING + _INIT);
            child_ptr = llvm_builder->CreateCall((llvm::Function *)child_init, {child_ptr});
            
                // Ret
            llvm_builder->CreateRet(child_ptr);

            // Define 'init' function
            auto init_f = llvm_module->getFunction(id + _FUNCTION_PADDING + _INIT);
            llvm::BasicBlock *init_b = llvm::BasicBlock::Create(* llvm_context, "entry", (llvm::Function *) init_f);
            llvm_builder->SetInsertPoint(init_b);

                // Set vtable
            auto self_ptr = init_f->args().begin();
            auto vtable_adr = llvm_builder->CreateStructGEP(llvm_module->getTypeByName(id), self_ptr, 0);
            llvm_builder->CreateStore(llvm_module->getNamedGlobal(id + _VTABLE_PADDING + "vtable"), vtable_adr);

                // Set all other fields
            std::vector<std::string> ancestors = s->get_all_ancestors(id);
            std::reverse(ancestors.begin(), ancestors.end());
            int field_index = 1; // 1 to skip the vtable
            for(auto &anc : ancestors) {
                for(auto &field : s->get_fields_for(anc)) {

                    llvm::Value *f_val = codegen(field, id);
                    auto f_addr = llvm_builder->CreateStructGEP(llvm_module->getTypeByName(id), self_ptr, field_index);
                    llvm_builder->CreateStore(f_val, f_addr);

                    field_index_map[id][field.get_data(DataType::id)] = field_index;

                    field_index++;
                }
            }
                // Ret
            llvm_builder->CreateRet(self_ptr);

            // Method code generation
            for(auto &method : n.get_children(NodeType::method)) {
                codegen(method, id);
            }
            
            return nullptr;
        }
        case NodeType::field: {
            llvm::Value *f_val;

            if(n.get_children(NodeType::any_expr).empty()) {
                // Default initialization
                std::string type = n.get_data(DataType::type);
                llvm::Type *f_type = get_llvm_type(type);

                if(type == "int32" || type == "bool") {
                    f_val = llvm::ConstantInt::get(f_type, 0);
                } else if(type == "string") {
                    f_val = llvm_builder->CreateGlobalStringPtr("");
                } else { // Class ref of Unit
                    f_val = llvm::ConstantPointerNull::get((llvm::PointerType *) f_type);
                }
            } else {
                f_val = codegen(*n.get_children(NodeType::any_expr).begin());
            }

            return f_val;
        }
        case NodeType::method: {
            llvm::Function *method = llvm_module->getFunction(clazz_name + _FUNCTION_PADDING + n.get_data(DataType::id));
            llvm::BasicBlock *method_b = llvm::BasicBlock::Create(* llvm_context, "entry", (llvm::Function *) method);
            llvm_builder->SetInsertPoint(method_b);

            auto arg = method->args().begin();
            arg++;
            for(auto &formal : n.get_children(NodeType::formal)) {
                llvm::Value* formal_val = llvm_builder->CreateAlloca(get_llvm_type(formal.get_data(DataType::type)));
                llvm_builder->CreateStore(arg, formal_val);
                named_value[formal.get_data(DataType::id)] = formal_val;
                ++arg;
            }

            auto ret_val = codegen(*n.get_children(NodeType::block).begin(), clazz_name, named_value);

            llvm_builder->CreateRet(ret_val);

            return nullptr;
        }
        case NodeType::block: {
            llvm::Value *val;
            for(auto &expr : n.get_children(NodeType::any_expr)) {
                val = codegen(expr, clazz_name, named_value);
            }
            return val;
        }
        case NodeType::if_expr: {
            llvm::Function *cur_f = llvm_builder->GetInsertBlock()->getParent(); 

            // Generate the conditionnal code
            llvm::Value *cond_val = codegen(*n.get_children(NodeType::if_statement).begin(), clazz_name, named_value);

            // Create the blocks
            llvm::BasicBlock *then_b = llvm::BasicBlock::Create(* llvm_context, "if.then");
            llvm::BasicBlock *else_b = llvm::BasicBlock::Create(* llvm_context, "if.else");
            llvm::BasicBlock *end_b = llvm::BasicBlock::Create(* llvm_context, "if.end");
            
            // Create a conditional branch
            llvm_builder->CreateCondBr(cond_val, then_b, else_b);

            llvm::Type *cast_type = get_llvm_type(n.get_data(DataType::type));

            // THEN Block 
            cur_f->getBasicBlockList().push_back(then_b); // Append the then block

            llvm_builder->SetInsertPoint(then_b);
            llvm::Value *then_val = codegen(*n.get_children(NodeType::then_statement).begin(), clazz_name, named_value);
            then_val = llvm_builder->CreatePointerCast(then_val, cast_type);
            llvm_builder->CreateBr(end_b);

            then_b = llvm_builder->GetInsertBlock(); // Important to get the last block for PHI
        
            // ELSE Block 
            cur_f->getBasicBlockList().push_back(else_b); // Append the else block

            llvm_builder->SetInsertPoint(else_b);
            llvm::Value *else_val = llvm::ConstantPointerNull::get((llvm::PointerType *)get_llvm_type("unit"));
            if(!n.get_children(NodeType::else_statement).empty()) {
                else_val = codegen(*n.get_children(NodeType::else_statement).begin(), clazz_name, named_value);
            }
            else_val = llvm_builder->CreatePointerCast(else_val, cast_type);
            llvm_builder->CreateBr(end_b);

            else_b = llvm_builder->GetInsertBlock(); // Important to get the last block for PHI
            
            // END Block
            cur_f->getBasicBlockList().push_back(end_b); // Append the end block
            llvm_builder->SetInsertPoint(end_b);

              // Build PHI
            llvm::Value *end_val = llvm_builder->CreatePHI(cast_type, 0, "if.PHI");

              // Add paths to PHI
            ((llvm::PHINode *)end_val)->addIncoming(then_val, then_b);
            ((llvm::PHINode *)end_val)->addIncoming(else_val, else_b);

            return end_val;
        }
        case NodeType::while_expr: {
            llvm::Function *cur_b = llvm_builder->GetInsertBlock()->getParent();
            llvm::BasicBlock *cond_b = llvm::BasicBlock::Create(* llvm_context, "while.cond", (llvm::Function *) cur_b);
            llvm::BasicBlock *body_b = llvm::BasicBlock::Create(* llvm_context, "while.body", (llvm::Function *) cur_b);
            llvm::BasicBlock *end_b = llvm::BasicBlock::Create(* llvm_context, "while.end", (llvm::Function *) cur_b);

            llvm_builder->CreateBr(cond_b);

            // COND Block
            llvm_builder->SetInsertPoint(cond_b);
            llvm::Value *cond_val = codegen(*n.get_children(NodeType::while_statement).begin(), clazz_name, named_value);
            llvm_builder->CreateCondBr(cond_val, body_b, end_b);

            // BODY Block
            llvm_builder->SetInsertPoint(body_b);
            codegen(*n.get_children(NodeType::do_statement).begin(), clazz_name, named_value);
            llvm_builder->CreateBr(cond_b);

            // End Block
            llvm_builder->SetInsertPoint(end_b);
            return nullptr;
        }
        case NodeType::let_expr: {
            // Allocate the new variable
            std::string init_type = n.get_children(NodeType::object_identifier).begin()->get_data(DataType::type);
            llvm::Value *let_val = llvm_builder->CreateAlloca(get_llvm_type(init_type));
        
            // Initialize it
            llvm::Value *init_val;
            if(n.get_children(NodeType::init_statement).empty()) {
                // Default initialization
                llvm::Type *f_type = get_llvm_type(init_type);
                if(init_type == "int32" || init_type == "bool") {
                    init_val = llvm::ConstantInt::get(f_type, 0);
                } else if(init_type == "string") {
                    init_val = llvm_builder->CreateGlobalStringPtr("");
                } else { // Class ref of Unit
                    init_val = llvm::ConstantPointerNull::get((llvm::PointerType *) f_type);
                }
            } else {
                // Generate the initialization
                init_val = codegen(*n.get_children(NodeType::init_statement).begin(), clazz_name, named_value);
                
                // Cast if needed
                init_val = llvm_builder->CreatePointerCast(init_val, get_llvm_type(init_type));
            }

            // Store the initial value
            llvm_builder->CreateStore(init_val, let_val);

            // Add to scope
            std::string id = n.get_children(NodeType::object_identifier).begin()->get_data(DataType::literal_value);
            named_value[id] = let_val;

            // Generate the body code
            llvm::Value *scope_val = codegen(*n.get_children(NodeType::scope_statement).begin(), clazz_name, named_value);
            
            return scope_val;
        }
        case NodeType::assign_expr: {
            // Generate the code to be assigned
            llvm::Value *val = codegen(*n.get_children(NodeType::any_expr).begin(), clazz_name, named_value);

            // Cast the value
            std::string cast_type = n.get_data(DataType::type);
            val = llvm_builder->CreatePointerCast(val, get_llvm_type(cast_type));

            std::string id = n.get_data(DataType::id);
            if(named_value[id] == nullptr) { // class variable

                // Get the pointer to the current class
                llvm::Function *cur_f = llvm_builder->GetInsertBlock()->getParent();
                auto clazz = cur_f->args().begin();
                
                // Save the result
                auto f_addr = llvm_builder->CreateStructGEP(llvm_module->getTypeByName(clazz_name), clazz, field_index_map[clazz_name][id]);
                llvm_builder->CreateStore(val, f_addr);

            } else { // local variable
                llvm_builder->CreateStore(val, named_value[id]);
            }

            return val;
        }
        case NodeType::unop_expr: {
            std::string op = n.get_data(DataType::op);

            llvm::Value *right = codegen(*n.get_children(NodeType::any_expr).begin(), clazz_name, named_value);
            llvm::Value *ret;
            if(op == "-") {
                ret = llvm_builder->CreateNeg(right);
            } else if(op == "not") {
                ret = llvm_builder->CreateNot(right);
            } else if(op == "isnull") {
                ret = llvm_builder->CreateIsNull(right);
            } else {
                return nullptr;
            }
            return ret;
        }
        case NodeType::binop_expr: {
            std::string op = n.get_data(DataType::op);

            // Generate code for LEFT
            llvm::Value *left = codegen(*n.get_children(NodeType::left_statement).begin(), clazz_name, named_value);

            if(op == "and") {

                // Create blocks
                llvm::BasicBlock *left_eval_true = llvm::BasicBlock::Create(*llvm_context, "and.true");
                llvm::BasicBlock *left_eval_false = llvm::BasicBlock::Create(*llvm_context, "and.true");
                llvm::BasicBlock *end = llvm::BasicBlock::Create(*llvm_context);

                llvm_builder->CreateCondBr(left, left_eval_true, left_eval_false);

                // If left is true, return eval(right)
                llvm_builder->GetInsertBlock()->getParent()->getBasicBlockList().push_back(left_eval_true);

                llvm_builder->SetInsertPoint(left_eval_true);
                llvm::Value *true_ret = codegen(*n.get_children(NodeType::right_statement).begin(), clazz_name, named_value);
                llvm_builder->CreateBr(end);

                left_eval_true = llvm_builder->GetInsertBlock();

                // Else, return false
                llvm_builder->GetInsertBlock()->getParent()->getBasicBlockList().push_back(left_eval_false);
                
                llvm_builder->SetInsertPoint(left_eval_false);
                llvm::Value *false_ret = llvm_builder->getFalse();
                llvm_builder->CreateBr(end);

                left_eval_false = llvm_builder->GetInsertBlock();

                // Return the value with PHI
                llvm_builder->GetInsertBlock()->getParent()->getBasicBlockList().push_back(end);
                llvm_builder->SetInsertPoint(end);

                llvm::Value *ret = llvm_builder->CreatePHI(get_llvm_type("bool"), 0);
                ((llvm::PHINode *)ret)->addIncoming(true_ret, left_eval_true);
                ((llvm::PHINode *)ret)->addIncoming(false_ret, left_eval_false);

                return ret;

            } else {
                // Generate code for RIGHT
                llvm::Value *right = codegen(*n.get_children(NodeType::right_statement).begin(), clazz_name, named_value);

                if(op == "=") {
                    // If comparing classes, cast the addresses to interger and compare
                    if(s->is_defined_clazz(n.get_children(NodeType::left_statement).begin()->get_data(DataType::type))) {
                        left = llvm_builder->CreatePointerCast(left, llvm::IntegerType::getInt64Ty(* llvm_context));
                        right = llvm_builder->CreatePointerCast(right, llvm::IntegerType::getInt64Ty(* llvm_context));
                    }
                    return llvm_builder->CreateICmpEQ(left, right);

                } else if(op == "<") {
                    return llvm_builder->CreateICmpSLT(left, right);

                } else if(op == "<=") {
                    return llvm_builder->CreateICmpSLE(left, right);

                } else if(op == "+") {
                    return llvm_builder->CreateAdd(left, right);

                } else if(op == "-") {
                    return llvm_builder->CreateSub(left, right);

                } else if(op == "*") {
                    return llvm_builder->CreateMul(left, right);

                } else if(op == "/") {
                    return llvm_builder->CreateUDiv(left, right);

                } else if(op == "^") {
                    llvm::Value *ret = llvm_builder->CreateCall(llvm_module->getFunction(_POWER), {left, right});
                    return llvm_builder->CreateIntCast(ret, get_llvm_type("int32"), true);
                } else {
                    return nullptr;
                }
            }
        }
        case NodeType::call_expr: {
            
            llvm::Value *vtable;
            llvm::Argument *clazz;
            std::string name;
            Node parent = *n.get_children(NodeType::parent_statement).begin();

            clazz = (llvm::Argument *) codegen(*n.get_children(NodeType::parent_statement).begin(), clazz_name, named_value); 
            if(parent.get_data(DataType::literal_value) == "self") {
                name = clazz_name;
            } else {
                name = parent.get_data(DataType::type);
            }
            
            // Fetch the VTABLE
            vtable = llvm_builder->CreateStructGEP(llvm_module->getTypeByName(name), clazz, 0);
            vtable = llvm_builder->CreateLoad(vtable);
            
            // Fetch the function
            std::string method_id = n.get_data(DataType::id);
            int method_index = method_index_map[name][method_id];
            llvm::Type *struct_type = llvm_module->getTypeByName(name + _VTABLE);
            llvm::Value *f = llvm_builder->CreateStructGEP(struct_type, vtable, method_index);
            f = llvm_builder->CreateLoad(f);

            // Cast the class pointer
            llvm::FunctionType *f_type = (llvm::FunctionType *)((llvm::PointerType *) struct_type->getStructElementType(method_index))->getElementType();
            llvm::Type *cast_type = f_type->getParamType(0);
            clazz = (llvm::Argument *)llvm_builder->CreatePointerCast(clazz, cast_type);

            // Generate the ARGS
            std::vector<llvm::Value * > args;
            args.push_back(clazz);
            int i = 1;
            for(auto &arg : n.get_children(NodeType::args)) {
                // Generate the ARG code
                llvm::Value *arg_val = codegen(arg, clazz_name, named_value);

                // Cast
                arg_val = llvm_builder->CreatePointerCast(arg_val, f_type->getParamType(i));

                args.push_back(arg_val);
                ++i;
            }

            // Call the function
            llvm::Value *f_call = llvm_builder->CreateCall(f_type, f, args);

            return f_call;
        }
        case NodeType::new_expr: {
            std::string type = n.get_data(DataType::type);
            return llvm_builder->CreateCall(llvm_module->getFunction(type + _FUNCTION_PADDING + _NEW_));
        }
        case NodeType::object_identifier: {
            std::string id = n.get_data(DataType::literal_value);     
            llvm::Value *val;
            if(named_value[id] == nullptr) { // class variable

                // Get the pointer to the current class
                llvm::Function *cur_f = llvm_builder->GetInsertBlock()->getParent();
                auto clazz = cur_f->args().begin();
                
                if(id != "self") {
                    val = llvm_builder->CreateStructGEP(llvm_module->getTypeByName(clazz_name), clazz, field_index_map[clazz_name][id]);
                } else {
                    return clazz;
                }

            } else { // local variable
                val = named_value[id];
            }
            return llvm_builder->CreateLoad(val);
        }
        case NodeType::literal_int32: {
            int val = std::stoi(n.get_data(DataType::literal_value));
            return llvm::ConstantInt::get(get_llvm_type("int32"), val);
        }
        case NodeType::literal_bool: {
            std::string val = n.get_data(DataType::literal_value);
            if(val == "true") {
                return llvm::ConstantInt::get(get_llvm_type("bool"), 1);
            } else {
                return llvm::ConstantInt::get(get_llvm_type("bool"), 0);
            }
        }
        case NodeType::literal_string: {
            std::string formated = std::string(n.get_data(DataType::literal_value));
            format_string(formated);

            return llvm_builder->CreateGlobalStringPtr(formated);
        }
        case NodeType::literal_unit: {
            return llvm::ConstantPointerNull::get((llvm::PointerType *) get_llvm_type("unit"));
        }
        default: {
            return nullptr;
        }
    }
}

llvm::Type *get_llvm_type(std::string type) {
    llvm::Type *t;
    if(type == "int32") {
        t = llvm::IntegerType::getInt32Ty(* llvm_context);
    } else if (type == "bool") {
        t = llvm::IntegerType::getInt1Ty(* llvm_context);
    } else if (type == "unit") {
        t = llvm::PointerType::get(llvm::PointerType::getInt8Ty(* llvm_context), 0); // Explained in doc
    } else if (type == "string") {
        t = llvm::PointerType::get(llvm::PointerType::getInt8Ty(* llvm_context), 0);
    } else { // Class ref
        t = llvm::PointerType::get(llvm_module->getTypeByName(type), 0);
    }
    return t;
}

void format_string(std::string &str) {

    str = str.substr(1, str.size() - 2); // Remove start and end \"

    // Tranform and replace each escape sequance by its ASCII representation
    size_t start_pos = 0;
    while((start_pos = str.find("\\x", start_pos)) != std::string::npos) {
        char escape_car[3] = {str[start_pos + 2], str[start_pos + 3], '\0'};
        char escape_val = std::stoi(escape_car, nullptr, 16);

        str.replace(start_pos, 4, std::string(1, escape_val));
        start_pos += 1;
    }
    return;
}
