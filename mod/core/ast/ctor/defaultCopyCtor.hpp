#pragma once

#include "baseCtor.hpp"
#include "../params.hpp"

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

        const ntype& getType() const override;

    private:
        mgdType _type;
    };
}
