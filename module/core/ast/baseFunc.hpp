#pragma once

#include "core/ast/node.hpp"
#include "core/type/mgdType.hpp"

namespace nm {

    class param;
    template <typename T, typename TACTIC> class tnarr;
    class params;
    class visitor;

    class _nout baseFunc: public node {
        NM(ADT(baseFunc, node), VISIT())
        friend class generalizer; // for _getType()
        friend class parser;      // for _getType()
        friend class exprMaker;   // for _setSrc()

    public:
        baseFunc() = default;

        baseFunc(const modifier& mod);

    public:
        using super::subs;
        scope& subs() override;

        using super::prioritize;
        priorType prioritize(const args& a) const override;

        /// @return parameters of run() func.
        ///         parameter is just a type. and I don't care about the value of each parameters.
        ///         that is the reason why I uses a ref to represents parameter.
        ///
        ///         I need the name and which types should be casted and binded from given arguments
        ///         are matters.
        virtual params& getParams();
        const params& getParams() const NM_CONST_FUNC(getParams())

        /// @return node which has same type of the func.
        ///         in byeol lang, even type is an object. that's why I return this as a node.
        /// @remark please don't confuse this to evalType.
        ///         evaluate means program just read the code and expand or calculate then replace
        ///         it to the accurate another identifier or variable. so evalType of the func is
        ///         just the func itself because there is nothing to calculate or run.
        ///         that's completely different to Return type of the function.
        ///         if there is a runExpr instance, and it takes the func and proper argument,
        ///         then it can be evaluated and its evalType is the return type of the func.
        virtual const node* getRet() const;

        const src& getSrc() const override;

        const modifier& getModifier() const override;

        virtual const baseObj& getOrigin() const = 0;

        static nbool isFuncButNotClosure(const node& n);
        static nbool isFuncButNotClosure(const node* it) NM_SIDE_FUNC(isFuncButNotClosure);

    protected:
        ntype& _getType(); // for generalizer.
        void _setSrc(const src& newSrc) override;

    private:
        nbool _isNatureNumber(const node& it) const;
        priorType _prioritize(const node& param, const node& arg) const;

    private:
        tstr<modifier> _mod;
        tstr<src> _src;

    public:
        static inline const std::string ME = "me";
    };

    typedef tnarr<baseFunc> funcs;
} // namespace nm
