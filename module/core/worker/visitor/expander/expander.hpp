/// @file
#pragma once

#include "core/worker/visitor/visitor.hpp"
#include "core/worker/visitor/expander/convergence/baseConvergence.hpp"

namespace by {

    /** @ingroup core
     *  @brief Visitor for expanding generic types and expressions
     *  @details As mentioned in @ref parser, the current prior type inference algorithm doesn't create dependency
     *  graphs but collects all prior type inference expressions and repeatedly attempts type inference. Since this
     *  approach is used, it's not suitable to perform the operation of repeatedly traversing expressions and
     *  attempting prior type inference all at once inside @ref verifier. Therefore, it's necessary to complete prior
     *  type inference with expander before verifying with verifier.
     *
     *  Basic operation is based on @ref visitor, so it's good to understand visitor beforehand.
     *
     *  @section expressions_needing_type_inference Definition of Expressions Needing Type Inference
     *  Basically, this applies to all expressions where the user did not specify a type. However, not all expressions
     *  without type specification are handled. Prior type inference is only needed when there is no type and the symbol
     *  exists in an obj @ref scope. Consider the following byeol code example:
     *
     *  @code
     *  def myObj
     *      age := 57 # 1) type inference
     *      foo() int
     *          name := "kniz" # 2) also type inference
     *          return name.len()
     *  @endcode
     *
     *  Both age in 1 and name in 2 need type inference, but only 1 performs prior type inference. This is because 2
     *  has no influence. That is, since name inside the foo() function is a local variable, it's sufficient for
     *  verifier to perform type inference inside the foo() function.
     *
     *  @section prior_type_inference_algorithm Prior Type Inference Algorithm
     *  When parser encounters an expression (property or function) that needs prior type inference, it puts the
     *  expression into a function called `@expand` of the object it belongs to. expander traverses all given root
     *  origin obj through visitor, checking if the `@expand` function exists. When it exists, it creates context to use
     *  during expansion and adds it to `_stack`.
     *
     *  After all visitation ends, it performs prior type inference through the following process:
     *  1. If `_stack` is empty, terminate type inference
     *  2. For all `_stack` elements, prepare @ref frame and verify the expression with @ref verifier
     *  3. If verification succeeds, complete type inference through `eval()`
     *  4. If it fails, move to the next element
     *  5. When type inference is attempted once for all elements, if even 1 succeeded, return to step 1
     *
     *  @section type_convergence Type Convergence
     *  For optimization purposes, during visitation for prior type inference, it performs the operation of replacing
     *  @ref getExpr with @ref origin objects, which is called type convergence. During convergence, sometimes
     *  replacement to origin is immediately possible, but sometimes replacement is only possible after prior type
     *  inference is completed. If getExpr is not replaced with origin during visitation, it's stored in `_cons` and
     *  replacement is attempted once after type inference is completed. Even in this case, if it fails, it's just
     *  ignored and no error occurs.
     */
    class _nout expander: public visitor {
        BY(CLASS(expander, visitor))

        /** @ingroup core
         *  @brief Expansion state tracking structure
         *  @details Tracks expansion state for objects and functions during type expansion.
         */
        struct _nout expansion {
            nbool isExpanded() const;

        public:
            tweak<obj> me;
            tweak<func> fun;
        };

    public:
        expander();

    public:
        void rel() override;

        using super::onVisit;

        nbool onVisit(const visitInfo& i, defAssignExpr& me, nbool) override;
        nbool onVisit(const visitInfo& i, asExpr& me, nbool) override;
        nbool onVisit(const visitInfo& i, obj& me, nbool) override;
        void onLeave(const visitInfo& i, obj& me, nbool) override;
        nbool onVisit(const visitInfo& i, func& me, nbool) override;
        void onLeave(const visitInfo& i, func& me, nbool) override;
        nbool onVisit(const visitInfo& i, baseFunc& me, nbool) override;
        void onLeave(const visitInfo& i, baseFunc& me, nbool) override;
        nbool onVisit(const visitInfo& i, getGenericExpr& me, nbool) override;

    protected:
        void _onWork() override;

    private:
        nbool _onVisitParams(baseFunc& f, param& p);
        void _onVisitFuncRet(baseFunc& f);

        void _convergeTypes(errReport& rpt);

        void _rel();

        /**
         *  @return true if there is a change.
         */
        nbool _expandAll(errReport& rpt);

        /**
         *  @return true if there is a change.
         */
        nbool _expand(errReport& rpt, expansion& eval);
        void _delEval(const std::map<obj*, expansion>::iterator& e);
        void _expand();
        nbool _isAllExpanded(obj& key) const;

    private:
        std::map<obj*, expansion> _stack;
        tnarr<baseConvergence> _cons;
        std::vector<obj*> _obj;
        std::vector<baseFunc*> _funcs;
    };
} // namespace by
