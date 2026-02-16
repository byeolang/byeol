/// @file
#pragma once

#include "core/ast/node.hpp"
#include "core/type/mgdType.hpp"

namespace by {

    class param;
    template <typename T, typename TACTIC> class tnarr;
    class params;
    class visitor;

    /** @ingroup core
     *  @brief Base class for all function types
     *  @details Abstract base class providing common functionality for all function types including @ref func, @ref
     *  closure, and bridged functions like @ref tbridgeFunc. Manages parameters, return types, @ref modifier, and
     *  function execution semantics.
     *
     *  @section function_hierarchy Function Hierarchy
     *  The function class hierarchy in byeol:
     *  - **baseFunc** - Abstract base for all functions
     *    - **func** - User-defined functions with @ref blockExpr bodies
     *    - **closure** - Nested functions with captured scope
     *    - **tbridgeFunc** - C++ native functions exposed to byeol
     *    - **ctor** - Constructor functions
     *
     *  @section parameters_and_return Parameters and Return
     *  baseFunc manages:
     *  - **Parameters** via `getParams()` - function parameter list with names and types
     *  - **Return type** via `getRet()` - the type this function returns when called
     *  - **Modifiers** via `getModifier()` - access control (public/protected/override)
     *
     *  Note: `getRet()` returns the return type, not the function's own type. The function's type is obtained via
     *  `getType()` or `infer()`.
     *
     *  @section prioritization Argument Prioritization
     *  baseFunc implements sophisticated `prioritize()` logic to select the best matching function overload based on
     *  argument types. This enables function overloading in byeol.
     */
    class _nout baseFunc: public node {
        BY(ME(baseFunc, node), VISIT())

    public:
        baseFunc() = default;

        baseFunc(const modifier& mod);

    public:
        using super::subs;
        scope& subs() override;

        using super::prioritize;
        priorType prioritize(const args& a) const override;

        /**
         *  @return parameters of eval() @ref func.
         *          parameter is just a type. and I don't care about the value of each parameters.
         *          that is the reason why I uses a ref to represents parameter.
         *
         *          I need the name and which types should be casted and binded from given arguments
         *          are matters.
         */
        virtual params& getParams();
        const params& getParams() const BY_CONST_FUNC(getParams());

        /**
         *  @return node which has same type of the func.
         *          in byeol lang, even type is an object. that's why I return this as a node.
         *  @remark please don't confuse this to @ref node::infer().
         *          evaluate means program just read the code and expand or calculate then replace
         *          it to the accurate another identifier or variable. so evalType of the func is
         *          just the func itself because there is nothing to calculate or eval.
         *          that's completely different to Return type of the function.
         *          if there is a evalExpr instance, and it takes the func and proper argument,
         *          then it can be evaluated and its evalType is the return type of the func.
         */
        virtual const node* getRet() const;

        const src& getSrc() const override;

        const modifier& getModifier() const override;

        virtual const baseObj& getOrigin() const = 0;

        /**
         * @brief Checks if the given node represents a function that is not a closure.
         * @param n The node to check.
         * @return true if the node is a function but not a closure, false otherwise.
         */
        static nbool isFuncButNotClosure(const node& n);
        static nbool isFuncButNotClosure(const node* it) BY_SIDE_FUNC(isFuncButNotClosure);

    protected:
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
} // namespace by
