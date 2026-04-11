/// @file
#pragma once

#include "core/ast/pod.hpp"
#include "core/ast/src/src.hpp"
#include "core/loader/pod/trustPodLoading.hpp"

namespace by {

    /** @ingroup core
     *  @brief C++ native pod loading implementation
     *  @details Specialized pod loader for C++ native libraries and modules.
     *  Handles dynamic library loading and native code integration with byeol.
     */
    class _nout cppPodLoading: public trustPodLoading {
        BY(CLASS(cppPodLoading, trustPodLoading))
        typedef void (*entrypointFunc)(bicontainable*);

    public:
        nbool parse(errReport& rpt, pod& pak) override;
        const std::string& getName() const override;
        void rel() override;

    private:
        /**
         * @brief Dynamically loads native C++ libraries and integrates them into the pod.
         * @details This method uses platform-specific dynamic loading (e.g., `dlib`) to load
         *          C++ shared libraries, find their entry points, and integrate their components
         *          into the Byeol runtime.
         * @param rpt The error report to collect any loading errors.
         * @param tray A bicontainable to store the loaded components or symbols.
         * @return true on successful loading, false otherwise.
         */
        nbool _loadLibs(errReport& rpt, pod& pak);

    private:
        dlibs _dlibs;
    };
}
