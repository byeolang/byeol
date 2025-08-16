#include "core/ast/exprs/defArrayExpr.hpp"

#include "core/builtin/container/mgd/arr.hpp"
#include "core/worker/visitor/visitor.hpp"

namespace by {

    BY(DEF_ME(defArrayExpr), DEF_VISIT())

    me::defArrayExpr(const node& type): _type(type) {}

    me::defArrayExpr(const narr& elems): _elems(elems) {}

    str me::run(const args& a) {
        auto* arrType = getArrayType();
        arr& ret = arrType ? *new arr(*arrType) : *new arr();

        for(const node& elem: _elems)
            ret.add(*elem.as<node>());

        return ret;
    }

    const baseObj* me::getArrayType() const {
        return getOrigin().getType().getParams() TO(get(0)) TO(getOrigin()) TO(template as<baseObj>().get());
    }

    const arr& me::getOrigin() const {
        if(!_org) {
            tstr<baseObj> typ = _type ? _type->as<baseObj>() : *_deduceElems();
            _org.bind(new arr(*typ));
            for(const node& e: _elems)
                _org->add(e);
        }

        return *_org;
    }

    tstr<baseObj> me::_deduceElems() const {
        ncnt len = _elems.len();
        BY_DI("deduceElems: len[%d]", len);
        WHEN(!len) .info("len == 0. deduced type as 'void'").ret(nVoid::singleton());

        str retLife = _elems[0].getEval() OR.info("deduceElem: elem0 is null").ret(nVoid::singleton());
        const node* ret = retLife.get();
        str ased;
        for(int n = 1; n < len; n++) {
            ased = _elems[n].as<node>();
            ret = ret->deduce(*ased);
            BY_DI("deduceElem: prevElem + elem%d[%s] --> %s", n, ased, ret);
            WHEN_NUL(ret).info("deduceElem: elem%d was null.", n).ret(nVoid::singleton());
        }

        return ret->cast<baseObj>();
    }

    str me::getEval() const { return str(getOrigin()); }

    narr& me::getElems() { return _elems; }
} // namespace by
