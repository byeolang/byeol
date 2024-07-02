#include "thread.hpp"
#include "../loader/slot/slotLoader.hpp"
#include "../loader/errReport.hpp"
#include "../ast/node.inl"
#include "../ast/baseFunc.hpp"
#include "../builtin/pkgs/default/defaultPack.hpp"
#include "../ast/dumScope.hpp"

namespace nm {

    NM_DEF_ME(thread)

    namespace {
        class dumFrame : public frame {
            NM(CLASS(dumFrame, frame))

        public:
            dumFrame() {
                super::add(*new dumScope());
            }

        public:
            using super::add;
            void add(node& owner, scope& s) override {}
            void addLocal(const std::string& name, const node& n) override {}

            void del() override {}

            using super::setMe;
            nbool setMe(const baseObj& obj) override { return true; }

            using super::setFunc;
            nbool setFunc(baseFunc& new1) override { return true; }

            void rel() override {}

            using super::setRet;
            virtual nbool setRet(const node& newRet) const override { return true; }
        };

        class dumFrames : public frames {
            NM(CLASS(dumFrames, frames))

        public:
            dumFrames() {
                super::add(*new dumFrame());
            }

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
            nbool del(nidx n) override { return true;}

            void rel() override {}
        };

        class dumThread : public thread {
            NM(CLASS(dumThread, thread))

        public:
            dumThread() {
                super::setEx(dummyErrReport::singletone);
            }

        public:
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
    }

    const nmap& me::getSlots() const {
        static tstr<nmap> _inner;
        if(!_inner)
            _inner = _initSlots();
        return *_inner;
    }

    str me::run(const args& a) { return str(); }

    me::thread(): _ex(new errReport()) {} // for singleton
    me::thread(const errReport& ex): _ex(ex) {}

    thread& me::get() {
        if (nul(_instance))
            _instance = &_getDumThread();
        return *_instance;
    }

    void me::set(thread* new1) {
        thread& prev = get();
        _instance = new1 ? new1 : &_getDumThread();
        NM_DI("thread::set(%s -> %s)", prev.getType().getName().c_str(), _instance->getType().getName().c_str());
    }
    void me::set(thread& new1) { set(&new1); }
    void me::set() { set(nullptr); }

    const instancer& me::getInstancer() {
        return instancer::get();
    }

    errReport& me::getEx() { return *_ex; }
    void me::setEx(const errReport& new1) { _ex.bind(new1); }

    // node:
    scope& me::subs() {
        static dumScope inner;
        return inner;
    }

    priorType me::prioritize(const args& a) const {
        return NO_MATCH;
    }

    void me::rel() {
        _frames.rel();
    }

    frames& me::_getFrames() {
        return _frames;
    }

    frame& me::_getNowFrame() {
        return _getFrames()[_getFrames().len() - 1];
    }

    void me::_loadBuiltIns(nmap& tray) const {
        tray.add(defaultPack().subs());
    }

    tstr<nmap> me::_initSlots() const {
        tstr<nmap> ret;
        NM_I("initiates loading system slots.");
        ret.bind(new nmap());
        errReport report;
        slotLoader().setReport(report).setBaseSlots(*ret)
#ifdef NM_BUILD_PLATFORM_IS_LINUX
            .addPath("/usr/share/namu/pack/")
#endif
            .addPath("pack/")
            .load();

        _loadBuiltIns(*ret);

        NM_I("%d system slots has been loaded.", ret->len());

#if NM_IS_DBG
        NM_I("next following is list for them.");
        for(const auto& s : *ret) {
            if(nul(s)) {
                NM_E("cast isn't type of slot&");
                continue;
            }

            const slot& sl = s.cast<slot>();
            if(nul(sl)) continue;

            const manifest& mani = sl.getManifest();
            NM_DI(" - %s v%s", mani.name.c_str(), mani.version.c_str());
        }
#endif
        return ret;
    }

    void me::dump() const {
        // TODO:
    }

    nbool me::isInteractable() const { return true; }
}
