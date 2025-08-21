/// @file
#pragma once

#include "core/builtin/container/native/tnarr.hpp"
#include "core/ast/obj.hpp"

namespace by {

    /// @ingroup core
    /// @brief Function call arguments container
    /// @details Container for function call arguments with 'me' object support.
    /// Manages argument evaluation, type checking, and parameter matching for function calls.
    class _nout args: public narr {
        BY(CLASS(args, narr))

    public:
        args();
        args(const narr& rhs);
        args(const baseObj* me);
        args(const baseObj* me, const narr& rhs);
        args(const me& rhs);

    public:
        /// @param me this should be evaluated already before you pass it to this func.
        const me& setMe(const node& me) const;
        const me& setMe(const node* me) const BY_SIDE_FUNC(me, setMe(*me), *this);
        node* getMe() const;

        /// @remark in order to call this func, you must have prepared thread and frames.
        std::string toStr() const;

        tmay<me> evalAll(const params& ps) const;
        tmay<me> evalAll(const params* it) const BY_SIDE_FUNC(evalAll);

    private:
        mutable str _me;
    };
}
