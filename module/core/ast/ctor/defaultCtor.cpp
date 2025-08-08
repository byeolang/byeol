#include "core/ast/ctor/defaultCtor.hpp"

#include "core/worker/visitor/visitor.hpp"
#include "core/type/typeMaker.hpp"

namespace by {

    NM(DEF_ME(defaultCtor), DEF_VISIT())

    me::defaultCtor(const node& org): super(typeMaker::make<defaultCtor>(params(), &org)) {}

    str me::run(const args& a) { return a.getMe(); }
}
