/// @file
#pragma once

#include "core/ast/pack.hpp"
#include "core/ast/src/src.hpp"
#include "core/loader/pack/trustPackLoading.hpp"

namespace by {

    /** @ingroup core
     *  @brief C++ native pack loading implementation
     *  @details Specialized pack loader for C++ native libraries and modules.
     *  Handles dynamic library loading and native code integration with byeol.
     */
    class _nout cppPackLoading: public trustPackLoading {
        BY(CLASS(cppPackLoading, trustPackLoading))
        typedef void (*entrypointFunc)(bicontainable*);

    public:
        tstr<srcs> parse(errReport& rpt, bicontainable& tray) override;
        const std::string& getName() const override;
        void rel() override;

    private:
        /**
         * @brief Dynamically loads native C++ libraries and integrates them into the pack.
         * @details This method uses platform-specific dynamic loading (e.g., `dlib`) to load
         *          C++ shared libraries, find their entry points, and integrate their components
         *          into the Byeol runtime.
         * @param rpt The error report to collect any loading errors.
         * @param tray A bicontainable to store the loaded components or symbols.
         * @return true on successful loading, false otherwise.
         */
        nbool _loadLibs(errReport& rpt, bicontainable& tray);

    private:
        dlibs _dlibs;
    };
}
