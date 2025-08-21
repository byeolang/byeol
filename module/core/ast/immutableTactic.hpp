/// @file
#pragma once

#include "core/common.hpp"

namespace by {

    /// @ingroup core
    /// @brief Immutable binding strategy
    /// @details Strategy that prevents binding modifications for immutable objects.
    class _nout immutableTactic: public strTactic {
        BY(ME(immutableTactic, strTactic))

    public:
        using super::bind;
        nbool bind(binder& me, const instance& it) override;

    public:
        static me singleton;
    };
}
