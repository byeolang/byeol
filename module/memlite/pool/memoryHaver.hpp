#pragma once

#include "memlite/common.hpp"

namespace by {

    class instance;

    class _nout memoryHaver {
        NM_ME(memoryHaver)
        NM_INIT_META(me)

    public:
        virtual ~memoryHaver() {}

    public:
        virtual nbool has(const instance& it) const = 0;
        nbool has(const instance* it) const NM_SIDE_FUNC(has);
        virtual ncnt len() const = 0;
        virtual ncnt size() const = 0;
        virtual nbool rel() = 0;
        nbool isFull() const;
        nbool isCapable() const;

    protected:
        virtual void* _get(nidx n) = 0;
    };
}
