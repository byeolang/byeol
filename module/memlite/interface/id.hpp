/// @file
#pragma once

#include "memlite/common.hpp"

namespace by {

    /// @ingroup memlite
    /// @brief Unique identifier for instances in memlite system
    /// @details Compact 64-bit identifier with tag number, chunk number, and serial number.
    /// Provides efficient instance identification and validation.
    struct _nout id {
        id(nint new_tagN, nint newChkN, nuint newSerial);
        id();

    public:
        nbool operator==(const id& rhs) const;
        nbool operator!=(const id& rhs) const;

    public:
        void rel();
        nbool isHeap() const;
        nbool isValid() const;

    public:
        nint tagN: 21;
        nint chkN: 22;
        nuint serial: 21;
    };
}
