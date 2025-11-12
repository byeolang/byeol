#include "core/type/funcType.hpp"
#include "core/ast/ctor/ctor.hpp"
#include "core/type/as/impliAses.hpp"
#include "core/ast/baseFunc.hpp"
#include "core/builtin/primitive/nStr.hpp"

namespace by {
    BY(DEF_ME(funcType))

    me::funcType(const std::string& typeName, const type& superType, const params& ps, nbool isAdt, const node* ret):
        super(typeName, superType, ps, isAdt, ret) {}

    nbool me::operator==(const type& rhs) const {
        WHEN(&rhs == this) .ret(true);
        return isSameSign(rhs);
    }

    namespace {
        struct baseFuncAsBaseFunc: public tas<baseFunc, baseFunc> {
            typedef tas<baseFunc, baseFunc> __super25;
            BY(CLASS(baseFuncAsBaseFunc, __super25))

        public:
            nbool is(const type& from, const type& to) const override {
                WHEN(from.getName() == ttype<ctor>::get().getName()) .ret(false); // ctor can't be casted to any func.
                // TODO: not support closure by `as` yet, but it's TODO.
                // TODO: WHEN(from.getName() == ttype<closure>::get().getName()).ret(false);

                return from == to;
            }

        protected:
            str _onAs(const baseFunc& me, const type& to) const override {
                // if you're about to run this func, it means that our libmeta confirm that this
                // is castable action. so don't try to call `is()` again.
                return me;
            }
        };

        /*str baseFuncAsClosure::_onAs(const baseFunc& me, const type& to) const {
            WHEN(me.isSub<closure>()).ret(me);
            baseObj& meObj = thread::get().getNowFrame() TO(getMe()) TO(template cast<baseObj>()) OR.ret(str());
            return new closure(meObj, me);
        }*/
    }

    const impliAses& me::_getImpliAses() const {
        static impliAses inner{new baseFuncAsBaseFunc()};
        return inner;
    }

    namespace {
        struct baseFuncAsStr: public tas<nStr, baseFunc> {
            typedef tas<nStr, baseFunc> __super12;
            BY(CLASS(baseFuncAsStr, __super12))

        protected:
            str _onAs(const baseFunc& me, const type& to) const override {
                return new nStr(me.getSrc().getName() + "(" + me.getParams().toStr() + ")");
            }
        };
    }

    const ases& me::_getAses() const {
        static ases inner(*new baseFuncAsStr());
        return inner;
    }
} // namespace by
