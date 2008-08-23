/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; This program is free software; you can redistribute it and/or
;; modify it under the terms of the GNU General Public License
;; as published by the Free Software Foundation; either version 2
;; of the License, or (at your option) any later version.
;;
;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with this program; if not, write to the Free Software
;; Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
   ***** END LICENSE BLOCK *****
*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <unicode/unistr.h>
#include <unicode/ustream.h>

#include <boost/algorithm/string.hpp>

#include "llvm/GlobalVariable.h"
#include "llvm/System/DynamicLibrary.h"
#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/ModuleProvider.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Support/IRBuilder.h"
#include "llvm/Module.h"
#include "llvm/DerivedTypes.h"
#include "llvm/Constants.h"
#include "llvm/Instructions.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/ModuleProvider.h"
#include "llvm/ExecutionEngine/JIT.h"
#include "llvm/ExecutionEngine/Interpreter.h"
#include "llvm/ExecutionEngine/GenericValue.h"

#include "pLexers.h"
#include "pParser.h"
#include "pDriver.h"

using namespace std;

namespace rphp {


/**
 * execute given file -- php source or bytecode
 */
void pDriver::execute(string fileName) {
    //cout << "executing file: " << fileName << endl;
    // TODO: determine file type, then run executeBC or executePHP
    executeBC(fileName);
}

/**
 * execute precompiled bytecode
 */
void pDriver::executeBC(string fileName) {

    // Now we create the JIT.
    string* errMsg = new string();
    llvm::MemoryBuffer* mb = llvm::MemoryBuffer::getFile(fileName.c_str(), errMsg);
    if (errMsg->length()) {
        cerr << "error loading file [" << fileName << "]: " << *errMsg << endl;
        delete errMsg;
        return;
    }
    llvm::ModuleProvider* MP = llvm::getBitcodeModuleProvider(mb, errMsg);
    if (errMsg->length()) {
        cerr << "error parsing bitcode file [" << fileName << "]: " << *errMsg << endl;
        delete errMsg;
        return;
    }

    // MP now owns mb and will delete

    if (llvm::sys::DynamicLibrary::LoadLibraryPermanently("librphp-runtime.so", errMsg)) {
        cerr << "error loading runtime library: " << *errMsg << endl;
        delete errMsg;
        return;
    }

    //MP->getModule()->setDataLayout("e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128");
    //MP->getModule()->setTargetTriple("x86_64-pc-linux-gnu");

    //llvm::ExecutionEngine* EE = llvm::ExecutionEngine::create(MP, false);
    llvm::ExecutionEngine* EE = llvm::ExecutionEngine::createJIT(MP, errMsg);
    if (!EE) {
        cerr << *errMsg << endl;
        delete errMsg;
        return;
    }

    // errMsg isn't needed.
    delete errMsg;

    // EE now owns MP

    llvm::Function* rphpMain = MP->getModule()->getFunction("rphp_main");
    if (!rphpMain) {
        cerr << "error: rphp_main symbol not found" << endl;
        MP->getModule()->dump();
        delete EE;
        return;
    }

    EE->runStaticConstructorsDestructors(false);

    // Call the rphp_main function with no arguments:
    std::vector<llvm::GenericValue> noargs;
    llvm::GenericValue gv = EE->runFunction(rphpMain, noargs);

    EE->runStaticConstructorsDestructors(true);

    delete EE;

}

/**
 * compile, then execute PHP source
 */
void pDriver::executePHP(string fileName) {
    cout << "compile and execute php file: " << fileName << endl;
}

/**
 * compile to llvm bytecode
 */
void pDriver::compileToBC(string fileName) {
    cout << "compiling " << fileName << " to bytecode " << endl;

    llvm::Module* phpModule = compileToIR(fileName);

    boost::replace_last(fileName, ".php", ".rbc");

    std::ofstream OS(fileName.c_str(), std::ios_base::out|std::ios::trunc|std::ios::binary);
    if (!OS.fail())
        llvm::WriteBitcodeToFile(phpModule, OS);

    //if (OS.fail())
    //    return -1;

    delete phpModule;

}

/**
 * compile to llvm assembly
 */
void pDriver::compileToAsm(string fileName) {
    cout << "compiling " << fileName << " to llvm assembly " << endl;
}

/**
 * compile to native binary
 */
void pDriver::compileToNative(string fileName) {
    cout << "compiling " << fileName << " to native binary " << endl;
}

/**
 *  compile the given file to an llvm::Module, which is returned
 *  the caller is responsible for deleting the module
 */
llvm::Module* pDriver::compileToIR(string fileName) {

    // TODO: llvm example module. this whole project will work better if this
    // thing actually compiles the file in fileName

    // Create the "module" or "program" or "translation unit" to hold the
    // function
    llvm::Module *M = new llvm::Module(fileName);

    // Create the main function: first create the type 'int ()'
    llvm::FunctionType *FT = llvm::FunctionType::get(llvm::Type::VoidTy, std::vector<const llvm::Type*>(),
                                        /*not vararg*/false);

    // By passing a module as the last parameter to the Function constructor,
    // it automatically gets appended to the Module.
    llvm::Function *F = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, "rphp_main", M);

