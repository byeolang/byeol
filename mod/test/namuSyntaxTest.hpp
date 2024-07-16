#pragma once

#include "namuTest.hpp"
#include <iostream>

struct namuSyntaxTest : public namuTest {
    void TearDown() override;

    nm::origin& getSubPack();
    const nm::origin& getSubPack() const;
    nm::slot& getSlot();
    const nm::slot& getSlot() const;
    nm::errReport& getReport();

    namuSyntaxTest& make(const std::string& name);
    namuSyntaxTest& make();
    namuSyntaxTest& make(const nm::manifest& mani);
    namuSyntaxTest& parse(const nm::nchar* src);
    namuSyntaxTest& negative();

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
