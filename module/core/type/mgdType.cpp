#include "core/type/mgdType.hpp"
#include "core/ast/obj.hpp"

namespace by {

    BY(DEF_ME(mgdType))

    me::mgdType(const std::string& name, const type& super, const params& ps):
        me(name, super, ps, false, nullptr) {}

    me::mgdType(const std::string& name, const type& super, const params& ps, nbool isAdt,
        const node* ret):
        _name(name), _params(ps), _isAdt(isAdt), _ret(ret) {
        _initSupers(super);
    }

    me::mgdType(const std::string& name, const types& supersFromRhs):
        _name(name), _supers(supersFromRhs) {}

    me::mgdType(const std::string& name, const types& supers, const types& subs, const params& ps,
        nbool isAdt):
        _name(name), _supers(supers), _subs(subs), _params(ps), _isAdt(isAdt) {}

    nbool me::isTemplate() const { return false; }

    nbool me::isAbstract() const { return _isAdt; }

    const type& me::getSuper() const { return *_supers[_supers.size() - 1]; }

    const nbool& me::isInit() const {
        static nbool inner = true;
        return inner;
    }

    const std::string& me::getName() const { return _name; }

    ncnt me::size() const { return 0; }

    void me::onCloneDeep(const clonable& from) {
        const me& rhs = (const me&) from;

        // TODO: you may need to cloneDeep those vectors, _supers, _subs.
        _params.onCloneDeep(rhs._params);
        if(rhs._ret) _ret.bind((node*) rhs._ret->cloneDeep());
    }

    params& me::getParams() { return _params; }

    void* me::make() const { return nullptr; }

    const node* me::getRet() const { return _ret.get(); }

    void me::setRet(const node& new1) { _ret.bind(new1); }

    types& me::_getSupers() { return _supers; }

    types& me::_getSubs() { return _subs; }

    type& me::_getStatic() const { return (type&) *this; }

    types** me::_onGetLeafs() const {
        static types* inner = nullptr;
        return &inner;
    }

    void me::_initSupers(const type& super) {
        const types& supers = super.getSupers();
        _supers.insert(_supers.end(), supers.begin(), supers.end());
        _supers.push_back((type*) &super);
    }
} // namespace by
