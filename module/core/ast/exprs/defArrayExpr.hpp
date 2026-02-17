/// @file
#pragma once

#include "core/builtin/container/mgd/arr.hpp"
#include "core/ast/expr.hpp"

namespace by {

    /** @ingroup core
     *  @brief Array literal expression
     *  @details Handles array literal expressions. The array type is inferred through type promotion (`promote()`)
     *  of the literal constant types written as array elements.
     *
     *  @remark arr class
     *  @ref tnarr is the class handling arrays in the native environment. @ref arr extends tnarr for the managed
     *  environment. This allows similar APIs to be used in both C++ and byeol. See arr and tnarr for details.
     */
    class _nout defArrayExpr: public expr {
        BY(CLASS(defArrayExpr, expr, expr::exprType), VISIT())
        friend class coreInternal;

    public:
        defArrayExpr(const node& type);
        defArrayExpr(const narr& elems);

    public:
        using super::eval;
        str eval(const args& a) override;

        str infer() const override;
        /**
         * @brief Gets the type of the array elements after type promotion.
         * @details The type is inferred by promoting the types of all elements in the array literal.
         * @return A const pointer to the `baseObj` representing the inferred array element type.
         */
        const baseObj* getArrayType() const;
        const arr& getOrigin() const;
        const narr& getElems() const BY_CONST_FUNC(_getElems())

    private:
        tstr<baseObj> _promoteElems() const;
        narr& _getElems();

    private:
        str _type;
        narr _elems;
        mutable tstr<arr> _org;
    };
}
