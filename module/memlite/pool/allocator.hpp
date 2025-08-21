/// @file
#pragma once

#include "memlite/pool/memoryHaver.hpp"

namespace by {

    /// @ingroup memlite
    /// @brief Abstract allocator interface for memory management
    /// @details Provides block-based allocation and deallocation interface.
    /// Base class for various allocation strategies in the memory pool system.
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
