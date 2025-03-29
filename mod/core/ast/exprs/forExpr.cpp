#include "forExpr.hpp"

#include <utility>

#include "../../builtin.hpp"
#include "../../frame/frameInteract.hpp"
#include "../../frame/thread.hpp"
#include "../../worker/visitor/visitor.hpp"

namespace nm {

    NM(DEF_ME(forExpr), DEF_VISIT())

    me::forExpr(const std::string& localName, const node& container, const blockExpr& blk):
        super(blk), _container(container), _name(localName) {}

    const std::string& me::getLocalName() const { return _name; }

    str me::getContainer() { return *_container; }

    str me::_makeEval() const {
        str ased = _container->getEval();
        str elemType = ased->run("getElemType") orRet NM_E("elemType == null"), str();
        auto& fr = thread::get()._getNowFrame() orRet str();

        frameInteract f1(getBlock());
        fr.addLocal(getLocalName(), *((node*) elemType->clone()));
        return super::_makeEval();
    }

    namespace {
        class forLoop: public me::loop {
            NM(CLASS(forLoop, loop))

        public:
            forLoop(arr& ret, str container, str iter, const forExpr& owner):
                super(ret),
                _container(std::move(container)),
                _iter(std::move(iter)),
                _owner(owner) {}

        public:
            nbool isLooping() override { return !_iter->run("isEnd")->cast<nbool>(); }

            void run(blockExpr& blk, frame& fr) override {
                str elem = _iter->run("get") orRet NM_E("elem is null"), void();
                fr.addLocal(_owner.getLocalName(), *elem);

                super::run(blk, fr);
            }

            nbool postprocess(frame& fr) override {
                _iter->run("next", args{narr{*new nInt(1)}});
                return super::postprocess(fr);
            }

        private:
            str _container;
            str _iter;
            const forExpr& _owner;
        };
    }

    tstr<me::loop> me::_makeLoop(arr& ret) const {
        str ased = _container->as<node>() orRet NM_E("ased is null"), str();
        str iter = ased->run("iterate", args{narr{*new nInt(0)}}) orRet NM_E("iter is null"), str();

        NM_DI("forExpr: loop %s in %s", getLocalName(), ased->getSrc());
        return new forLoop(ret, ased, iter, *this);
    }
} // namespace nm
