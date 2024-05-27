#pragma once

#include "namuTest.hpp"
#include <iostream>

struct namuSyntaxTest : public namuTest {
    void TearDown();

    namu::node& getSubPack();
    const namu::node& getSubPack() const;
    namu::slot& getSlot();
    const namu::slot& getSlot() const;
    namu::errReport& getReport();

    namuSyntaxTest& make(const std::string& name);
    namuSyntaxTest& make();
    namuSyntaxTest& make(const namu::manifest& mani);
    namuSyntaxTest& parse(const namu::nchar* src);
    namuSyntaxTest& negative();

    namu::nbool shouldParsed(namu::nbool well);
    namu::nbool shouldVerified(namu::nbool well);

    namu::str run();

private:
    void _rel();

private:
    const namu::nchar* _src;
    namu::errReport _rpt;
    namu::interpreter _ip;
    namu::sigHandler _handler;
};
