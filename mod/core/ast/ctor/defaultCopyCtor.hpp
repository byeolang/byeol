#pragma once

#include "core/ast/ctor/baseCtor.hpp"

namespace nm {

    class visitor;

    class _nout defaultCopyCtor: public baseCtor {
        NM(ME(defaultCopyCtor, baseCtor), INIT_META(defaultCopyCtor), CLONE(defaultCopyCtor),
            VISIT())

    public:
        defaultCopyCtor(const node& org);

    public:
        using super::run;
        str run(const args& a) override;
    };
}
