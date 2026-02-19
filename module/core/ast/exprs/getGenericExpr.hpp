/// @file
#pragma once

#include "core/ast/exprs/getExpr.hpp"

namespace by {

    class visitor;
    class genericOrigin;

    /** @ingroup core
     *  @brief Generic type access expression
     *  @details Uses @ref genericOrigin objects to create generic types.
     *
     *  @section lazy_but_not_lazy Lazy but Not Lazy
     *  genericOrigin lazily creates generic types using `eval()` with given type parameters. However, since they are
     *  created in advance by the @ref verifier before program execution, they aren't actually created lazily.
     */
    class _nout getGenericExpr: public getExpr {
        BY(CLASS(getGenericExpr, getExpr, expr::exprType), VISIT())

    public:
        getGenericExpr(const std::string& genericName, const args& typeParams);
        getGenericExpr(const node& me, const std::string& genericName, const args& typeParams);

    protected:
        /**
         * @brief Protected virtual method for resolving generic type access.
         * @details This method is responsible for instantiating or retrieving the appropriate
         *          generic type based on the base object and type parameters.
         * @param me The base object on which the generic type is being accessed.
         * @return A pointer to the resolved generic type node, or nullptr if not found.
         */
        node* _onGet(node& me) const override;
    };
}
