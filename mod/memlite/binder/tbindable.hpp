#pragma once

#include "../interface/instance.hpp"

namespace nm {

    template <typename T> class tbindable {
        NM_ME(tbindable)
        NM_INIT_META(me)

    public:
        //  tbindable:
        explicit operator nbool() const;

    public:
        virtual nbool bind(const T& it);
        nbool bind(const T* it);
        virtual void rel() = 0;
        nbool canBind(const T& it) const;
        virtual nbool canBind(const type& it) const = 0;
        virtual nbool isBind() const = 0;
    };
}
