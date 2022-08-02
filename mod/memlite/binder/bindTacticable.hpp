#pragma once

#include "../common.hpp"

namespace namu {

    class binder;
    class instance;
    class _nout bindTacticable {
    public:
        virtual void rel(binder& me) = 0;
        virtual nbool bind(binder& me, const instance& it) = 0;
    };
}
