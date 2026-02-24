#pragma once

#include "test/byeolTest.hpp"

struct byeolE2ETest : public byeolTest {
    byeolE2ETest();

public:
    byeolE2ETest& parse(by::nint argc, ...);
    byeolE2ETest& negative();
    byeolE2ETest& silenceLog();
    by::programRes run();
    static by::flagArgs parseFlag(by::nint argc, ...);

private:
    static by::flagArgs _parseFlag(by::nint argc, va_list va);

public:
    by::cli cli;
    by::errReport rpt;
};
