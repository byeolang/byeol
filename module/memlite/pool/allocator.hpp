/// @file
#pragma once

#include "memlite/pool/memoryHaver.hpp"

namespace by {

    /** @ingroup memlite
     *  @brief Abstract allocator interface for memory management
     *  @details Inherits from @ref memoryHaver, so can measure state and size of managed
     *  memory. Additionally can allocate and deallocate memory.
     *
     *  @remark void* and byte-only perspective
     *  Following memlite's concept of viewing all memory only as void* and bytes, new()
     *  and del() parameters only provide void*.
     *
     *  @section naming_conventions naming conventions
     *  Allocation uses new1() (meaning "new one"), deallocation uses del(). This naming
     *  is frequently used not only in memlite but throughout the byeol project.
     */
    class _nout allocator: public memoryHaver {
        BY_ME(allocator)
        BY_INIT_META(me)

    public:
        //  allocator:
        explicit allocator(ncnt blksize = 1);

    public:
        virtual void* new1() = 0;
        virtual nbool del(void* used, ncnt sz) = 0;
        ncnt getBlkSize() const;

    private:
        ncnt _blksize;
    };
}
