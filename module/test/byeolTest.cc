#include "test/byeolTest.hpp"

typedef byeolTest me;

void me::SetUp() {
    _assertVault();

    thr.bind(new by::thread());
    by::thread::set(*thr);
}

void me::TearDown() {
    _assertVault();

    by::thread::set(nullptr);
    thr.rel();
}

by::instance::vault& me::_getVault() { return by::instance::_vault; }

void me::_assertVault() {
    auto& v = _getVault();
    NM_DI("vault[%s].len()=%d", &v, v.len());
    ASSERT_EQ(v.len(), 0);
}
