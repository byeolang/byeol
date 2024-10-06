#include "starter.hpp"

#include <csignal>

#include "../ast/baseFunc.hpp"
#include "../ast/dumScope.hpp"
#include "../ast/node.inl"
#include "../ast/obj.hpp"
#include "../ast/slot.hpp"
#include "../worker/visitor/graphVisitor.hpp"
#include "threadUse.hpp"

namespace nm {

    NM_DEF_ME(starter)

    void me::_prepare() {
        super::_prepare();
        if(getReport().isSub<dummyErrReport>()) setReport(*new errReport());
    }

    void me::setArgs(const args& a) { _args.bind(a); }

    args& me::getArgs() {
        static args inner;
        return _args ? *_args : inner;
    }

    str me::_onWork() {
        args& a = getArgs();

        // TODO: don't use static variable '_cache':
        //  instead, put cache onto origin object, and if arr instance is origin, remove the cache.
        arr::_cache.clear();
        node& pak = getOr(getTask()) orRet NM_E("there is no pack!"), str();

        NM_I("run a pack");
        node& main = getOr(_findMain(pak, args())) orRet NM_E("there is 0 or more than 2 main() found."), str();

        if(main.canRun(a)) {
            threadUse thr(getReport());
            _prepareFrame(thread::get()._getFrames());
            NM_DI("-------------- START ----------------");
            str res = _postprocess(pak.run(MAIN));
            NM_DI("-------------------------------------");
            return res;
        }

        // TODO: main(str[])

        return NM_E("couldn't run main func(). it doesn't match any argument"), str();
    }

    str me::_postprocess(str res) {
        errReport& ex = thread::get().getEx();
        if(ex) {
            NM_E("unhandled exception found:");
            ex.dump();
        }
        return res;
    }

    void me::_prepareFrame(frames& fr) { fr.rel(); }

    node& me::_findMain(node& pak, const args& a) {
        // TODO: now, only find to main() but I need to find main(argc, argv) case, too.
        node& ret = getOr(pak.sub(MAIN)) orDo NM_E("couldn't find main().");

        return ret;
    }
} // namespace nm
