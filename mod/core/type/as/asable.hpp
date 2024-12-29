#pragma once

#include "../../common.hpp"

namespace nm {

    class _nout asable {
    public:
        virtual ~asable() = default;

    public:
        virtual nbool is(const type& from, const type& to) const = 0;
        virtual str as(const node& from, const type& to) const = 0;
    };
}
