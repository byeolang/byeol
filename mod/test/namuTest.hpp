#pragma once

#include "common/dep.hpp"
#include <iostream>

struct namuTest : public ::testing::Test {
    void SetUp();

    void TearDown();

protected:
    nm::instance::vault& _getVault();

private:
    void _assertVault();

private:
    nm::tstr<nm::thread> thr;
};
