#include "leaf/ast/leaf.hpp"
#include "leaf/ast/nulLeaf.hpp"

namespace nm {

    NM_DEF_ME(leaf)

    me* me::sub(const std::string& name) {
        me& ret = _subs[name] OR.ret(nullptr);
        return &ret;
    }

    me* me::sub(nidx n) { return std::next(begin(), n)->second.get(); }

    me::leaf(std::initializer_list<me*> subs, const std::string& name): _name(name) { add(subs); }

    me::leaf(const me& rhs, const std::string& name): super(), _subs(rhs._subs), _name(name) {}

    me::leaf(const std::string& name): _name(name) {}


    me::operator nbool() const { return isExist(); }

    nbool me::has(const std::string& name) const { return _subs.find(name) != _subs.end(); }

    void me::add(const leaf& new1) {
        WHEN_NUL(new1).ret();

        _subs.insert(make_pair(new1.getName(), tstr<me>(new1)));
    }

    void me::add(std::initializer_list<leaf*> subs) {
        for(auto e: subs)
            add(*e);
    }

    void me::del(const leaf& it) { del(it.getName()); }

    void me::del(const std::string& name) { _subs.erase(name); }

    const std::string& me::getName() const { return _name; }

    void me::setName(const std::string& newName) { _name = newName; }

    ncnt me::len() const { return _subs.size(); }

    me::iterator me::begin() { return _subs.begin(); }

    me::iterator me::end() { return _subs.end(); }

    const type& me::getType() const { return ttype<me>::get(); }

    nint me::asInt() const { return 0; }

    nchar me::asChar() const { return '\0'; }

    const std::string& me::asStr() const {
        static std::string inner;
        return inner;
    }

    nbool me::asBool() const { return false; }

    nbool me::isExist() const { return true; }
} // namespace nm
