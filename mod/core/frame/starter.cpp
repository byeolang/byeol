#include "../ast/obj.hpp"
#include "../ast/baseFunc.hpp"
#include "../ast/dumScope.hpp"
#include "../ast/slot.hpp"
#include "../ast/node.inl"
#include "starter.hpp"
#include "threadUse.hpp"
#include <csignal>
#include "../visitor/graphVisitor.hpp"

namespace namu {

    NAMU_DEF_ME(starter)

    me& me::setPack(node& pak) {
        _pak.bind(pak);
        return *this;
    }

    node& me::getPack() { return *_pak; }

    void me::_prepare() {
        super::_prepare();
        if(nul(getTask()))
            setTask(*new args());
        if(getReport().isSub<dummyErrReport>())
            setReport(*new errReport());
    }

    str me::_onWork() {
        args& a = getTask();

        // TODO: don't use static variable '_cache':
        //  instead, put cache onto origin object, and if arr instance is origin, remove the cache.
        arr::_cache.clear();
        node& pak = getPack();
        if(nul(pak)) return NAMU_E("there is no pack!"), str();

        NAMU_I("run a pack");
        node& main = _findMain(pak, args());
        if(nul(main))
            return NAMU_E("there is 0 or more than 2 main() found."), str();

        if(main.canRun(a)) {
            threadUse thr(getReport());
            _prepareFrame(thread::get()._getFrames());
            NAMU_DI("-------------- START ----------------");
            str res = _postprocess(pak.run(MAIN));
            NAMU_DI("-------------------------------------");
            return res;
        }

        // TODO: main(str[])

        return NAMU_E("couldn't run main func(). it doesn't match any argument"), str();
    }

    str me::_postprocess(str res) {
        errReport& ex = thread::get().getEx();
        if(ex) {
            NAMU_E("unhandled exception found:");
            ex.dump();
        }
        return res;
    }

    void me::_prepareFrame(frames& fr) {
        fr.rel();
    }

    node& me::_findMain(node& pak, const args& a) {
        // TODO: now, only find to main() but I need to find main(argc, argv) case, too.
        node& ret = pak.sub(MAIN);
        if(nul(ret))
            NAMU_E("couldn't find main().");

        return ret;
    }
}
