#pragma once

#include "tarr.hpp"

namespace namu {
    template <typename T>
    class _nout tdumArr : public tarr<T> {
        NAMU(CLASS(tdumArr, tarr<T>))
        typedef typename super::iter iter;

    public:
        tdumArr() {}
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

    typedef tdumArr<node> dumArr;
}
