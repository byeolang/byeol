/// @file
#pragma once

#include "frontend/flag/flags.hpp"

namespace by {

    /// @ingroup byeol
    /// @brief Command-line interface for byeol language system
    /// @details Handles command-line argument parsing and execution flow
    /// for the byeol language interpreter and related tools.
    struct cli {
        const flags& getFlags() const;

        nbool run(flagArgs& a);

        nint getRes() const;

    private:
        nint _res;
    };
}
