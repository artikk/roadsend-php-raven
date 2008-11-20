/* ***** BEGIN LICENSE BLOCK *****
;; Roadsend PHP Compiler
;;
;; Copyright (c) 2008 Shannon Weyrick <weyrick@roadsend.com>
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

#ifndef RPHP_PLEXER_H_
#define RPHP_PLEXER_H_

#include "pSourceFile.h"
#include "pLangLexerDef.h"

namespace rphp { namespace lexer {

class pLexer {

private:
    pLangTokens tokens_;
    pLangLexer lexer_;

    const pSourceFile* source_;
    pSourceString contents_;

    pSourceCharIterator sourceBegin_;
    pSourceCharIterator sourceEnd_;

public:

    typedef pTokenIterator iterator_type;

    pLexer(const pSourceFile* s);

    bool preprocess(void);

    pTokenIterator tokBegin(void);
    pTokenIterator tokEnd(void);

    const pSourceCharIterator sourceBegin(void) const;
    const pSourceCharIterator sourceEnd(void) const;

    void dumpTokens(void);
    const wchar_t* getTokenDescription(const std::size_t t) const;

    const pSourceString& contents(void) const { return contents_; }

    pLangTokens& getTokens(void) {
        return tokens_;
    }

};

} } // namespace

#endif /* RPHP_PLEXER_H_ */
