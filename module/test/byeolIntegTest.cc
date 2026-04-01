#include "test/byeolIntegTest.hpp"
using namespace by;

typedef byeolIntegTest me;

void me::TearDown() {
    byeolTest::TearDown();
    _src = "";
    _ip.rel();
}

by::node* me::getSubPack() { return _ip.getSubPack(); }

const by::node* me::getSubPack() const { return _ip.getSubPack(); }

by::pack* me::getPack() { return _ip.getTask(); }

const by::pack* me::getPack() const { return _ip.getTask(); }

byeolIntegTest& me::make(const std::string& name) { return make(by::manifest(name)); }

byeolIntegTest& me::make() { return make(by::manifest()); }

byeolIntegTest& me::make(const by::manifest& mani) {
    _rel();
    nbool isVerbose = logger::get().isEnable();
    int flag = isVerbose ?
        interpreter::DEFAULT | interpreter::LOG_STRUCTURE | interpreter::GUARD | interpreter::LOG_GRAPH_ON_EX :
        interpreter::DUMP_ON_EX | interpreter::LOG_ON_END;
    _ip.setTask(*new by::pack(mani)).setFlag(flag);
    return *this;
}

byeolIntegTest& me::setFlag(nint newFlag) {
    _ip.setFlag(newFlag);
    return *this;
}

byeolIntegTest& me::addFlag(nint flag) {
    _ip.addFlag(flag);
    return *this;
}

byeolIntegTest& me::delFlag(nint flag) {
    _ip.delFlag(flag);
    return *this;
}

byeolIntegTest& me::parse(const by::nchar* src) {
    using namespace by;

    defaultSigZone<interpreter> zone(_ip);
    _ip.getParser().addSupply(*new bufSupply(std::string(src)));
    _ip.setReport(getReport()).work();
    return *this;
}

byeolIntegTest& me::negative() {
    super::negative();

    WHEN(logger::get().isEnable()) .ret(*this); // in verbose mode, do not silence log.
    typedef by::interpreter ip;
    getReport().setNoisy(false);
    _ip.setFlag(0);
    return *this;
}

by::nbool me::shouldParsed(by::nbool expect) {
    by::nbool ret = _ip.isParsed();
    EXPECT_EQ(ret, expect);
    return ret;
}

by::nbool me::shouldVerified(by::nbool expect) {
    by::nbool ret = _ip.isVerified();
    EXPECT_EQ(ret, expect);
    return ret;
}

by::str me::run(by::nbool silent) {
    using by::starter;
    auto flag = silent ? 0 : starter::LOG_STRUCTURE | starter::LOG_GRAPH_ON_EX | starter::DEFAULT;
    return starter().setTask(getSubPack()).setReport(getReport()).setFlag(flag).work();
}

void me::_rel() {
    _src = "";
    _ip.rel();
    getReport().rel();
}
