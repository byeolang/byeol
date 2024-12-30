#include "defaultCtor.hpp"

#include "../../worker/visitor/visitor.hpp"
#include "../../type/typeMaker.hpp"

namespace nm {

    NM(DEF_ME(defaultCtor), DEF_VISIT())

    me::defaultCtor(const node& org): super(typeMaker::make<defaultCtor>(params(), org)) {}

    str me::run(const args& a) { return a.getMe(); }
}
