#include "namuTest.hpp"

typedef namuTest me;

void me::SetUp() {
    _assertVault();

    thr.bind(new nm::thread());
    nm::thread::set(*thr);
}

void me::TearDown() {
    _assertVault();

    nm::thread::set();
    thr.rel();
}

nm::instance::vault& me::_getVault() {
    return nm::instance::_vault;
}

void me::_assertVault() {
    auto& v = _getVault();
    NAMU_DI("vault[%x].len()=%d", &v, v.len());
    ASSERT_EQ(v.len(), 0);
}
