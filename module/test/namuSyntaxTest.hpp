#pragma once

#include <iostream>

#include "test/namuTest.hpp"

struct namuSyntaxTest: public namuTest {
    void TearDown() override;

    nm::node* getSubPack();
    const nm::node* getSubPack() const;
    nm::slot* getSlot();
    const nm::slot* getSlot() const;
    nm::errReport& getReport();

    namuSyntaxTest& make(const std::string& name);
    namuSyntaxTest& make();
    namuSyntaxTest& make(const nm::manifest& mani);
    namuSyntaxTest& setFlag(nm::nint newFlag);
    namuSyntaxTest& addFlag(nm::nint Flag);
    namuSyntaxTest& delFlag(nm::nint flag);
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
