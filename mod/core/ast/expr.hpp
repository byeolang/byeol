#pragma once

#include "args.hpp"
#include "point.hpp"
#include "src/src.hpp"

struct exprTest;

namespace nm {

    class _nout expr: public node {
    public:
        // expr can be casted to node. it's proxy of a node.
        class _nout exprType: public ntype {
            NM(ME(exprType, ntype))

        public:
            using super::asImpli;
            nbool isImpli(const type& to) const override;
            str asImpli(const node& from, const type& to) const override;
        };

        NM(ADT(expr, node, exprType))
        friend class exprMaker;
        friend struct ::exprTest;

    public:
        using super::subs;
        scope& subs() override;

        nbool isImpli(const type& to) const override;
        str asImpli(const type& to) const override;

        /// run of expr class get treated like 'evaluate' in namu.
        /// it should not take any argument to run()
        using super::prioritize;
        priorType prioritize(const args& a) const override;
        const src& getSrc() const override;

    private:
        void _setSrc(const src& newSrc) override;

    protected:
        tstr<src> _src;
        point _pos;
    };
} // namespace nm
