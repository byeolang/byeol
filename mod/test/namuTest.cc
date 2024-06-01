#include "namuTest.hpp"

typedef namuTest me;

void me::SetUp() {
    _assertVault();

    thr.bind(new namu::thread());
    namu::thread::set(*thr);
}

void me::TearDown() {
    _assertVault();

    namu::thread::set();
    thr.rel();
}

namu::instance::vault& me::_getVault() {
    return namu::instance::_vault;
}

void me::_assertVault() {
    auto& v = _getVault();
    NAMU_DI("vault[%x].len()=%d", &v, v.len());
    ASSERT_EQ(v.len(), 0);
}
