#pragma once

#include <iostream>

#include "test/byeolTest.hpp"

struct byeolSyntaxTest: public byeolTest {
    void TearDown() override;

    nm::node* getSubPack();
    const nm::node* getSubPack() const;
    nm::slot* getSlot();
    const nm::slot* getSlot() const;
    nm::errReport& getReport();

    byeolSyntaxTest& make(const std::string& name);
    byeolSyntaxTest& make();
    byeolSyntaxTest& make(const nm::manifest& mani);
    byeolSyntaxTest& setFlag(nm::nint newFlag);
    byeolSyntaxTest& addFlag(nm::nint Flag);
    byeolSyntaxTest& delFlag(nm::nint flag);
    byeolSyntaxTest& parse(const nm::nchar* src);
    byeolSyntaxTest& negative();

    nm::nbool shouldParsed(nm::nbool well);
    nm::nbool shouldVerified(nm::nbool well);

    nm::str run();

private:
    void _rel();

private:
    const nm::nchar* _src;
    nm::errReport _rpt;
    nm::interpreter _ip;
};
