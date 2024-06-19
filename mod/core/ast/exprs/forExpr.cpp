#include "forExpr.hpp"
#include "../../builtin.hpp"
#include "../../visitor/visitor.hpp"
#include "../../frame/thread.hpp"
#include "../../frame/frameInteract.hpp"

namespace namu {

    NAMU(DEF_ME(forExpr), DEF_VISIT())

    me::forExpr(const std::string& localName, const node& container, const blockExpr& blk):
        super(blk), _container(container), _name(localName), _initEval(false) {}

    const std::string& me::getLocalName() const {
        return _name;
    }

    str me::getContainer() {
        return *_container;
    }

    str me::getEval() const {
        if(_initEval) return super::getEval();

        str ased = _container->getEval();
        str elemType = ased->run("getElemType");
        if(!elemType) return NAMU_E("elemType == null"), str();

        blockExpr& blk = getBlock();
        frameInteract f1(blk); {
            auto& fr = thread::get()._getNowFrame();
            if(nul(fr)) return str();
            fr.addLocal(getLocalName(), *((node*) elemType->clone()));

            _initEval = true;
            str newEval = blk.getEval();
            if(!newEval) return newEval;
            if(!newEval->isSub<retExpr>())
                newEval.bind(new arr(*newEval));
            setEval(*newEval);
            return newEval;
        }
    }

    namespace {
        class forLoop : public me::loop {
            NAMU(CLASS(forLoop, loop))

        public:
            forLoop(arr& ret, str iter, const forExpr& owner): super(ret), _iter(iter), _owner(owner) {}

        public:
            nbool isLooping() override {
                return !_iter->run("isEnd")->cast<nbool>();
            }

            void run(blockExpr& blk, frame& fr) override {
                str elem = _iter->run("get");
                if(!elem)
                    return NAMU_E("elem is null"), void();
                fr.addLocal(_owner.getLocalName(), *elem);

                super::run(blk, fr);
            }

            nbool postprocess(frame& fr) override {
                _iter->run("next", args{narr{*new nInt(1)}});
                return super::postprocess(fr);
            }

        private:
            str _iter;
            const forExpr& _owner;
        };
    }

    tstr<me::loop> me::_makeLoop(arr& ret) const {
        str ased = _container->as<node>();
        if(!ased) return NAMU_E("ased is null"), str();
        str iter = ased->run("iterate", args{narr{*new nInt(0)}});
        if(!iter) return NAMU_E("iter is null"), str();

        NAMU_DI("forExpr: loop %s in %s", getLocalName().c_str(), ased->getSrc().getName().c_str());
        return new forLoop(ret, iter, *this);
    }
}
