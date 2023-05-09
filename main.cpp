#include "llvm/Support/raw_ostream.h"
 #include "lexer.h"
 #include "driver.hpp"
 #include "parser.hpp"
 #include "ast.hpp"
 #include "astPrinter.hpp"
 #include "semantic.hpp"
 #include "codeGenerator.hpp"
// #include "llvm/IR/IRBuilder.h"
extern ast::Program *program;

namespace generator
{
    class CodeGenerator : public ast::Visitor
    {
    public:
        llvm::IRBuilder<> m_builder;
    };
}

int main(int argc, char **argv)
{
    carbonic_c::Driver driver;
    // while (lexer->yylex() != 0)
    // driver.parse_args(argc, argv);
    int x = driver.parse_program();

    std::cout << std::endl;
    // // analyzer::AstPrinter printer;
    // // program->accept(&printer);
    // analyzer::Semantic analyzer;
    // program->accept(&analyzer);
    // prettyPrinter::codePrinter printer;
    // program->accept(&printer);

    // // create function signature
    // llvm::LLVMContext context;
    // std::unique_ptr<llvm::Module> module = std::make_unique<llvm::Module>("name", context);
    // llvm::FunctionType *funcType = llvm::FunctionType::get(llvm::Type::getInt32Ty(context), {llvm::Type::getInt32Ty(context)}, false);
    // llvm::Function *func = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "return_input", module.get());
    // // create basic block and set insertion point
    // llvm::BasicBlock *bb = llvm::BasicBlock::Create(context, "entry", func);
    // llvm::IRBuilder<> builder(bb);
    // // create function argument
    // llvm::Value *arg = func->arg_begin();
    // arg->setName("input");
    // // return input argument
    // builder.CreateRet(arg);
    // // print LLVM IR code
    // module->print(llvm::errs(), nullptr);
    generator::codeGenerator code_generator;
    program->accept(&code_generator);
    return 0;
}