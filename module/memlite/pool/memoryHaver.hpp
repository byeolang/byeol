/// @file
#pragma once

#include "memlite/common.hpp"

namespace by {

    class instance;

    /** @ingroup memlite
     *  @brief Abstract interface for objects that own memory
     *  @details Base interface for classes that directly or indirectly own memory in the
     *  memory pool and can READ that memory. Defines interfaces for querying memory size
     *  and state.
     *
     *  @section Indirect ownership?
     *  "Indirect ownership" means the object doesn't directly allocate and use memory, but
     *  internal member variables handle it. If memory allocation must be done through
     *  internal members, the class should only inherit memoryHaver. If allocation is also
     *  possible, inherit @ref allocator.
     *
     *  @section len and size
     *  Allocatable memory size is expressed as size, while allocated memory is expressed
     *  as len. Control is based solely on void* and byte units.
     *
     *  @remark memoryHaver derivatives manage different memory sizes
     *  Note that memoryHaver derived classes each manage different sizes of memory.
     */
    class _nout memoryHaver {
        BY_ME(memoryHaver)
        BY_INIT_META(me)

    public:
        virtual ~memoryHaver() {}

    public:
        virtual nbool has(const instance& it) const = 0;
        nbool has(const instance* it) const BY_SIDE_FUNC(has);
        virtual ncnt len() const = 0;
        virtual ncnt size() const = 0;
        virtual nbool rel() = 0;
        nbool isFull() const;
        nbool isCapable() const;

    protected:
        virtual void* _get(nidx n) = 0;
    };
}
