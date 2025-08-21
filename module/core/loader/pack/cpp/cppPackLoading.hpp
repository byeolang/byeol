/// @file
#pragma once

#include "core/ast/slot.hpp"
#include "core/ast/src/src.hpp"
#include "core/loader/pack/trustPackLoading.hpp"

namespace by {

    /// @ingroup core
    /// @brief C++ native pack loading implementation
    /// @details Specialized pack loader for C++ native libraries and modules.
    /// Handles dynamic library loading and native code integration with byeol.
    class _nout cppPackLoading: public trustPackLoading {
        BY(CLASS(cppPackLoading, trustPackLoading))
        typedef void (*entrypointFunc)(bicontainable*);

    public:
        tstr<srcs> parse(errReport& rpt, bicontainable& tray) override;
        const std::string& getName() const override;
        void rel() override;

    private:
        nbool _loadLibs(errReport& rpt, bicontainable& tray);

    private:
        dlibs _dlibs;
    };
}
