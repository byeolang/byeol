#pragma once

#include "tnarr.inl"

namespace wrd {

    template <typename T, typename TACTIC = strTactic>
    class tndumArr: public tnarr<T, TACTIC> {
        typedef tnarr<T, TACTIC> _super_;
        WRD(CLASS(tndumArr, _super_))

    public:
        tndumArr() {}

        // add:
        using super::add;
        wbool add(const typename super::iter& e, const node& new1) override { return false; }
        wbool add(widx n, const node& new1) override { return false; }

    public:
        static me singletone;
    };

    typedef tndumArr<node, strTactic> ndumArr;
}