#pragma once

#include "core/common.hpp"
#include "core/type/ntype.hpp"

namespace nm {

    /// @remark arrayable has API treating index as its parameter.
    template <typename T, typename R = T*, typename RSquare = T&> class tarrayable {
        NM_ME(tarrayable)

    public:
        virtual ~tarrayable() {}

        // operator:
        virtual RSquare operator[](nidx n) = 0;

        const RSquare operator[](nidx n) const NM_CONST_FUNC(operator[](n))

        // in:
        virtual nbool in(nidx n) const = 0;

        // get:
        virtual R get(nidx n) = 0;
        const R get(nidx n) const NM_CONST_FUNC(get(n))

        // set:
        virtual nbool set(nidx n, const T& new1) = 0;
        nbool set(nidx n, const T* new1) NM_SIDE_FUNC(new1, set(n, *new1), false);

        // add:
        /// @return how many element has been added from rhs.
        virtual nbool add(nidx n, const T& new1) = 0;
        nbool add(nidx n, const T* new1) NM_SIDE_FUNC(new1, add(n, *new1), false);

        // del:
        /// delete last element if exists.
        /// @return true if element got deleted successfully.
        virtual nbool del(nidx n) = 0;
    };
} // namespace nm
