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

    clonable* me::cloneDeep() const {
        me* ret = new me();
        for(auto e = this->begin(); e; e++)
            ret->add((node*) e->clone());

        ret->_me = _me;
        return ret;
    }

    namespace {
        std::string getEvalTypeFrom(const node& value) {
            if(nul(value)) return "null";
            str eval = value.getEval();
            if(eval) return eval->getType().getName();

            const auto& name = safeGet(value.cast<getExpr>(), getName());
            if(!nul(name)) return name;
            return value.getType().getName();
        }
    }

    std::string me::toStr() const {
        int n = 0;
        std::string msg;
        each([&](const auto& val) {
            return msg += getEvalTypeFrom(val) + (++n >= len() ? "" : ","), true;
        });
        return msg;
    }
} // namespace nm
