/* ***** BEGIN LICENSE BLOCK *****
 * Roadsend PHP Compiler Runtime Libraries
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 * ***** END LICENSE BLOCK ***** */

#ifndef RPHP_PTYPES_H_
#define RPHP_PTYPES_H_

#include <boost/lexical_cast.hpp>

#include "pVar.h"
#include "pHash.h"
#include "pObject.h"

namespace rphp {

    // a visitor for determining type of pVar
    class pVarTypeChecker : public boost::static_visitor<pVarType> {

    public:

        pVarType operator()(const pTriState &h) const {
            return (pNull(h)) ? pVarNullType : pVarBoolType;
        }

        pVarType operator()(const pInt &i) const {
            return pVarIntType;
        }

        pVarType operator()(const pFloat &i) const {
            return pVarFloatType;
        }

        pVarType operator()(const pBString &str) const {
            return pVarBStringType;
        }

        pVarType operator()(const pUStringP &str) const {
            return pVarUStringType;
        }

        pVarType operator()(const pHashP &h) const {
            return pVarHashType;
        }

        pVarType operator()(const pObjectP &h) const {
            return pVarObjectType;
        }

        pVarType operator()(const pVarRef &p) const {
            return pVarRefType;
        }

    };


    // a visitor for converting to a number (long or float)
    class convertToNumber : public boost::static_visitor<void> {
    protected:
        pVar &var;

    public:
        convertToNumber(pVar &v) : var(v) { }

        void operator()(pTriState &h) const {
                (h) ? var = 1l : var = 0l;
        }

        void operator()(pInt &a) const {
            // nothing, already numeric
        }

        void operator()(pFloat &i) const {
            // nothing, already numeric
        }

        void operator()(pBString &a) const {
            // TODO: handle floats
            try {
                var = boost::lexical_cast<long>(a);
            } catch(boost::bad_lexical_cast &) {
                var = 0l;
            }
        }

        void operator()(pUStringP &a) const {
            // TODO: do a real conversion here
            // should handle both integers and floats
            var = 0l;
        }

        void operator()(pHashP &h) const {
            var = (pInt)h->getSize();
        }
        
        void operator()(pObjectP &h) const {
            var = (pInt)h->getNumProperties();
        }

        void operator()(pVarRef &r) const {
            // unbox
            //boost::apply_visitor(convertToNumber(*r), *r);
        }

    };


    /*
     * convenience accessors
     *
     */

    // convert to number (in place)
    inline pVarType pVar_getType(const pVar &p) {
        return boost::apply_visitor(pVarTypeChecker(), p);
    }

    // convert to number (in place)
    inline void pVar_convertToNumber(pVar &p) {
        boost::apply_visitor(convertToNumber(p), p);
    }

    // get the boolean value of a pVar. does NOT convert so pVar
    // must already be a pTriState
    inline pTriState pVar_getVal_bool(const pVar &p) {
            return boost::get<rphp::pTriState>(p);
    }

    inline long pVar_getVal_int(const pVar &p) {
            return boost::get<pInt>(p);
    }

    inline pVarRef pVar_getVal_ref(const pVar &p) {
            return boost::get<pVarRef>(p);
    }

    /*
     * type conversions
     *
     */
    pVar pVar_castToNumber(const pVar p);
    pVar pVar_add(const pVar lhs, const pVar rhs);

} /* namespace rphp */


#endif /* RPHP_PTYPES_H_ */