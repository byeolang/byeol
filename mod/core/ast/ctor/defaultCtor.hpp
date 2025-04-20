#pragma once

#include "core/ast/ctor/baseCtor.hpp"
#include "core/ast/args.hpp"

namespace nm {

    class visitor;

    class _nout defaultCtor: public baseCtor {
        NM(ME(defaultCtor, baseCtor), CLONE(defaultCtor), VISIT())

    public:
        defaultCtor(const node& org);

    public:
        using super::run;
        str run(const args& a) override;
    };
}
