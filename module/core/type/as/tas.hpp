/// @file
#pragma once

#include "core/builtin/err/errCode.hpp"
#include "core/type/as/aser.hpp"
#include "core/ast/node.hpp"
#include "core/type/ntype.hpp"

namespace by {

    template <typename T, typename Me> struct tas: public aser {
        typedef tas<T, Me> __me1;
        BY(ME(__me1, aser))

    public:
        virtual ~tas() = default;

    public:
        using super::is;

        str as(const node& me, const type& to) const override {
            const Me& cast = me.cast<Me>() OR.exErr(CAST_NOT_AVAILABLE, me, to).ret(str());
            return _onAs(cast, to);
        }

        nbool is(const type& from, const type& to) const override { return to == ttype<T>::get(); }

    protected:
        virtual str _onAs(const Me& me, const type& to) const = 0;
    };
}
