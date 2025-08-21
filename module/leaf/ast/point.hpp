/// @file
#pragma once

#include "leaf/common.hpp"

namespace by {
    /// @ingroup leaf
    /// @brief Source code position tracking structure
    /// @details Represents a position in source code with row and column information.
    /// Used for error reporting and source location tracking during parsing.
    struct _nout point {
        point& operator++();

    public:
        nbool isOrigin() const;
        void rel();

    public:
        ncnt row;
        ncnt col;
    };

    /// @ingroup leaf
    /// @brief Source code area tracking structure
    /// @details Represents a range in source code from start point to end point.
    /// Used for tracking spans of tokens or expressions in the source.
    struct _nout area {
        area& operator++();

    public:
        void rel();

    public:
        point start;
        point end;
    };
}
