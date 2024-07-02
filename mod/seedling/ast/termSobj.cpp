#include "termSobj.hpp"

namespace nm {
    NM(DEF_ME(termSobj))

    me::termSobj(const std::string& rawVal, const std::string& name): _rawVal(rawVal) {}
    me::termSobj(nbool val, const std::string& name): super(name), _rawVal(val ? "true" : "false") {}
    me::termSobj(nint val, const std::string& name): super(name), _rawVal(std::to_string(val)) {}
    me::termSobj(nflt val, const std::string& name): super(name), _rawVal(std::to_string(val)) {}
    me::termSobj(const nchar* val, const std::string& name): super(name), _rawVal(val) {}
    me::termSobj(const nchar val, const std::string& name): super(name), _rawVal(1, val) {}
    me::termSobj(const me& rhs, const std::string& name): super(rhs, name), _rawVal(rhs._rawVal) {}

    const std::string& me::asStr() const {
        return _rawVal;
    }

    nchar me::asChar() const {
        return _rawVal[0];
    }

    nint me::asInt() const {
        return std::stoi(_rawVal);
    }

    nbool me::asBool() const {
        std::string low = toLower(_rawVal);

        if(low == "false") return false;
        if(low == "true") return true;
        return asInt() != 0;
    }

    const type& me::getType() const {
        return ttype<me>::get();
    }

    std::string me::toLower(std::string it) {
        transform(it.begin(), it.end(), it.begin(), ::tolower);
        return it;
    }
}
