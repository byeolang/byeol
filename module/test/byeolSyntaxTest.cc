#include "test/byeolSyntaxTest.hpp"
using namespace by;

typedef byeolSyntaxTest me;

void me::TearDown() {
    byeolTest::TearDown();
    _src = "";
    _ip.rel();
}

by::node* me::getSubPack() { return _ip.getSubPack(); }

const by::node* me::getSubPack() const { return _ip.getSubPack(); }

by::slot* me::getSlot() { return _ip.getTask(); }

const by::slot* me::getSlot() const { return _ip.getTask(); }

by::errReport& me::getReport() { return _rpt; }

byeolSyntaxTest& me::make(const std::string& name) { return make(by::manifest(name)); }

byeolSyntaxTest& me::make() { return make(by::manifest()); }

byeolSyntaxTest& me::make(const by::manifest& mani) {
    _rel();
    nbool isVerbose = logger::get().isEnable();
    int flag = isVerbose ?
        interpreter::DEFAULT | interpreter::LOG_STRUCTURE | interpreter::GUARD | interpreter::LOG_GRAPH_ON_EX :
        interpreter::DUMP_ON_EX | interpreter::LOG_ON_END;
    _ip.setTask(*new by::slot(mani)).setFlag(flag);
    return *this;
}

byeolSyntaxTest& me::setFlag(nint newFlag) {
    _ip.setFlag(newFlag);
    return *this;
}

byeolSyntaxTest& me::addFlag(nint flag) {
    _ip.addFlag(flag);
    return *this;
}

byeolSyntaxTest& me::delFlag(nint flag) {
    _ip.delFlag(flag);
    return *this;
}

byeolSyntaxTest& me::parse(const by::nchar* src) {
    using namespace by;

    defaultSigZone<interpreter> zone(_ip);
    _ip.addSupply(*new by::bufSupply(std::string(src))).setReport(_rpt).work();
    return *this;
}

byeolSyntaxTest& me::negative() {
    typedef by::interpreter ip;
    _rpt.setNoisy(false);
    _ip.setFlag(0);
    return *this;
}

byeolSyntaxTest& me::silenceLog() {
    _rpt.setNoisy(false);
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

by::str me::run() {
    using by::starter;
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
