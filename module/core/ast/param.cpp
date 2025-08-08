#include "core/ast/param.hpp"

#include "core/ast/node.hpp"

namespace by {
    NM_DEF_ME(param)

    me::param(const std::string& newName, const node& org): _name(newName) { setOrigin(org); }

    me::param(const std::string& newName, const node* org): _name(newName) { setOrigin(*org); }

    nbool me::operator==(const me& rhs) const {
        return getOrigin().getType() == rhs.getOrigin().getType();
    }

    nbool me::operator!=(const me& rhs) const { return !operator==(rhs); }

    void me::setOrigin(const node& newType) { _org.bind(newType); }

    const std::string& me::getName() const { return _name; }

    void me::setName(const std::string& new1) { _name = new1; }

    const node& me::getOrigin() const { return *_org; }

    void me::onCloneDeep(const clonable& new1) {
        me& rhs = (me&) new1;
        _org.bind((node*) (rhs._org ? rhs._org->cloneDeep() : nullptr));
    }
} // namespace by
