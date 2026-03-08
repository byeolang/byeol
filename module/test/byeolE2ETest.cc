#include "byeolE2ETest.hpp"

BY(DEF_ME(byeolE2ETest))

using namespace by;

me::byeolE2ETest() { cli.setReport(rpt); }

flagArgs me::parseFlag(nint argc, ...) {
    va_list va;
    va_start(va, argc);
    flagArgs ret = _parseFlag(argc, va);
    va_end(va);
    return ret;
}

flagArgs me::_parseFlag(nint argc, va_list va) {
    flagArgs ret;
    for(nint n = 0; n < argc; n++)
        ret.add(new nStr(va_arg(va, const nchar*)));
    return ret;
}

me& me::parse(by::nint argc, ...) {
    va_list va;
    va_start(va, argc);
    cli.setTask(new flagArgs(_parseFlag(argc, va)));
    va_end(va);
    return *this;
}

me& me::negative() {
    WHEN(logger::get().isEnable()) .ret(*this); // in verbose mode, do not silence log.
    typedef by::interpreter ip;
    rpt.setNoisy(false);
    return *this;
}

by::programRes me::run() { return cli.work(); }
