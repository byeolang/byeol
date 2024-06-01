#pragma once

#include "scope.hpp"

namespace namu {
    class _nout dumScope : public scope {
        NAMU(CLASS(dumScope, scope))

    public:
        dumScope();
        dumScope(const node& owner);
        dumScope(const super& rhs);

    public:
        // add:
        using super::add;
        nbool add(const std::string& key, const node& val) override;

        me* cloneDeep() const override;
    };
}