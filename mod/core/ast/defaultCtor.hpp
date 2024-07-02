#pragma once

#include "ctor.hpp"

namespace nm {

    class visitor;

    class _nout defaultCtor : public ctor {
        NAMU(CLASS(defaultCtor, ctor), VISIT())

    public:
        defaultCtor(const node& org);

    public:
        using super::run;
        str run(const args& a) override;
    };
}
