#pragma once

#include "core/type/as/aser.hpp"
#include "core/type/ntype.hpp"

namespace nm {

    template <typename T> struct tas: public aser {
        NM(ADT(tas<T>, aser))

    public:
        virtual ~tas() = default;

        nbool is(const type& from, const type& to) const override { return to == ttype<T>::get(); }
    };
}
