#include "cpIter.hpp"

namespace nm {

    NM(DEF_ME(cpIter))

    me::cpIter(const nchar* from): _e(from) {}

    me::cpIter(const std::string& from): _e(from.c_str()) {}

    me me::operator+(ncnt step) {
        next(step);
        return *this;
    }

    me& me::operator++() {
        next(1);
        return *this;
    }

    me me::operator++(int) {
        me ret = *this;
        next(1);
        return ret;
    }

    me& me::operator+=(ncnt step) {
        next(1);
        return *this;
    }

    std::string me::operator*() const { return get(); }

    me::operator nbool() const { return !isEnd(); }

    nbool me::operator==(const me& rhs) const { return _e == rhs._e; }

    nbool me::isEnd() const { return !_e || !*_e; }

    ncnt me::next(ncnt step) {
        nint n = 0;
        while(n++ < step) {
            _e = _nextCodepoint(_e);
            if(isEnd()) break;
        }
        return n;
    }

    std::string me::get() const { return isEnd() ? "" : std::string(_e, _nextCodepoint(_e) - _e); }

    ncnt me::remainLen() const {
        me copy(*this);
        ncnt len = 0;
        while(copy++)
            len++;
        return len;
    }

    const nchar* me::_nextCodepoint(const nchar* from) const { return from + _skipBytes(*from); }

    ncnt me::_skipBytes(nchar ch) const {
        if(ch == 0) return 0;
        if((ch & 0x80) == 0x00) return 1; // U+0000 to U+007F
        if((ch & 0xE0) == 0xC0) return 2; // U+0080 to U+07FF
        if((ch & 0xF0) == 0xE0) return 3; // U+0800 to U+FFFF
        if((ch & 0xF8) == 0xF0) return 4; // U+10000 to U+10FFFF
        return 0;
    }

} // namespace nm
