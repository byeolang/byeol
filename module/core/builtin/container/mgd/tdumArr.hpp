/// @file
#pragma once

#include "core/builtin/container/mgd/tarr.hpp"
#include "core/ast/origin.hpp"

namespace by {
    /// @ingroup core
    /// @brief Dummy array that ignores all modifications
    /// @details No-op array implementation that accepts but ignores all set/add operations.
    template <typename T, typename defaultElemType = T> class _nout tdumArr: public tarr<T, defaultElemType> {
        typedef tarr<T, defaultElemType> __super9;
        BY(CLASS(tdumArr, __super9))
        typedef typename super::iter iter;

    public:
        tdumArr(): super(*new origin(*new mgdType("node", ttype<node>::get(), params()))) {}

        tdumArr(const node& elemType): super(elemType) {}

    public:
        using tucontainable<node>::set;
        using tarrayable<node>::set;

        nbool set(const iter& at, const node& new1) override { return true; }

        nbool set(nidx n, const node& new1) override { return true; }

        using tucontainable<node>::add;
        using tarrayable<node>::add;

        nbool add(const iter& at, const node& new1) override { return true; }

        nbool add(nidx n, const node& new1) override { return true; }

        void add(const iter& here, const iter& from, const iter& to) override {}
    };

    typedef tdumArr<node, mockObj> dumArr;
} // namespace by
