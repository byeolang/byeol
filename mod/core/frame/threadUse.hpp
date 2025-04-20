#pragma once

#include "core/frame/thread.hpp"

namespace nm {
    class _nout threadUse {
        NM(ME(threadUse))

    public:
        threadUse();
        threadUse(const errReport& new1);
        ~threadUse();

    public:
        thread& get();
        const thread& get() const NM_CONST_FUNC(get())

        void use();
        void rel();

    private:
        thread _thr;
    };
}
