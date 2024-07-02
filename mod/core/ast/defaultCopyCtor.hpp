#pragma once

#include "ctor.hpp"
#include "params.hpp"

namespace nm {

    class visitor;

    class _nout defaultCopyCtor : public ctor {
        NM(CLASS(defaultCopyCtor, ctor), VISIT())

    public:
        defaultCopyCtor(const node& org);

    public:
        using super::run;
        str run(const args& a) override;

        const params& getParams() const override;

    public:
        params _params;
    };
}
