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

#ifndef RPHP_PMODULE_H_
#define RPHP_PMODULE_H_

#include "pAST.h"

namespace rphp {

// encapsulates a single php "module" (one script)
class pModule {

private:
    std::string originalFileName;
    AST::treeTop* ast;

public:
    pModule(std::string fileName): originalFileName(fileName), ast(new AST::treeTop())
    {

    }
    
    ~pModule() { delete ast; }

    AST::treeTop* getTreeTop() { return ast; }

    void lowerToIR();
    void writeBitcode(std::string fileName);
    void dumpAST();


};

} // namespace

#endif /* RPHP_PMODULE_H_ */
