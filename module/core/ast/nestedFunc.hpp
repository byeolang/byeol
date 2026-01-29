/// @file
#pragma once

#include "core/ast/func.hpp"

namespace by {
    /** @ingroup core
     *  @brief Nested function with captured scope
     *  @details Function that captures local scope at creation time.
     *  Clones function with shallow copied block scope for variable capture in @ref closure "closures".
     *  @note Created from @ref func with captured local variables, doesn't see variables defined after creation.
     */
    class _nout nestedFunc: public func {
        BY(ME(nestedFunc, func), CLONE(nestedFunc))

    public:
        explicit nestedFunc(const func& org, const scope& subs);

    protected:
        /**
         * @brief Overrides base class to handle frame interaction and local scope capture for nested functions.
         * @details This method ensures that the nested function correctly captures and maintains its local
         *          scope from its creation context, which is crucial for closures.
         * @param meObj The "me" object (context) for this nested function call.
         * @param s The scope associated with this function.
         * @param exN An index related to exception handling.
         * @return An `str` object representing the result of the frame interaction.
         */
        str _interactFrame(node& meObj, scope& s, nidx exN) override;
    };
}
