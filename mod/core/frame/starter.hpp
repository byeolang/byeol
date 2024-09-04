#pragma once

#include "../ast/origin.hpp"
#include "../worker/worker.inl"
#include "defaultSigZone.hpp"

namespace nm {
    class frames;

    class _nout starter: public worker<str, node> {
        typedef worker<str, node> __super8;
        NM(CLASS(starter, __super8))

    public:
        enum logFlag3 {
            LOG_STRUCTURE = 1 << 7,
            LOG_GRAPH_ON_EX = 1 << 8
        };

    public:
        static inline const std::string MAIN = "main";

    public:
        void setArgs(const args& a);
        args& getArgs();
        const args& getArgs() const NM_CONST_FUNC(getArgs())

    protected:
        str _onWork() override;
        void _prepare() override;

    private:
        node& _findMain(node& pak, const args& a);
        void _prepareFrame(frames& fr);
        str _postprocess(str res);

    private:
        str _pak;
        tstr<args> _args;
    };
}
