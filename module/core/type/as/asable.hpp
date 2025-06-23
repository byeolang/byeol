#pragma once

#include "core/common.hpp"

namespace nm {

    class _nout asable {
    public:
        virtual ~asable() = default;

    public:
        virtual nbool is(const type& from, const type& to) const = 0;
        nbool is(const type& from, const type* to) NM_SIDE_FUNC(to, is(from, *to), false);
        nbool is(const type* from, const type& to) NM_SIDE_FUNC(from, is(*from, to), false);
        nbool is(const type* from, const type* to) NM_SIDE_FUNC(from && to, is(*from, *to), false);

        virtual str as(const node& from, const type& to) const = 0;
        str as(const node& from, const type* to) const NM_SIDE_FUNC(to, as(from, *to), str());
        str as(const node* from, const type& to) const NM_SIDE_FUNC(from, as(*from, to), str());
        str as(const node* from, const type* to) const NM_SIDE_FUNC(from && to, as(*from, *to), str());
    };
}
