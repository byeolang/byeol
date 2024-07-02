#pragma once

#include "thread.hpp"

namespace nm {
    class _nout threadUse {
        NAMU(ME(threadUse))

    public:
        threadUse();
        threadUse(const errReport& new1);
        ~threadUse();

    public:
        thread& get();
        const thread& get() const NAMU_UNCONST_FUNC(get())

        void use();
        void rel();

    private:
        thread _thr;
    };
}
