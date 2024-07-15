#include "ases.hpp"
#include "../../ast/node.hpp"
#include "../../builtin/primitive/nVoid.hpp"

namespace nm {

    NM_DEF_ME(ases)

    me::~ases() {
        rel();
    }

    str me::as(const node& from, const type& to) const {
        const type& fromType = from.getType();
        if(to.isSuper(fromType)) return str(from);

        for(auto e : _casts)
            if(e->is(fromType, to))
                return e->as(from, to);
        // there is no null in namulang:
        //  returns void if no valid casting found.
        return str(nVoid::singletone());
    }

    void me::rel() {
        for(auto* e : _casts)
            delete e;
        _casts.clear();
    }

    void me::add(const asable& newCast) {
        _casts.push_back((asable*) &newCast);
    }

    void me::add(const asable* newCast) {
        _casts.push_back((asable*) newCast);
    }

    int me::len() const {
        return _casts.size();
    }

    nbool me::is(const type& from, const type& to) const {
        if(to.isSuper(from)) return true;

        for(auto e : _casts)
            if(e->is(from, to)) return true;
        return false;
    }
}
