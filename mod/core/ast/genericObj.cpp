#include "genericObj.hpp"
#include "../visitor/generalizer.hpp"
#include "args.hpp"
#include "../visitor/verifier.hpp"
#include "dumScope.hpp"
#include "baseFunc.hpp"
#include "../type/mgdType.hpp"

namespace nm {

    NM(DEF_ME(genericObj), DEF_VISIT())

    me::genericObj(const obj& orgObj, const strings& paramNames): _orgObj(orgObj),
            _paramNames(paramNames) {}

    const obj& me::getOrigin() const {
        return *_orgObj;
    }

    const me::strings& me::getParamNames() const {
        return _paramNames;
    }

    str me::getEval() const {
        return _orgObj;
    }

    const std::map<std::string, tstr<obj>>& me::getCache() const {
        return _cache;
    }

    void me::inFrame(const bicontainable& args) {}

    void me::outFrame(const bicontainable& args) {}

    str me::_onRunSub(node& sub, const args& a) {
        return str();
    }

    scope& me::subs() {
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

        if(!_cache.count(key))
            _cache.insert({key, _makeGeneric(key, a)});
        return _cache[key];
    }

    std::string me::_makeKey(const args& a) const {
        if(a.len() != _paramNames.size()) return NM_E("len of args doesn't match to _paramNames"), std::string();
        return a.asStr();
    }

    /// make a generic object.
    tstr<obj> me::_makeGeneric(const std::string& argName, const args& a) const {
        if(!_orgObj) return NM_E("_orgObj is null"), tstr<obj>();

        std::string name = _orgObj->getType().getName() + "<" + argName + ">";
        NM_DI("|==========================================|");
        NM_DI("|--- generic: make %s generic class ---|", name);
        tstr<obj> ret = (obj*) _orgObj->cloneDeep(); // clone all of shares including func.
        src* s = new src(_orgObj->getSrc());
        s->_setName(name);
        ret->_setSrc(*s);
        // update origin:
        //  genericObj makes an origin object. but ret->getOrigin() is pointing _orgObj now.
        //  I need to update it.
        ret->_setOrigin(&ret.get());
        // clone type:
        if(ret->_type) {
            mgdType* newType = new mgdType(name, ret->_type->getSupers());
            newType->getBeans() = a;
            ret->_setType(newType);
        }

        ncnt n = 0;
        generalizer g;
        for(auto& e : a)
            g.add(*new param(_paramNames[n++], e));

        g.setFlag(generalizer::INTERNAL).setTask(*ret).work();
        NM_DI("|============================|");
        return ret;
    }
}
