#include "core/ast/exprs/forExpr.hpp"

#include <utility>

#include "core/builtin.hpp"
#include "core/frame/frameInteract.hpp"
#include "core/frame/thread.hpp"
#include "core/worker/visitor/visitor.hpp"
#include "core/internal/coreInternal.hpp"

namespace by {

    BY(DEF_ME(forExpr), DEF_VISIT())

    me::forExpr(const std::string& localName, const node& container, const blockExpr& blk):
        super(blk), _container(container), _name(localName) {}

    const std::string& me::getLocalName() const { return _name; }

    str me::getContainer() { return *_container; }

    str me::_makeEval() const {
        str ased = _container->infer();
        str elemType = ased->eval("getElemType") OR.err("elemType == null").ret(str());
        frame& fr = coreInternal::getNowFrame() OR.ret(str());

        frameInteract f1(getBlock());
        fr.addLocal(getLocalName(), *((node*) elemType->clone()));
        return super::_makeEval();
    }

    namespace {
        class forLoop: public me::loop {
            BY(CLASS(forLoop, loop))

        public:
            forLoop(arr* ret, const str& container, const str& iter, const forExpr& owner):
                super(ret), _container(container), _iter(iter), _owner(owner) {}

        public:
            nbool isLooping() override {
                nbool isEnd = _iter->eval("isEnd")->cast<nbool>() OR.ret(false);
                return !isEnd;
            }

            void eval(blockExpr& blk, frame& fr) override {
                str elem = _iter->eval("get") OR.err("elem is null").ret();
                fr.addLocal(_owner.getLocalName(), *elem);

                super::eval(blk, fr);
            }

            nbool postprocess(frame& fr) override {
                _iter->eval("next", args(narr(*new nInt(1))));
                return super::postprocess(fr);
            }

        private:
            str _container;
            str _iter;
            const forExpr& _owner;
        };
    }

    tstr<me::loop> me::_makeLoop(arr* ret) const {
        str ased = _container->as<node>() OR.err("ased is null").ret(str());
        str iter = ased->eval("iterate", args(narr(*new nInt(0)))) OR.err("iter is null").ret(str());

        BY_DI("forExpr: loop %s in %s", getLocalName(), ased->getSrc());
        return new forLoop(ret, ased, iter, *this);
    }
} // namespace by
