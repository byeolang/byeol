#include "core/ast/ctor/defaultCopyCtor.hpp"

#include "core/worker/visitor/visitor.hpp"
#include "core/ast/args.hpp"
#include "core/type/typeMaker.hpp"
#include "core/ast/ctor.hpp"

namespace by {

    BY(DEF_ME(defaultCopyCtor), DEF_VISIT())

    me::defaultCopyCtor(const node& org): super(typeMaker::make<me>(params(*new param("rhs", org)), &org)) {}

    str me::eval(const args& a) {
        WHEN(!canEval(a)) .ret(str());
        const obj& rhs = a.begin()->cast<obj>() OR.ret(str());
        obj& me = a.getMe()->cast<obj>() OR.ret(str());

        auto& owns = me.getOwns();
        owns.rel();
        owns.add(rhs.getOwns());
        return me;
    }

    const src& me::getSrc() const {
        static src inner(srcFile(), ctor::CTOR_NAME, point{});
        return inner;
    }
}
