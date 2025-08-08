#pragma once

#include "memlite/interface/instance.hpp"

namespace by {

    template <typename T> class tbindable {
        NM_ME(tbindable)
        NM_INIT_META(me)

    public:
        //  tbindable:
        explicit operator nbool() const;

    public:
        virtual nbool bind(const T& it);
        /// @remark binder will loose previous binding instance whether given parameter `it` isn't
        ///         nullptr or not.
        nbool bind(const T* it);

        virtual void rel() = 0;
        nbool canBind(const T& it) const;
        nbool canBind(const T* it) const NM_SIDE_FUNC(canBind);
        virtual nbool canBind(const type& it) const = 0;
        nbool canBind(const type* it) const NM_SIDE_FUNC(canBind);
        virtual nbool isBind() const = 0;
    };
}