    // Add a basic block to the function... again, it automatically inserts
    // because of the last argument.
    llvm::BasicBlock *BB = llvm::BasicBlock::Create("EntryBlock", F);

    // ** STARTUP **
    // runtime engine pointer type
    const llvm::Type* rEnginePointer = llvm::PointerType::get(llvm::Type::Int8Ty,0);

    // startup function type
    llvm::FunctionType *runtimeStartupFuncType = llvm::FunctionType::get(rEnginePointer, std::vector<const llvm::Type*>(), false);
    // startup function
    llvm::Function *runtimeStartupFunc = llvm::Function::Create(runtimeStartupFuncType, llvm::Function::ExternalLinkage, "rphp_newRuntimeEngine", M);
    // startup instruction call
    llvm::Instruction *runtimeStartInstr = llvm::CallInst::Create(runtimeStartupFunc, "runtime");

    // ** hello world **
    llvm::ArrayType* ArrayTy_0 = llvm::ArrayType::get(llvm::IntegerType::get(8), 12);
    llvm::PointerType* PointerTy_4 = llvm::PointerType::get(llvm::IntegerType::get(8), 0);
    llvm::GlobalVariable* gvar_array__str = new llvm::GlobalVariable(
    /*Type=*/ArrayTy_0,
    /*isConstant=*/true,
    /*Linkage=*/llvm::GlobalValue::InternalLinkage,
    /*Initializer=*/0, // has initializer, specified below
    /*Name=*/".str",
    M);

    // Constant Definitions
    llvm::Constant* const_array_7 = llvm::ConstantArray::get("hello world", true);
    std::vector<llvm::Constant*> const_ptr_8_indices;
    llvm::Constant* const_int32_9 = llvm::Constant::getNullValue(llvm::IntegerType::get(32));
    const_ptr_8_indices.push_back(const_int32_9);
    const_ptr_8_indices.push_back(const_int32_9);
    llvm::Constant* const_ptr_8 = llvm::ConstantExpr::getGetElementPtr(gvar_array__str, &const_ptr_8_indices[0], const_ptr_8_indices.size() );
    llvm::UndefValue* const_int32_10 = llvm::UndefValue::get(llvm::IntegerType::get(32));

    // Global Variable Definitions
    gvar_array__str->setInitializer(const_array_7);

    // argument sig for print function
    std::vector<const llvm::Type*> printSig;
    printSig.push_back(rEnginePointer);
    printSig.push_back(PointerTy_4);
    // print function type
    llvm::FunctionType *printFuncType = llvm::FunctionType::get(llvm::Type::VoidTy, printSig, false);
    // print function
    llvm::Function *printFunc = llvm::Function::Create(printFuncType, llvm::Function::ExternalLinkage, "rphp_print_cstr", M);
    // push args
    std::vector<llvm::Value*> printArgsV;
    printArgsV.push_back(runtimeStartInstr);
    printArgsV.push_back(const_ptr_8);
    // print instruction call
    llvm::Instruction *printInstr = llvm::CallInst::Create(printFunc, printArgsV.begin(), printArgsV.end());


    //  ** SHUTDOWN **
    // argument sig for shutdown function
    std::vector<const llvm::Type*> engineSig(1, rEnginePointer);
    // shutdown function type
    llvm::FunctionType *runtimeDeleteFuncType = llvm::FunctionType::get(llvm::Type::VoidTy, engineSig, false);
    // shutdown function
    llvm::Function *runtimeDeleteFunc = llvm::Function::Create(runtimeDeleteFuncType, llvm::Function::ExternalLinkage, "rphp_deleteRuntimeEngine", M);

