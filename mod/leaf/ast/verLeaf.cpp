#include "leaf/ast/verLeaf.hpp"

namespace nm {

    NM(DEF_ME(verLeaf))

    me::verLeaf(int major, int minor, int fix):
        super(std::to_string(major) + DELIMITER + std::to_string(minor) + DELIMITER +
            std::to_string(fix)),
        _maj(major),
        _min(minor),
        _fix(fix) {}

    me::verLeaf(const std::string& verStr): super(verStr), _maj(0), _min(0), _fix(0) {
        _parseVerStr(verStr);
    }

    me::verLeaf(const nchar* verStr): super(verStr), _maj(0), _min(0), _fix(0) {
        _parseVerStr(std::string(verStr));
    }

    nbool me::operator>(const me& rhs) const {
        nint res = _isFromBigger(_maj, rhs._maj);
        WHEN(res != 0).ret(res == 1);

        res = _isFromBigger(_min, rhs._min);
        WHEN(res != 0).ret(res == 1);

        return _isFromBigger(_fix, rhs._fix) > 0;
    }

    nbool me::operator<(const me& rhs) const {
        nint res = _isFromBigger(rhs._maj, _maj);
        WHEN(res != 0).ret(res == 1);

        res = _isFromBigger(rhs._min, _min);
        WHEN(res != 0).ret(res == 1);

        return _isFromBigger(rhs._fix, _fix) > 0;
    }

    nbool me::operator==(const me& rhs) const {
        WHEN(this == &rhs).ret(true);

        return _maj == rhs._maj && _min == rhs._min && _fix == rhs._fix;
    }

    nbool me::operator!=(const me& rhs) const { return !operator==(rhs); }

    nbool me::operator<=(const me& rhs) const { return !operator>(rhs); }

    nbool me::operator>=(const me& rhs) const { return !operator<(rhs); }

    nint me::asMajor() const { return _maj; }

    nint me::asMinor() const { return _min; }

    nint me::asFix() const { return _fix; }

    const type& me::getType() const { return ttype<me>::get(); }

    nint me::_isFromBigger(nint from, nint to) {
        if(from > to) return 1;
        if(from == to) return 0;
        return -1;
    }

    void me::_parseVerStr(const std::string& verStr) {
        std::stringstream ss(verStr);
        std::string token;

        nint* them[] = {&_maj, &_min, &_fix};
        for(int n = 0; n < VER_LEN; n++) {
            WHEN(!std::getline(ss, token, DELIMITER[0])).err("error parsing to %s", verStr).ret();

            *them[n] = std::stoi(token);
        }
    }
} // namespace nm
