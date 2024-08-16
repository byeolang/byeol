#pragma once

#include "asable.hpp"

namespace nm {

    class ntype;

    template <typename T> struct tas: public asable {
        virtual ~tas() = default;

        nbool is(const type& from, const type& to) const override { return to == ttype<T>::get(); }
    };
}
