#pragma once

#include "memlite/pool/memoryHaver.hpp"

namespace by {

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
