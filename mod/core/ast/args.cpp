#include "args.hpp"

#include "exprs/getExpr.hpp"

namespace nm {

    NM(DEF_ME(args))

    me::args() {}

    me::args(const baseObj& me): _me(me) {}

    me::args(const narr& rhs): super(rhs) {}

    me::args(const baseObj& me, const narr& rhs): super(rhs), _me(me) {}

    const me& me::setMe(const node& me) const {
        _me.bind(me);
        return *this;
    }

    node& me::getMe() const { return *_me; }

    std::string me::toStr() const {
        int n = 0;
        std::string msg;
        each([&](const auto& val) {
            return msg += util::getEvalTypeFrom(val) + (++n >= len() ? "" : ","), true;
        });
        return msg;
    }
} // namespace nm
