#pragma once

#include "baseCtor.hpp"

namespace nm {

    class visitor;

    class _nout defaultCtor: public baseCtor {
        NM(CLASS(defaultCtor, baseCtor), VISIT())

    public:
        defaultCtor(const node& org);

    public:
        using super::run;
        str run(const args& a) override;
    };
}
