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

void me::setNoisyEx(by::nbool noisy) {
    getReport().setNoisy(noisy);
}

me& me::negative() {
    setNoisyEx(false);
    return *this;
}

by::errReport& me::getReport() { return by::thread::get().getEx(); }

by::instance::vault& me::_getVault() { return by::instance::getVault(); }

void me::_assertVault() {
    auto& v = _getVault();
    BY_DI("vault[%s].len()=%d", &v, v.len());
    ASSERT_EQ(v.len(), 0);
}
