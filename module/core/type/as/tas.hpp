/// @file
#pragma once

#include "core/type/as/aser.hpp"
#include "core/type/ntype.hpp"

namespace by {

    template <typename T> struct tas: public aser {
        BY(ADT(tas<T>, aser))

    public:
        virtual ~tas() = default;

    public:
        using super::is;

        nbool is(const type& from, const type& to) const override { return to == ttype<T>::get(); }
    };
}
