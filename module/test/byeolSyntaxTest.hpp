#pragma once

#include <iostream>

#include "test/byeolTest.hpp"

struct byeolSyntaxTest: public byeolTest {
    void TearDown() override;

    by::node* getSubPack();
    const by::node* getSubPack() const;
    by::slot* getSlot();
    const by::slot* getSlot() const;
    by::errReport& getReport();

    byeolSyntaxTest& make(const std::string& name);
    byeolSyntaxTest& make();
    byeolSyntaxTest& make(const by::manifest& mani);
    byeolSyntaxTest& setFlag(by::nint newFlag);
    byeolSyntaxTest& addFlag(by::nint Flag);
    byeolSyntaxTest& delFlag(by::nint flag);
    byeolSyntaxTest& parse(const by::nchar* src);
    byeolSyntaxTest& negative();

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
