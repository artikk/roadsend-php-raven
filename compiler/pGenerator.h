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

#ifndef RPHP_PGENERATOR_H_
#define RPHP_PGENERATOR_H_

#include "pASTVisitors.h"

namespace rphp {

class pGenerator: public AST::defaultVisitor {
public:
    void visit(AST::treeTop*);
    void visit(AST::statementNode*);
    void visit(AST::echoNode*);

};

} // namespace

#endif /* RPHP_PGENERATOR_H_ */
