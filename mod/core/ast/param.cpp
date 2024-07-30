#include "param.hpp"

#include "node.hpp"

namespace nm {
    NM_DEF_ME(param)

    me::param(const std::string& newName, const node& org): _name(newName) { setOrigin(org); }

    me::param(const std::string& newName, const node* org): _name(newName) { setOrigin(*org); }

    void me::setOrigin(const node& newType) { _org.bind(newType); }

    const std::string& me::getName() const { return _name; }

    void me::setName(const std::string& new1) { _name = new1; }

    const node& me::getOrigin() const { return *_org; }

    clonable* me::cloneDeep() const {
        return new me(_name, _org ? (node*) _org->cloneDeep() : nullptr);
    }
}
