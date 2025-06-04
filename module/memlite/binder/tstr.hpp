#pragma once

#include "memlite/binder/strTactic.hpp"
#include "memlite/binder/tweak.hpp"

namespace nm {

    template <typename T, typename TACTIC = strTactic> class tstr: public tweak<T, TACTIC> {
        typedef tweak<T, TACTIC> _super_;
        NM_ME(tstr, _super_)
        NM_INIT_META(me)
        friend class ref;

    public:
        //  tstr:
        tstr();
        /// @param subType  subType is should be sub type of 'T' or type 'T' will be used from base
        ///                 class.
        tstr(const type& subtype);
        tstr(const T& it);
        tstr(const T* it);
        tstr(const binder& rhs);
        tstr(const me& rhs);

        me& operator=(const me& rhs) = default;
    };
}
