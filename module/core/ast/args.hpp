#pragma once

#include "core/builtin/container/native/tnarr.hpp"
#include "core/ast/obj.hpp"

namespace nm {

    class _nout args: public narr {
        NM(CLASS(args, narr))

    public:
        args();
        args(const baseObj& me);
        args(const narr& rhs);
        args(const baseObj& me, const narr& rhs);

        /// @param  elems   instances to derived type of T.
        ///                 should be created on Heap.
        ///         me      this's nullable.
        template <typename... Es> explicit args(const baseObj& me, const Es&... elems): _me(me) {
            static_assert(areBaseOfT<node, Es...>::value,
                "some of type of args are not base of type 'node'");
            add({(node*) &elems...});
        }

    public:
        /// @param me this should be evaluated already before you pass it to this func.
        const me& setMe(const node& me) const;
        const me& setMe(const node* me) const NM_SIDE_FUNC(me, setMe(*me), *this);
        node& getMe() const;

        /// @remark in order to call this func, you must have prepared thread and frames.
        std::string toStr() const;

        tmay<me> evalAll(const params& ps) const;
        tmay<me> evalAll(const params* it) const NM_SIDE_FUNC(evalAll);

    private:
        mutable str _me;
    };
}
