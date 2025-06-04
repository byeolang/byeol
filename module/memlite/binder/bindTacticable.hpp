#pragma once

#include "memlite/common.hpp"

namespace nm {

    class binder;
    class instance;

    class _nout bindTacticable {
    public:
        virtual void rel(binder& me) = 0;
        virtual nbool bind(binder& me, const instance& it) = 0;
        nbool bind(binder& me, const instance* it) NM_SIDE_FUNC(it, bind(me, *it), false)
    };
}
