#include "stela/parser/stelaSmartDedent.hpp"
#include "stela/parser/bison/stelaLowparser.hpp"

namespace by {

    BY_DEF_ME(stelaSmartDedent)

    me::stelaSmartDedent(): _cnt(OFF), _isEnable(false) {}

    me& me::countDown() {
        _cnt--;
        return *this;
    }

    me& me::countUp() {
        _cnt++;
        return *this;
    }

    nbool me::canDedent() const { return _cnt == CAUGHT && _isEnable; }

    me& me::useLater() {
        if(_cnt > OFF) _isEnable = true;
        return *this;
    }

    nint me::dedent() {
        _isEnable = false;
        return NEWLINE;
    }

    void me::rel() {
        _isEnable = false;
        _cnt = OFF;
    }

    nbool me::isOn() const { return _cnt > OFF; }
} // namespace by
