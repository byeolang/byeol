#pragma once

#include "expr.hpp"

namespace namu {

    class param;
    template <typename T, typename TACTIC> class tnarr;
    class params;
    class visitor;

    class _nout baseFunc : public expr {
        NAMU(ADT(baseFunc, expr), VISIT())

    public:
        static inline const std::string ME = "me";

    public:
        priorType prioritize(const args& a) const override;

        /// @return parameters of run() func.
        ///         parameter is just a type. and I don't care about the value of each parameters.
        ///         that is the reason why I uses a ref to represents parameter.
        ///
        ///         I need the name and which types should be casted and binded from given arguments
        ///         are matters.
        virtual const params& getParams() const;

        /// @return node which has same type of the func.
        ///         in namu lang, even type is an object. that's why I return this as a node.
        /// @remark please don't confuse this to evalType.
        ///         evaluate means program just read the code and expand or calculate then replace
        ///         it to the accurate another identifier or variable. so evalType of the func is
        ///         just the func itself because there is nothing to calculate or run.
        ///         that's completely different to Return type of the function.
        ///         if there is a runExpr instance, and it takes the func and proper argument,
        ///         then it can be evaluated and its evalType is the return type of the func.
        virtual str getRet() const = 0;

        /// @return whether setting new return-type has been done or not.
        virtual nbool setRet(const node& newRet);

    private:
        nbool _isNatureNumber(const node& it) const;
        priorType _prioritize(const node& param, const node& arg) const;
    };

    typedef tnarr<baseFunc> funcs;
}
