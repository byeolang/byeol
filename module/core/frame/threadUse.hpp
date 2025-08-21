/// @file
#pragma once

#include "core/frame/thread.hpp"

namespace by {
    /// @ingroup core
    /// @brief Thread usage wrapper
    /// @details RAII wrapper for thread management with automatic setup and cleanup.
    class _nout threadUse {
        BY(ME(threadUse))

    public:
        threadUse();
        threadUse(const errReport& new1);
        ~threadUse();

    public:
        thread& get();
        const thread& get() const BY_CONST_FUNC(get())

        void use();
        void rel();

    private:
        thread _thr;
    };
}
