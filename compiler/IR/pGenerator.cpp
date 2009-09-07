/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; Copyright (c) 2008-2009 Shannon Weyrick <weyrick@roadsend.com>
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

#include "rphp/IR/pGenerator.h"

#include "rphp/IR/pCompileError.h"
#include "rphp/IR/pGenSupport.h"
#include "rphp/IR/pDeclare.h"
#include "rphp/IR/pCodeGen.h"

#include <llvm/Module.h>
#include <llvm/Linker.h>
#include <llvm/Support/IRBuilder.h>

using namespace llvm;

namespace rphp { namespace IR {

pGenerator::pGenerator(pSourceModule& mod):
    llvmModule_(new Module(mod.fileName(), getGlobalContext())),
    sourceModule_(mod),
    entryFunction_(NULL),
    initFunction_(NULL),
    IRHelper_(llvmModule_)
{

    loadAndLinkRuntimeIR();
    createEntryPoint();
    runPasses();

}

void pGenerator::runPasses() {

    assert(entryFunction_ != NULL);
    assert(initFunction_ != NULL);

    pDeclare* declarePass = new pDeclare(llvmModule_, initFunction_);
    sourceModule_.applyVisitor(declarePass);
    delete declarePass;

    // declare is over, terminate init function
    // @TODO: probably wrong, as a) the _entry_ block is terminated
    // and b) the way of creating the ret is not really good...
    initFunction_->getEntryBlock().getInstList().push_back(ReturnInst::Create(getGlobalContext()));

    AST::statementList& topStmts = sourceModule_.getAST();
    pCodeGen* codeGenPass;

    // first codegen all declarations (i.e. functions, classes and methods)
    for (AST::statementList::iterator i = topStmts.begin();
         i != topStmts.end();
         ++i) {
        if ((*i)->getKind() == AST::functionDeclKind) {
            AST::functionDecl* f = static_cast<AST::functionDecl*>(*i);
            codeGenPass = new pCodeGen(llvmModule_,
                                       pGenSupport::mangleUserFunctionName(llvmModule_->getModuleIdentifier(),
                                                                           f->functionDef()->name()));
            codeGenPass->visit(f->body());
            delete codeGenPass;
        }
    }

    // now global
    codeGenPass = new pCodeGen(llvmModule_, entryFunction_->getName());
    sourceModule_.applyVisitor(codeGenPass);
    delete codeGenPass;

    //verifyModule(*llvmModule_, PrintMessageAction);

}

void pGenerator::loadAndLinkRuntimeIR(void) {

    std::string errMsg;
    Module* irMod = pGenSupport::getRuntimeIR();

    // Linker will take ownership of irMod

    // here we link the irRuntime.ir with our clean module so it already includes
    // definitions of types and functions we need for code generation
    Linker l(llvmModule_->getModuleIdentifier()+"_link", llvmModule_);
    l.LinkInModule(irMod, &errMsg);
    if (errMsg.length()) {
        throw pCompileError("error linking in runtime IR [" + errMsg + "]");
    }

    // take ownership of module so it's not freed
    l.releaseModule();

}

void pGenerator::createEntryPoint(void) {

    // entry function
    entryFunction_ = Function::Create(IRHelper_.moduleEntryFunType(),
                                      Function::ExternalLinkage,
                                      pGenSupport::mangleEntryFunctionName(llvmModule_->getModuleIdentifier()),
                                      llvmModule_);

    Function::arg_iterator a = entryFunction_->arg_begin();
    (*a).setName("funRetVal");
    Value* runtime = ++a;
    runtime->setName("rEngine");

    // module init function
    initFunction_ = Function::Create(IRHelper_.moduleInitFunType(),
                                     Function::ExternalLinkage,
                                     pGenSupport::mangleInitFunctionName(llvmModule_->getModuleIdentifier()),
                                     llvmModule_);
    initFunction_->arg_begin()->setName("rEngine");
    BasicBlock::Create(getGlobalContext(), "entry", initFunction_);

    // module entry, entry block. exit is created in pCodeGen finalize
    IRBuilder<> block(getGlobalContext());
    block.SetInsertPoint(BasicBlock::Create(getGlobalContext(), "entry", entryFunction_));

    // call init function
    block.CreateCall(initFunction_, runtime);

}

const pIdentString pGenerator::entryFunctionName(void) const {
    if (!entryFunction_)
        return "";
    return entryFunction_->getName();
}

} } // namespace

