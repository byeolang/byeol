#include "core/ast/exprs/whileExpr.hpp"

#include "core/frame/frameInteract.hpp"
#include "core/frame/thread.hpp"
#include "core/worker/visitor/visitor.hpp"

namespace nm {

    NM(DEF_ME(whileExpr), DEF_VISIT())

    me::whileExpr(const node& condition, const blockExpr& blk): super(blk), _condition(condition) {}

    node& me::getCondition() { return *_condition; }

    namespace {
        class whileLoop: public me::loop {
            NM(CLASS(whileLoop, loop))

        public:
            whileLoop(arr& ret, const whileExpr& owner): super(ret), _owner(owner) {}

        public:
            nbool isLooping() override {
                tstr<nBool> ased = _owner.getCondition()
                               .asImpli<nBool>() OR.err("cast to bool has been failed.")
                               .ret(false);
                return *ased->cast<nbool>();
            }

        private:
            const whileExpr& _owner;
        };
    }

    tstr<me::loop> me::_makeLoop(arr& ret) const {
        NM_DI("whileExpr: loop");
        return new whileLoop(ret, *this);
    }
} // namespace nm
