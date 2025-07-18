#include "leaf/ast/leaf.hpp"
#include "leaf/ast/nulLeaf.hpp"

namespace by {

    namespace {
        static nulLeaf inner;
    }

    BY_DEF_ME(leaf)

    me::leaf(std::initializer_list<me*> subs, const std::string& name): _name(name) { add(subs); }

    me::leaf(const me& rhs, const std::string& name): super(), _subs(rhs._subs), _name(name) {}

    me::leaf(const std::string& name): _name(name) {}

    me& me::operator[](const std::string& name) { return sub(name); }

    me& me::operator[](const nchar* name) { return sub(name); }

    me::operator nbool() const { return isExist(); }

    me& me::sub(const std::string& name) {
        me& ret = _subs[name].get() OR.ret(inner);
        return ret;
    }

    me& me::sub(const nchar* name) {
        WHEN_NUL(name).ret(inner);
        return sub(std::string(name));
    }

    me& me::sub(nidx n) {
        if(n < 0 || n >= _subs.size()) return inner;
        return *std::next(begin(), n)->second.get();
    }

    nbool me::has(const std::string& name) const { return _subs.find(name) != _subs.end(); }

    void me::add(const leaf& new1) { _subs.insert(make_pair(new1.getName(), tstr<me>(new1))); }

    void me::add(std::initializer_list<leaf*> subs) {
        for(auto e: subs)
            add(*e);
    }

    void me::del(const leaf& it) { del(it.getName()); }

    void me::del(const std::string& name) { _subs.erase(name); }

    void me::del(const nchar* name) {
        WHEN_NUL(name).ret();
        _subs.erase(name);
    }

    const std::string& me::getName() const { return _name; }

    void me::setName(const std::string& newName) { _name = newName; }

    void me::setName(const nchar* newName) {
        WHEN_NUL(newName).ret();
        _name = newName;
    }

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
} // namespace by
