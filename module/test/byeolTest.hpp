#pragma once

#include <iostream>

#include "test/common.hpp"

struct byeolTest: public ::testing::Test {
    void SetUp();

    void TearDown();

    void setNoisyEx(by::nbool noisy);

protected:
    by::instance::vault& _getVault();

private:
    void _assertVault();

private:
    by::tstr<by::thread> thr;
};
