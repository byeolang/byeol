/// @file
#pragma once

#include "core/ast/origin.hpp"
#include "core/worker/worker.inl"
#include "core/frame/defaultSigZone.hpp"

namespace by {
    class frames;

    /// @ingroup core
    /// @brief Program execution starter
    /// @details Worker that initiates and manages byeol program execution with main function lookup.
    class _nout starter: public worker<str, node> {
        typedef worker<str, node> __super8;
        BY(CLASS(starter, __super8))

    public:
        enum logFlag3 {
            LOG_STRUCTURE = 1 << 7,
            LOG_GRAPH_ON_EX = 1 << 8
        };

    public:
        static inline const std::string MAIN = "main";

    public:
        void setArgs(const args& a);
        void setArgs(const args* it) BY_SIDE_FUNC(setArgs);
        args& getArgs();
        const args& getArgs() const BY_CONST_FUNC(getArgs())

    protected:
        str _onWork() override;
        void _prepare() override;

    private:
        node* _findMain(node& pak, const args& a);
        void _prepareFrame(frames& fr);
        str _postprocess(const str& res);

    private:
        str _pak;
        tstr<args> _args;
    };
} // namespace by
