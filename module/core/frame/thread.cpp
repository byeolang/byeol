#include "core/frame/thread.hpp"

#include "core/ast/baseFunc.hpp"
#include "core/ast/dumScope.hpp"
#include "core/ast/node.inl"
#include "core/builtin/pkgs/default/inputFunc.hpp"
#include "core/builtin/pkgs/default/printFunc.hpp"
#include "core/builtin/err/errReport.hpp"
#include "core/builtin/err/err.hpp"
#include "core/loader/slot/slotLoader.hpp"

namespace by {

    BY_DEF_ME(thread)

    namespace {
        class dumFrame: public frame {
            BY(CLASS(dumFrame, frame))

        public:
            dumFrame() { super::add(dumScope::singleton()); }

        public:
            using super::add;

            void add(const node* owner, const scope& s) override {}

            using super::addLocal;

            void addLocal(const std::string& name, const node& n) override {}

            void del() override {}

            using super::setMe;

            nbool setMe(const node& obj) override { return true; }

            using super::addFunc;

            nbool addFunc(const baseFunc& new1) override { return true; }

            void rel() override {}

            using super::setRet;

            nbool setRet(const node& newRet) const override { return true; }
        };

        class dumFrames: public frames {
            BY(CLASS(dumFrames, frames))

        public:
            dumFrames() { super::add(*new dumFrame()); }

        public:
            using super::add;

            nbool add(const iter& e, const frame& new1) override { return true; }

            nbool add(nidx n, const frame& new1) override { return true; }

            void add(const iter& here, const iter& from, const iter& to) override {}

            using super::set;
            using tarrayable<frame>::set;

            nbool set(const iter& at, const frame& new1) override { return true; }

            nbool set(nidx n, const frame& new1) override { return true; }

            using super::del;
            using tarrayable<frame>::del;

            nbool del(const iter& from, const iter& end) override { return true; }

            nbool del(const iter& it) override { return true; }

            nbool del(nidx n) override { return true; }

            void rel() override {}
        };

        class dumThread: public thread {
            BY(CLASS(dumThread, thread))

        public:
            dumThread() { super::setEx(dummyErrReport::singleton); }

        public:
            using super::setEx;

            void setEx(const errReport& new1) override {}

            void rel() override {}

            nbool isInteractable() const override { return false; }

        protected:
            frames& _getFrames() override {
                static dumFrames inner;
                return inner;
            }
        };

        static dumThread& _getDumThread() {
            static dumThread inner;
            return inner;
        }

        static thread_local thread* _instance = nullptr;
    } // namespace

    const nmap& me::getSlots() const {
        static tstr<nmap> _inner;
        if(!_inner) _inner = _initSlots();
        return *_inner;
    }

    str me::run(const args& a) { return str(); }

    me::thread(): _ex(new errReport()) {} // for singleton

    me::thread(const errReport& ex): _ex(ex) {}

    thread& me::get() {
        if(!_instance) _instance = &_getDumThread();
        return *_instance;
    }

    void me::set(thread* new1) {
        _instance = new1 ? new1 : &_getDumThread();
        BY_DI("thread::set(%s -> %s)", get(), _instance);
    }

    void me::set(thread& new1) { set(&new1); }

    const instancer* me::getInstancer() { return instancer::get(); }

    errReport& me::getEx() { return *_ex; }

    void me::setEx(const errReport& new1) { _ex.bind(new1); }

    // node:
    scope& me::subs() { return dumScope::singleton(); }

    priorType me::prioritize(const args& a) const { return NO_MATCH; }

    void me::rel() { _frames.rel(); }

    frames& me::_getFrames() { return _frames; }

    frame* me::_getNowFrame() {
        ncnt n = _getFrames().len() - 1;
        WHEN(n < 0 || n >= _getFrames().len()) .ret(nullptr);

        return &_getFrames()[n];
    }

    void me::_loadBuiltIns(nmap& tray) const {
        tray.add("input", new inputFunc());
        tray.add("print", new printFunc<nStr>());
        tray.add("err", new err(""));
    }

    tstr<nmap> me::_initSlots() const {
        tstr<nmap> ret;
        BY_I("initiates loading system slots.");
        ret.bind(new nmap());
        errReport report;
        slotLoader()
            .setReport(report)
            .setBaseSlots(*ret)
#ifdef BY_BUILD_PLATFORM_IS_LINUX
            .addPath("/usr/share/pack/")
#endif
            .addPath("pack/")
            .load();

        _loadBuiltIns(*ret);

        BY_I("%d system slots has been loaded.", ret->len());

#if BY_IS_DBG
        BY_I("next following is list for them.");
        for(const auto& s: *ret) {
            const manifest& mani = s TO(template cast<slot>()) TO(getManifest()) OR_CONTINUE;
            BY_DI(" - %s v%s", mani.name, mani.version);
        }
#endif
        return ret;
    }

    void me::dump() const {
        getEx().dump();
    }

    nbool me::isInteractable() const { return true; }
} // namespace by
