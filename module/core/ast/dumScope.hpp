#pragma once

#include "core/ast/scope.hpp"

namespace by {
    class _nout dumScope: public scope {
        BY(CLASS(dumScope, scope))

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
