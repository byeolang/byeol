#include "core/frame/starter.hpp"

#include <csignal>

#include "core/ast/baseFunc.hpp"
#include "core/ast/dumScope.hpp"
#include "core/ast/node.inl"
#include "core/ast/obj.hpp"
#include "core/ast/slot.hpp"
#include "core/worker/visitor/graphVisitor.hpp"
#include "core/frame/threadUse.hpp"

namespace by {

    BY_DEF_ME(starter)

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
        node& pak = getTask() OR.err("there is no pack!").ret(str());

        BY_I("run a pack");
        node& main = _findMain(pak, args()) OR.err("there is 0 or more than 2 main() found.").ret(str());

        if(main.canRun(a)) {
            threadUse thr(getReport());
            _prepareFrame(thread::get()._getFrames());
            BY_DI("-------------- SYMBOL_START ----------------");
            str res = _postprocess(pak.run(MAIN));
            BY_DI("-------------------------------------");
            return res;
        }

        // TODO: main(str[])

        return BY_E("couldn't run main func(). it doesn't match any argument"), str();
    }

    str me::_postprocess(const str& res) {
        thread& th = thread::get();
        if(th.getEx()) {
            enablesZone zone(true);
            BY_DE("unhandled exception found:");
            th.dump();
        }
        return res;
    }

    void me::_prepareFrame(frames& fr) { fr.rel(); }

    node* me::_findMain(node& pak, const args& a) {
        // TODO: now, only find to main() but I need to find main(argc, argv) case, too.
        node& ret = pak.sub(MAIN) OR.err("couldn't find main().").ret(nullptr);
        return &ret;
    }
} // namespace by
