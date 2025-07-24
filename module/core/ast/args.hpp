#pragma once

#include "core/builtin/container/native/tnarr.hpp"
#include "core/ast/obj.hpp"

namespace nm {

    class _nout args: public narr {
        NM(CLASS(args, narr))

    public:
        args();
        args(const narr& rhs);
        args(const baseObj* me, const narr& rhs);
        args(const me& rhs): super((const super&) rhs), _me(rhs._me) {
            std::cout << "args::args(me&): this == " << (void*) getMe() << ", rhs == " << rhs.getMe() << "\n";
        }

    public:
        /// @param me this should be evaluated already before you pass it to this func.
        const me& setMe(const node& me) const;
        const me& setMe(const node* me) const NM_SIDE_FUNC(me, setMe(*me), *this);
        node* getMe() const;

        /// @remark in order to call this func, you must have prepared thread and frames.
        std::string toStr() const;

        tmay<me> evalAll(const params& ps) const;
        tmay<me> evalAll(const params* it) const NM_SIDE_FUNC(evalAll);

    private:
        mutable str _me;
    };
}
