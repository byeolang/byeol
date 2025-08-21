/// @file
#pragma once

#include "core/builtin/container/native/tnarr.inl"

namespace by {

    template <typename T, typename TACTIC = strTactic> class tndumArr: public tnarr<T, TACTIC> {
        typedef tnarr<T, TACTIC> _super_;
        BY(CLASS(tndumArr, _super_))

    public:
        tndumArr() {}

        tndumArr(const super& rhs): super(rhs) {}

    public:
        // add:
        using super::add;

        nbool add(const typename super::iter& e, const node& new1) override { return false; }

        nbool add(nidx n, const node& new1) override { return false; }

    public:
        static me singleton;
    };

    typedef tndumArr<node, strTactic> ndumArr;
}
