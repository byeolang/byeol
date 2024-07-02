#pragma once

#include "../builtin/container/native/tnarr.hpp"
#include "obj.hpp"

namespace nm {

    class _nout args : public narr {
        NAMU(CLASS(args, narr))

    public:
        args();
        args(const baseObj& me);
        args(const narr& rhs);
        args(const baseObj& me, const narr& rhs);

    public:
        /// @param me this should be evaluated already before you pass it to this func.
        const me& setMe(const node& me) const;
        node& getMe() const;

        clonable* cloneDeep() const override;

        /// @remark in order to call this func, you must have prepared thread and frames.
        std::string toStr() const;

    private:
        mutable str _me;
    };
}
