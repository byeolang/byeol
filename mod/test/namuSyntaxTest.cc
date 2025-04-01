#include "namuSyntaxTest.hpp"
using namespace nm;

typedef namuSyntaxTest me;

void me::TearDown() {
    namuTest::TearDown();
    _src = "";
    _ip.rel();
}

nm::node& me::getSubPack() { return _ip.getSubPack(); }

const nm::node& me::getSubPack() const { return _ip.getSubPack(); }

nm::slot& me::getSlot() { return _ip.getTask(); }

const nm::slot& me::getSlot() const { return _ip.getTask(); }

nm::errReport& me::getReport() { return _rpt; }

namuSyntaxTest& me::make(const std::string& name) { return make(nm::manifest(name)); }

namuSyntaxTest& me::make() { return make(nm::manifest()); }

namuSyntaxTest& me::make(const nm::manifest& mani) {
    _rel();
    nbool isVerbose = logger::get().isEnable();
    int flag = isVerbose ? interpreter::DEFAULT | interpreter::LOG_STRUCTURE | interpreter::GUARD |
            interpreter::LOG_GRAPH_ON_EX :
                           interpreter::DUMP_ON_EX | interpreter::LOG_ON_END;
    _ip.setTask(*new nm::slot(mani)).setFlag(flag);
    return *this;
}

namuSyntaxTest& me::setFlag(nint newFlag) {
    _ip.setFlag(newFlag);
    return *this;
}

namuSyntaxTest& me::addFlag(nint flag) {
    _ip.addFlag(flag);
    return *this;
}

namuSyntaxTest& me::delFlag(nint flag) {
    _ip.delFlag(flag);
    return *this;
}

namuSyntaxTest& me::parse(const nm::nchar* src) {
    using namespace nm;

    defaultSigZone<interpreter> zone(_ip);
    _ip.addSupply(*new nm::bufSupply(std::string(src))).setReport(_rpt).work();
    return *this;
}

namuSyntaxTest& me::negative() {
    typedef nm::interpreter ip;
    _rpt.setNoisy(false);
    _ip.setFlag(0);
    return *this;
}

nm::nbool me::shouldParsed(nm::nbool expect) {
    nm::nbool ret = _ip.isParsed();
    EXPECT_EQ(ret, expect);
    return ret;
}

nm::nbool me::shouldVerified(nm::nbool expect) {
    nm::nbool ret = _ip.isVerified();
    EXPECT_EQ(ret, expect);
    return ret;
}

nm::str me::run() {
    using nm::starter;
    return starter()
        .setTask(getSubPack())
        .setReport(_rpt)
        .setFlag(starter::LOG_STRUCTURE | starter::LOG_GRAPH_ON_EX | starter::DEFAULT)
        .work();
}

void me::_rel() {
    _src = "";
    _rpt.rel();
    _ip.rel();
}
