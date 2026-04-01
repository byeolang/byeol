#pragma once

#include <iostream>

#include "test/byeolTest.hpp"

struct byeolIntegTest: public byeolTest {
    BY(ME(byeolIntegTest, byeolTest))

public:
    void TearDown() override;

    by::node* getSubPack();
    const by::node* getSubPack() const;
    by::pack* getPack();
    const by::pack* getPack() const;

    byeolIntegTest& make(const std::string& name);
    byeolIntegTest& make();
    byeolIntegTest& make(const by::manifest& mani);
    byeolIntegTest& setFlag(by::nint newFlag);
    byeolIntegTest& addFlag(by::nint Flag);
    byeolIntegTest& delFlag(by::nint flag);
    byeolIntegTest& parse(const by::nchar* src);
    byeolIntegTest& negative() override;

    by::nbool shouldParsed(by::nbool well);
    by::nbool shouldVerified(by::nbool well);

    by::str run(by::nbool silent = false);

private:
    void _rel();

private:
    const by::nchar* _src;
    by::interpreter _ip;
};
