/// @file
#pragma once

#include "core/worker/visitor/verifier.hpp"
#include "core/worker/parser.hpp"

namespace by {

    /// @ingroup core
    /// @brief High-level interpreter for byeol language
    /// @details Coordinates parsing, expansion, and verification phases of interpretation.
    /// Manages the complete interpretation pipeline from source to executable slots.
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
        me& addSupply(const srcSupply& supply);
        me& relSupplies();
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
