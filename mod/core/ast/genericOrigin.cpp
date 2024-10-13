#include "genericOrigin.hpp"

#include "../type/mgdType.hpp"
#include "../worker/visitor/generalizer.hpp"
#include "../worker/visitor/verifier.hpp"
#include "args.hpp"
#include "baseFunc.hpp"
#include "dumScope.hpp"
#include "origin.hpp"

namespace nm {

    NM(DEF_ME(genericOrigin), DEF_VISIT())

    me::genericOrigin(const origin& org, const strings& paramNames):
        _org(org), _paramNames(paramNames) {}

    const baseObj& me::getOrigin() const { return *_org; }

    const me::strings& me::getParamNames() const { return _paramNames; }

    str me::getEval() const { return _org; }

    const std::map<std::string, tstr<obj>>& me::getCache() const { return _cache; }

    void me::inFrame(const bicontainable& args) const {}

    void me::outFrame(const bicontainable& args) const {}

    str me::_onRunSub(node& sub, const args& a) { return str(); }

    scope& me::subs() {
        if(_org) return _org->subs();

        static dumScope inner;
        return inner;
    }

    priorType me::prioritize(const args& a) const {
        std::string key = _makeKey(a);
        if(key.empty()) return NO_MATCH;
        if(!_cache.count(key)) return NO_MATCH;
        // after verification, making GenericObject is supressed.

        return EXACT_MATCH;
    }

    str me::run(const args& a) {
        std::string key = _makeKey(a);
        if(key.empty()) return NM_E("key is empty"), tstr<obj>();

        if(!_cache.count(key)) _makeGeneric(key, params::make(_paramNames, a));
        return _cache[key];
    }

    std::string me::_makeKey(const args& a) const {
        if(a.len() != _paramNames.size())
            return NM_E("len of args doesn't match to _paramNames"), std::string();
        return a.asStr();
    }

    /// make a generic object.
    tstr<obj> me::_makeGeneric(const std::string& argName, const params& ps) {
        if(!_org) return NM_E("_orgObj is null"), tstr<obj>();

        const ntype& orgType = _org->getType();
        tstr<obj> ret = (obj*) _org->cloneDeep(); // clone all of shares including func.
        src* s = new src(_org->getSrc());
        s->_setName(orgType.getName());
        ret->_setSrc(*s);

        // clone type:
        mgdType newType(orgType.getName(), ret->getType().getSupers());
        newType.getParams().add(ps);
        ret->_setType(newType);

        NM_DI("|==========================================|");
        NM_DI("|--- generic: make %s generic obj[%s] ---|", newType.createNameWithParams(),
            platformAPI::toAddrId(&ret.get()));
        NM_DI("|--- generic: make cache %s ---|", argName);

        // prevent infinite loop:
        //  now I'll run generalizer but sometimes generalizer asking this class to get generic
        //  object which is being creating. prevent such situation, I need to put a instance into
        //  cache first.
        _cache.insert({argName, ret});

        generalizer().add(*_org).add(ps).setFlag(generalizer::INTERNAL).setTask(*ret).work();

        NM_DI("|============================|");
        return ret;
    }
} // namespace nm
