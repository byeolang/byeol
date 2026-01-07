/// @file
#pragma once

#include "core/ast/origin.hpp"
#include "core/worker/tworker.inl"
#include "core/frame/defaultSigZone.hpp"

namespace by {
    class frames;

    /// @ingroup core
    /// @brief Program execution starter
    /// @details A kind of @ref worker that executes AST verified by @ref verifier. If an exception occurs in @ref
    /// thread during execution, it dumps logs and returns the exception. See @ref worker for more details if
    /// unfamiliar with the worker concept.
    ///
    /// @section main_function_lookup Main Function Look Up
    /// The main function must meet the following conditions:
    ///
    /// 1. Found directly under root (pack)
    /// 2. Name must be `main`
    /// 3. Return type must be void or int
    /// 4. Main function must have 1 or more statements
    ///
    /// @section usage_example Usage Example
    /// Can be used with @ref interpreter to parse and execute byeol code.
    ///
    /// @code
    ///     interpreter ip;
    ///     ip.work();
    ///     if(!ip.isVerified()) return -1;
    ///
    ///     str res = starter().setTask(ip.getSubPack()).work();
    ///     if(res)
    ///         if(res->isSub<nInt>()) return *res->cast<nInt>();
    ///     return -1;
    /// @endcode
    class _nout starter: public tworker<str, node> {
        typedef tworker<str, node> __super8;
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
