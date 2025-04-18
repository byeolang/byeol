#pragma once

#include "scope.hpp"

namespace nm {
    class _nout dumScope: public scope {
        NM(CLASS(dumScope, scope))

    public:
        dumScope();
        dumScope(const scope::super& org);

    public:
        // add:
        using super::add;
        nbool add(const std::string& key, const node& val) override;

    public:
        static me& singleton();
    };
}
