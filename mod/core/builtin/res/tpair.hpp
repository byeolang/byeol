#pragma once

#include "../../common.hpp"

namespace nm {

    template <typename L, typename R> struct tpair {
        tpair() {}

        tpair(const L& newL, const R& newR): l(newL), r(newR) {}

        L l;
        R r;
    };

    template <typename L, typename R> struct tpair<L&, R> {
        tpair(): l(nulOf<L>()) {}

        tpair(L& newL, const R& newR): l(newL), r(newR) {}

        L& l;
        R r;
    };

    template <typename L, typename R> struct tpair<L, R&> {
        tpair(): r(nulOf<R>()) {}

        tpair(const L& newL, R& newR): l(newL), r(newR) {}

        L l;
        R& r;
    };

    template <typename L, typename R> struct tpair<L&, R&> {
        tpair(): l(nulOf<L>()), r(nulOf<R>()) {}

        tpair(L& newL, R& newR): l(newL), r(newR) {}

        L& l;
        R& r;
    };
} // namespace nm
