#include "defaultCopyCtor.hpp"

#include "../../worker/visitor/visitor.hpp"
#include "../args.hpp"
#include "../../type/typeMaker.hpp"

namespace nm {

    NM(DEF_ME(defaultCopyCtor), DEF_VISIT())

    me::defaultCopyCtor(const node& org):
        super(typeMaker::make<me>(params(*new param("rhs", org)), org)) {}

    str me::run(const args& a) {
        WHEN(!canRun(a)).ret(str());

        const node& p = getParams()[0].getOrigin();
        return str((node*) a.begin()->as(p)->clone());
    }
}
