/// @file
#pragma once

#include "core/worker/visitor/verifier.hpp"
#include "core/worker/parser.hpp"

namespace by {

    /// @ingroup core
    /// @brief High-level interpreter for byeol language
    /// @details Coordinates the complete interpretation pipeline combining @ref parser, @ref expander, and @ref
    /// verifier. Manages the entire process from source code to executable @ref slot instances. This is the main entry
    /// point for interpreting byeol source code.
    ///
    /// @section interpretation_pipeline Interpretation Pipeline
    /// The interpreter executes the following stages in order:
    /// 1. **Parsing** - Converts source code to AST using @ref parser
    /// 2. **Expansion** - Performs prior type inference using @ref expander
    /// 3. **Verification** - Validates semantics and type safety using @ref verifier
    ///
    /// Each stage must complete successfully before the next stage begins. If any stage fails, errors are collected in
    /// the @ref errReport and the process stops.
    ///
    /// @section usage_with_starter Usage with starter
    /// After interpretation completes successfully, use @ref starter to actually execute the verified AST:
    ///
    /// @code
    ///     interpreter ip;
    ///     ip.work();
    ///     if(!ip.isVerified()) return -1;
    ///
    ///     str res = starter().setTask(ip.getSubPack()).work();
    /// @endcode
    class _nout interpreter: public tworker<tstr<slot>, slot> {
        typedef tworker<tstr<slot>, slot> __super7;
        BY(CLASS(interpreter, __super7))

    public:
        enum logFlag2 {
            LOG_STRUCTURE = 1 << 7,
            LOG_GRAPH_ON_EX = 1 << 8
        };

    public:
        interpreter();

    public:
        parser& getParser();
        const parser& getParser() const BY_CONST_FUNC(getParser())
        verifier& getVerifier();
        const verifier& getVerifier() const BY_CONST_FUNC(getVerifier())
        nbool isParsed() const;
        nbool isVerified() const;
        node* getSubPack();
        const node* getSubPack() const BY_CONST_FUNC(getSubPack())

        void rel() override;

    protected:
        tstr<slot> _onWork() override;

    private:
        nbool _isPackExist();

        void _parse();
        void _expand();
        void _verify();
        void _log() const;
        void _showGraph(nbool showData) const;

    private:
        verifier _veri;
        parser _pser;
        nbool _isParsed;
    };
} // namespace by
