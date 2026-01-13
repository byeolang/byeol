/// @file
#pragma once

#include "core/builtin/container/native/tnarr.hpp"
#include "core/ast/obj.hpp"

namespace by {

    /** @ingroup core
     *  @brief Function or object evaluation arguments
     *  @details Represents arguments passed for `eval()` of functions or objects. Inherits from @ref narr, so all
     *  narr APIs can be used.
     *
     *  @section me_object Me Object
     *  args is characterized by including a `me` object when passed. For example, with functions, the function
     *  fundamentally cannot know from which runtime object the `eval()` request came. Unlike some other languages,
     *  it doesn't create a hidden `thisptr` in args. Through separate `setMe()`, functions or properties using args
     *  can know from which object these args were passed.
     *
     *  Note that @ref baseObj calls `setMe(this)` to place itself in args when `eval(name, args)` is called.
     */
    class _nout args: public narr {
        BY(CLASS(args, narr))

    public:
        args();
        args(const narr& rhs);
        args(const baseObj* me);
        args(const baseObj* me, const narr& rhs);
        args(const me& rhs);

    public:
        /**
         *  @param me this should be evaluated already before you pass it to this func.
         */
        const me& setMe(const node& me) const;
        const me& setMe(const node* me) const BY_SIDE_FUNC(me, setMe(*me), *this);
        node* getMe() const;

        /**
         *  @remark in order to call this func, you must have prepared thread and frames.
         */
        std::string toStr() const;

        tmay<me> evalAll(const params& ps) const;
        tmay<me> evalAll(const params* it) const BY_SIDE_FUNC(evalAll);

    private:
        mutable str _me;
    };
}
