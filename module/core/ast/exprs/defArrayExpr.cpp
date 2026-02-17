#include "core/ast/exprs/defArrayExpr.hpp"

#include "core/builtin/container/mgd/arr.hpp"
#include "core/worker/visitor/visitor.hpp"

namespace by {

    BY(DEF_ME(defArrayExpr), DEF_VISIT())

    me::defArrayExpr(const node& type): _type(type) {}

    me::defArrayExpr(const narr& elems): _elems(elems) {}

    str me::eval(const args& a) {
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
            tstr<baseObj> typ = _type ? _type->as<baseObj>() : *_promoteElems();
            _org.bind(new arr(*typ));
            for(const node& e: _elems)
                _org->add(e);
        }

        return *_org;
    }

    tstr<baseObj> me::_promoteElems() const {
        ncnt len = _elems.len();
        BY_DI("promoteElems: len[%d]", len);
        WHEN(!len) .info("len == 0. promoted type as 'void'").ret(nVoid::singleton());

        str retLife = _elems[0].infer() OR.info("promoteElem: elem0 is null").ret(nVoid::singleton());
        const node* ret = retLife.get();
        str ased;
        for(int n = 1; n < len; n++) {
            ased = _elems[n].as<node>();
            ret = ret->promote(*ased);
            BY_DI("promoteElem: prevElem + elem%d[%s] --> %s", n, ased, ret);
            WHEN_NUL(ret).info("promoteElem: elem%d was null.", n).ret(nVoid::singleton());
        }

        return ret->cast<baseObj>();
    }

    str me::infer() const { return str(getOrigin()); }

    narr& me::_getElems() { return _elems; }
} // namespace by
