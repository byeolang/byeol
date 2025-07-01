#include "core/ast/ctor/defaultCopyCtor.hpp"

#include "core/worker/visitor/visitor.hpp"
#include "core/ast/args.hpp"
#include "core/type/typeMaker.hpp"

namespace nm {

    NM(DEF_ME(defaultCopyCtor), DEF_VISIT())

    me::defaultCopyCtor(const node& org):
        super(typeMaker::make<me>(params(*new param("rhs", org)), &org)) {}

    str me::run(const args& a) {
        WHEN(!canRun(a)).ret(str());

        const node& p = getParams()[0].getOrigin();
        return str((node*) a.begin()->as(p)->clone());
    }
}
