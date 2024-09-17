#include "smartDedent.hpp"

namespace nm {

    NM_DEF_ME(smartDedent)

    me::smartDedent(): _cnt(OFF), _isEnable(false) {}

    void me::countDown() {
        if(_cnt > OFF) _cnt--;
    }

    void me::countUp() {
        if(_cnt > OFF) _cnt++;
    }

    nbool me::canDedent() const { return _cnt == CAUGHT && _isEnable; }

    void me::setEnable() { if(_cnt > OFF) _isEnable = true; }

    void me::rel() {
        _isEnable = false;
        _cnt = OFF;
    }

    nbool me::isOn() const { return _cnt > OFF; }
}
