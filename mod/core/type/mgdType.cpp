#include "mgdType.hpp"

namespace nm {

    NM(DEF_ME(mgdType))

    me::mgdType(const std::string& name): _name(name) {
        _initSupers(ttype<obj>::get());
    }
    me::mgdType(const std::string& name, const mgdType& super): _name(name) {
        _initSupers(super);
    }
    me::mgdType(const std::string& name, const narr& beans): _name(name) {
        _initSupers(ttype<obj>::get());
        getBeans() = beans;
    }
    me::mgdType(const std::string& name, const mgdType& super, const narr& beans): _name(name) {
        _initSupers(super);
        getBeans() = beans;
    }
    me::mgdType(const std::string& name, const types& supersFromRhs): _name(name), _supers(supersFromRhs) {}

    const type& me::getSuper() const {
        return *_supers[_supers.size()-1];
    }

    const nbool& me::isInit() const {
        static nbool inner = true;
        return inner;
    }

    const std::string& me::getName() const { return _name; }

    types& me::_getSupers() {
        return _supers;
    }

    void me::_initSupers(const type& super) {
        const types& supers = super.getSupers();
        _supers.insert(_supers.end(), supers.begin(), supers.end());
        _supers.push_back((type*) &super);
    }
}
