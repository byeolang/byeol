#include "defArrayExpr.hpp"
#include "../../visitor/visitor.hpp"
#include "../../builtin/container/mgd/arr.hpp"

namespace namu {

    NAMU(DEF_ME(defArrayExpr), DEF_VISIT())

    me::defArrayExpr(const node& type): _type(type) {}
    me::defArrayExpr(const narr& elems): _elems(elems) {}

    str me::run(const args& a) {
        arr& ret = *new arr(getArrayType());

        for(const node& elem : _elems)
            ret.add(*elem.as<node>());

        return ret;
    }

    const node& me::getArrayType() const {
        return getOrigin().getType().getBeans()[0];
    }

    const arr& me::getOrigin() const {
        if(!_org) {
            str typ = _type ? _type->as<node>() : *_deduceElems();
            _org.bind(new arr(*typ));
            for(const node& e : _elems)
                _org->add(e);
        }

        return *_org;
    }

    str me::_deduceElems() const {
        ncnt len = _elems.len();
        NAMU_DI("deduceElems: len[%d]", len);
        if(!len) return NAMU_DI("len == 0. deduced type as 'void'"), nVoid::singletone();

        str ased1 = _elems[0].getEval();
        const node* ret = &ased1.get();
        if(!ret)
            return NAMU_DI("deduceElem: elem0 is 'void'. deduced type as 'void'"), nVoid::singletone();
        str ased;

        for(int n=1; n < len; n++) {
            ased = _elems[n].as<node>();
            ret = &ret->deduce(*ased);
            NAMU_DI("deduceElem: prevElem + elem%d[%s] --> %s",
                    n, ased->getType().getName().c_str(), ret ? ret->getType().getName().c_str() : "null");
            if(!ret)
                return NAMU_DI("deduceElem: elem%d was void. deduced type as 'void'", n), nVoid::singletone();
        }

        return *ret;
    }

    str me::getEval() const {
        return str(getOrigin());
    }

    narr& me::getElems() {
        return _elems;
    }
}
