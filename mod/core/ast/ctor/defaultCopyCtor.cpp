#include "defaultCopyCtor.hpp"

#include "../../worker/visitor/visitor.hpp"

namespace nm {

    NM(DEF_ME(defaultCopyCtor), DEF_VISIT())

    me::defaultCopyCtor(const node& org): super(org) { _params.add(*new param("rhs", org)); }

    str me::run(const args& a) {
        if(!canRun(a)) return str();

        const node& p = getParams()[0].getOrigin();
        return str((node*) a.begin()->as(p)->clone());
    }

    const params& me::getParams() const { return _params; }
}