    // push args
    std::vector<llvm::Value*> shutdownArgsV;
    shutdownArgsV.push_back(runtimeStartInstr);

    // shutdown instruction call
    llvm::Instruction *runtimeDeleteInstr = llvm::CallInst::Create(runtimeDeleteFunc, shutdownArgsV.begin(), shutdownArgsV.end());

    // Get pointers to the constant integers...
    //llvm::Value *Two = llvm::ConstantInt::get(llvm::Type::Int32Ty, 2);
    //llvm::Value *Three = llvm::ConstantInt::get(llvm::Type::Int32Ty, 3);

    // Create the add instruction... does not insert...
    //llvm::Instruction *Add = llvm::BinaryOperator::create(llvm::Instruction::Add, Two, Three, "addresult");

    // explicitly insert it into the basic block...
    //BB->getInstList().push_back(Add);

    BB->getInstList().push_back(runtimeStartInstr);
    BB->getInstList().push_back(printInstr);
    BB->getInstList().push_back(runtimeDeleteInstr);

    // Create the return instruction and add it to the basic block
    BB->getInstList().push_back(llvm::ReturnInst::Create());
    //BB->getInstList().push_back(llvm::ReturnInst::Create(Add));

    if (llvm::verifyModule(*M, llvm::PrintMessageAction)) {
        cerr << "module corrupt" << endl;
        exit(-1);
    }
    else {
        cerr << "module verified" << endl;
    }

    return M;

}

// dump LLVM IR
void pDriver::dumpIR(string fileName) {

    llvm::Module* phpModule = compileToIR(fileName);
    phpModule->dump();
    delete phpModule;

}


void pDriver::dumpTokens(string fileName) {

    ifstream inFile;

    std::string contents = readFile(fileName);

    pLangTokens tokens;
    pLangLexer lexer(tokens);

    std::string::iterator source_it = contents.begin();
    for (tokIteratorType iter = lexer.begin(source_it, contents.end()); iter != lexer.end(); ++iter)
    {
        std::cout << "tok: " << (*iter).id() << " >" << (*iter).value() << "<" << std::endl;
        if ((*iter).id() == 0)
            break;
    }


}

std::string pDriver::readFile(std::string fileName)
{
    std::ifstream instream(fileName.c_str());
    if (!instream.is_open()) {
        std::cerr << "Couldn't open file: " << fileName << std::endl;
        exit(-1);
    }
    instream.unsetf(std::ios::skipws);
    return std::string(std::istreambuf_iterator<char>(instream.rdbuf()),
                       std::istreambuf_iterator<char>());
}

void pDriver::dumpAST(string fileName) {

    ifstream inFile;

    std::string contents = readFile(fileName);

    pLangTokens tokens;
    pLangLexer lexer(tokens);
    pLangGrammar parser(tokens);

    std::string::iterator it = contents.begin();
    tokIteratorType iter = lexer.begin(it, contents.end());
    tokIteratorType end = lexer.end();

    std::string ws = "WS";

    bool r = phrase_parse(iter, end, parser, in_state(ws)[tokens.skip_toks]);

    if (r && iter == end)
    {
        std::cout << "-------------------------\n";
        std::cout << "Parsing succeeded\n";
        std::cout << "-------------------------\n";
    }
    else
    {
        std::cout << "-------------------------\n";
        std::cout << "Parsing failed\n";
        std::cout << "-------------------------\n";
    }

    /*
    ifstream inFile;

    inFile.open(fileName.c_str(), ifstream::in);
    if (!inFile) {
        cout << "Unable to open file: " << endl;
        exit(1); // terminate with error
    }

    UnicodeString contents;
    char buf[512];
    while (inFile) {
        inFile.getline(buf, 512);
        //cout << "read: " << buf << endl;
        contents += buf;
    }

    inFile.close();

    parser p;
    p.set_token_stream( new parser::token_stream_type() );
    p.set_memory_pool( new parser::memory_pool_type() );
    p.setDebug( true );

    p.tokenize(contents);
    start_ast* phpAst;
    bool matched = p.parse_start(&phpAst);
    if( matched )
    {
        std::cout << "Successfully parsed" << std::endl;
        debug_visitor dv;
        dv.visit_start(phpAst);
    }else
    {
        //*ast = 0;
        //std::cout << p.expected_symbol(ast_node::Kind_start, "start");
        std::cout << "Couldn't parse content" << std::endl;
    }
*/
}


}


