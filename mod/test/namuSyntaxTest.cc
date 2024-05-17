#include "namuSyntaxTest.hpp"

typedef namuSyntaxTest me;

void me::TearDown() {
    namuTest::TearDown();
    _rel();
}

namu::node& me::getSubPack() {
    return _ip.getSubPack();
}

const namu::node& me::getSubPack() const {
    return _ip.getSubPack();
}

namu::slot& me::getSlot() {
    return _ip.getTask();
}

const namu::slot& me::getSlot() const {
    return _ip.getTask();
}

namu::errReport& me::getReport() {
    return _rpt;
}

namuSyntaxTest& me::make(const std::string& name) {
    return make(namu::manifest(name));
}

namuSyntaxTest& me::make() {
    return make(namu::manifest());
}

namuSyntaxTest& me::make(const namu::manifest& mani) {
    _rel();
    _ip.setTask(*new namu::slot(mani));
    return *this;
}

namuSyntaxTest& me::parse(const namu::nchar* src) {
    using namu::interpreter;
    namu::nbool isVerbose = namu::logger::get().isEnable();
    int flag = isVerbose ? interpreter::DEFAULT | interpreter::LOG_STRUCTURE | interpreter::GUARD:
        interpreter::DUMP_ON_EX | interpreter::LOG_ON_END;

    _ip.addSupply(*new namu::bufSupply(std::string(src)))
       .setFlag(flag)
       .setReport(_rpt)
       .work();
    return *this;
}

namuSyntaxTest& me::negative() {
    typedef namu::interpreter ip;
    _ip.delFlag(ip::LOG_STRUCTURE | ip::LOG_ON_EX | ip::DUMP_ON_EX | ip::LOG_ON_END | ip::DUMP_ON_END);
    return *this;
}

namu::nbool me::shouldParsed(namu::nbool expect) {
    namu::nbool ret = _ip.isParsed();
    EXPECT_EQ(ret, expect);
    return ret;
}

namu::nbool me::shouldVerified(namu::nbool expect) {
    namu::nbool ret = _ip.isVerified();
    EXPECT_EQ(ret, expect);
    return ret;
}

namu::str me::run() {
    return namu::starter().setPack(getSubPack()).work();
}

void me::_rel() {
    _src = "";
    _rpt.rel();
    _ip.rel();
}
