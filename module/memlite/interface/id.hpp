#pragma once

#include "memlite/common.hpp"

namespace by {

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
