/// @file
#pragma once

#include "memlite/common.hpp"

namespace by {

    /** @ingroup memlite
     *  @brief Unique identifier for instances in memlite system
     *  @details 64-bit integer @ref instance identifier. tagN identifies @ref life, chkN
     *  indicates which @ref chunk, and serial is used for object validation.
     *
     *  @section serial is instance creation count during process execution
     *  As seen from @ref pool and chunk, using a custom memory pool means when memory is
     *  released, only the destructor is called without initializing all memory. Previously
     *  allocated and destroyed data remains, and accessing this data is even possible. (If
     *  accessing used data causes exceptions or UB, implementing @ref binder with weak or
     *  strong pointers would be impossible.) binder additionally compares serial to
     *  distinguish whether retrieved data is truly valid.
     *
     *  @section tagN used for accessing life objects
     *  @ref watcher uses this value to retrieve the tagN-th @ref life object from its array.
     *
     *  @section chkN used for retrieving chunk objects
     *  @ref pool first calculates instance size mapped to id to retrieve @ref chunks. Then
     *  chunks returns the memory block at its chkN-th element. Externally, the received
     *  memory address and serial value are compared to verify it's the same instance.
     */
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
