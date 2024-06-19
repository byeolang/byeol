#include "whileExpr.hpp"
#include "../../visitor/visitor.hpp"
#include "../../frame/thread.hpp"
#include "../../frame/frameInteract.hpp"

namespace namu {

    NAMU(DEF_ME(whileExpr), DEF_VISIT())

    me::whileExpr(const node& condition, const blockExpr& blk): super(blk), _condition(condition) {}

    node& me::getCondition() {
        return *_condition;
    }

    namespace {
        class whileLoop : public me::loop {
            NAMU(CLASS(whileLoop, loop))

        public:
            whileLoop(arr& ret, const whileExpr& owner): super(ret), _owner(owner) {}

        public:
            nbool isLooping() override {
                str ased = _owner.getCondition().asImpli<nBool>();
                if(!ased) return NAMU_E("cast to bool has been failed."), false;
                return ased->cast<nbool>();
            }

        private:
            const whileExpr& _owner;
        };
    }

    tstr<me::loop> me::_makeLoop(arr& ret) const {
        NAMU_DI("whileExpr: loop");
        return new whileLoop(ret, *this);
    }
}
