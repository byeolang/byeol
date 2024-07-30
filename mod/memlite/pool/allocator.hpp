#pragma once

#include "memoryHaver.hpp"

namespace nm {

    class _nout allocator: public memoryHaver {
        NM_ME(allocator)
        NM_INIT_META(me)

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
