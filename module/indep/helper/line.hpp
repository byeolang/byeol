/// @file
#pragma once

#include "indep/def/_nout.hpp"
#include "indep/macro.hpp"

namespace by {

    /// @ingroup indep
    /// @brief Manages hierarchical depth visualization for logging and tracing.
    /// @details Thread-safe line depth management using thread_local storage.
    class _nout line {
        BY(ME(line))

    public:
        /// @brief Increases the current depth level.
        static void incLv();

        /// @brief Decreases the current depth level.
        static void decLv();

        /// @brief Returns the current depth level.
        static ncnt getLv();

        /// @brief Returns line visualization string for current depth.
        /// @details Returns predefined string_view without copying.
        static const std::string& getLine();
    };

} // namespace by
