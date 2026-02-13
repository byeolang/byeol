#pragma once

#include <iostream>

#include "test/byeolTest.hpp"

struct byeolIntegTest: public byeolTest {
    void TearDown() override;

    by::node* getSubPack();
    const by::node* getSubPack() const;
    by::slot* getSlot();
    const by::slot* getSlot() const;
    by::errReport& getReport();

    byeolIntegTest& make(const std::string& name);
    byeolIntegTest& make();
    byeolIntegTest& make(const by::manifest& mani);
    byeolIntegTest& setFlag(by::nint newFlag);
    byeolIntegTest& addFlag(by::nint Flag);
    byeolIntegTest& delFlag(by::nint flag);
    byeolIntegTest& parse(const by::nchar* src);
    byeolIntegTest& negative();
    byeolIntegTest& silenceLog();

    by::nbool shouldParsed(by::nbool well);
    by::nbool shouldVerified(by::nbool well);

    by::str run();

private:
    void _rel();

private:
    const by::nchar* _src;
    by::errReport _rpt;
    by::interpreter _ip;
};
