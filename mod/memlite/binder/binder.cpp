#include "binder.hpp"
#include "../watcher/bindTag.hpp"
#include "bindTacticable.hpp"
#include "tbindable.inl"

namespace nm {

    NM_DEF_ME(binder)

    binder::binder(const type& type, bindTacticable& tactic): _type(&type.getStatic()), _tactic(&tactic) {}
    binder::binder(const me& rhs): _type(rhs._type), _tactic(rhs._tactic) { _assign(rhs); }
    binder::~binder() { me::rel(); }

    instance* me::operator->() { return &get(); }
    instance& me::operator*() { return get(); }

    me& me::operator=(const me& rhs) {
        if(this == &rhs) return *this;

        _assign(rhs);
        return *this;
    }

    nbool me::isBind() const {
        const bindTag& tag = _getBindTag();
        if(nul(tag)) return false;

        return tag.isBind();
    }

    void me::rel() {
        _tactic->rel(*this);
    }

    id me::getItsId() const { return _itsId; }
    nbool me::canBind(const type& type) const { return getType().isSuper(type); }
    const type& me::getType() const { return *_type; }

    nbool me::bind(const instance& it) {
        rel();
        if(!tbindable<instance>::bind(it)) return false;

        return _tactic->bind(*this, it);
    }

    instance& me::get() {
        return safeGet(_getBindTag(), get());
    }

    nbool me::_assign(const binder& rhs) {
        rel();
        if(nul(rhs)) return true;

        _type = rhs._type;
        if (nul(_tactic))
            _tactic = rhs._tactic;
        return bind(rhs.get());
    }

    nbool me::_onSame(const typeProvidable& rhs) const {
        const me& cast = (const me&) rhs;
        return get() == cast.get();
    }

    bindTag& me::_getBindTag() const {
        return (bindTag&) bindTag::getBindTag(_itsId);
    }

    void* me::cast(const type& to) {
        if(!isBind()) return nullptr;
        return get().cast(to);
    }
}
