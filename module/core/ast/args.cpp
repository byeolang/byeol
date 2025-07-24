#include "core/ast/args.hpp"

#include "core/ast/exprs/getExpr.hpp"
#include "core/ast/closure.hpp"

namespace nm {

    NM(DEF_ME(args))

    me::args() {}

    me::args(const narr& rhs): super(rhs) {}

    me::args(const baseObj* me, const narr& rhs): super(rhs), _me(me) {}

    const me& me::setMe(const node& me) const {
        _me.bind(me);
        return *this;
    }

    node* me::getMe() const { return _me.get(); }

    std::string me::toStr() const {
        int n = 0;
        std::string msg;
        each([&](const auto& val) {
            return msg += util::getEvalTypeFrom(val) + (++n >= len() ? "" : ","), true;
        });
        return msg;
    }

    tmay<me> me::evalAll(const params& ps) const {
        tmay<me> res(me{});
        WHEN(len() != ps.len())
            .err("length of args(%d) and typs(%d) doesn't match.", len(), ps.len())
            .retMay<me>();

        int n = 0;
        for(const node& e: *this) {
            const param& p = ps[n++];
            str evaluated = closure::make(e);
            if(!evaluated) evaluated = e.asImpli(*p.getOrigin().as<node>());
            WHEN(!evaluated)
                .err("evaluation of arg[%s] -> param[%s] has been failed.", e, p.getOrigin())
                .retMay<me>();
            res->add(*evaluated);
        }

        res->setMe(getMe());
        return res;
    }
} // namespace nm
