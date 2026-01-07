/// @file
#pragma once

#include "core/worker/visitor/visitor.hpp"
#include "core/ast/params.hpp"
#include "core/frame/frame.hpp"

struct verifierTest;

namespace by {

    class loopExpr;
    class thread;
    class endExpr;

    /// @ingroup core
    /// @brief AST verifier for semantic analysis
    /// @details Handles code verification based on @ref visitor. Although the byeol language has an interpreter
    /// structure, the language itself uses strong typing and produces errors in advance as if executing compilation.
    /// verifier implements the core of that functionality. It checks for errors before executing the final AST created
    /// by @ref parser and @ref expander.
    ///
    /// @section understanding_difficulty Understanding Difficulty
    /// verifier is an advanced class even within the `core` module that uses various modules, so you need to know about
    /// several classes beforehand. It's probably one of the classes that will only be understood at the very last stage
    /// in this project. You need to know not only @ref node and @ref visitor but also @ref expr and WHEN macros to
    /// understand it easily.
    ///
    /// @section visitor_based Visitor Based
    /// Since onTraverse is filled in by visitor, verifier only defines what should be done during onVisit and onLeave.
    ///
    /// @section eval_and_infer eval and infer
    /// Program execution ultimately consists of each @ref node's eval(), but verifier is not interested in executed
    /// runtime values. It's only interested in `type matching` such as whether execution is possible and whether the
    /// result of a specified expression is implicitly convertible with the defined property's type. node's `infer()`
    /// performs type inference functionality, returning the result type as an origin object without knowing the value
    /// when executed. It's faster than eval() because it doesn't compute values. Now you might sense it, but verifier
    /// focuses on using infer() rather than eval().
    ///
    /// @code
    ///     void me::onLeave(const visitInfo& i, assignExpr& me, nbool) {
    ///         ....
    ///         str left = me.getLeft().infer() OR.myExErr(me, LHS_IS_NUL).ret();
    ///         ....
    ///         str right = me.getRight().infer() OR.myExErr(me, RHS_IS_NUL).ret();
    ///         ....
    ///     }
    /// @endcode
    ///
    /// As in the above example, infer() is mainly used instead of eval().
    ///
    /// @section many_lines_of_code Many Lines of Code File
    /// verifier is one of the files with the most LOC. At this time, the code is largely classified into 3 types.
    ///
    /// * Preliminary work for verification
    /// * Actual verification logic using WHEN macros
    /// * Detailed logging for each stage
    ///
    /// @section detailed_stage_logging Detailed Stage Logging
    /// visitor can only have 1 onVisit() function per type. However, verifier usually has 2 or more verification cases
    /// that need to be checked when given one type. So if not careful, one onVisit() function becomes mixed with
    /// multiple verification logics making it difficult to distinguish. Detailed stage logging helps with this, playing
    /// the role of leaving logs about what verification logic will be performed on the object being verified. For
    /// example, the _STEP macro automatically counts which stage within that function it is and shows the log.
    ///
    /// @section when_macro_verification Verification Using WHEN Macros
    /// verifier is a class with many verification codes like `if (!check) return res;`. So verifier inevitably has
    /// numerous WHEN macros.
    ///
    /// @section verifier_vs_starter verifier vs starter
    /// @ref starter directly executes the program. verifier only performs verification. The biggest difference between
    /// them is that when starter executes expressions with `eval()`, verifier only gets type information with
    /// `infer()`. However, there are some tasks that verifier must perform identically to starter.
    ///
    /// * First, using @ref frameInteract. To find out if a type exists and is currently accessible, the @ref obj @ref
    ///   scope or func scope must actually be registered in the frame. Usually if inFrame() is called for a specific
    ///   object in onVisit(), outFrame() will be called for the same object in onLeave().
    ///
    /// * Actually define properties. Although the value is empty, the property itself must be defined to check whether
    ///   the property is duplicate or if a non-existent property is being accessed.
    ///
    /// * Actually create generic classes. Only then can generic class verification be possible.
    class _nout verifier: public visitor {
        BY(CLASS(verifier, visitor))
        friend struct ::verifierTest;
        friend class genericOrigin;

    public:
        using super::onLeave;
        using super::onVisit;
        void onLeave(const visitInfo& i, node& me, nbool) override;
        void onLeave(const visitInfo& i, asExpr& me, nbool) override;
        void onLeave(const visitInfo& i, isExpr& me, nbool) override;
        void onLeave(const visitInfo& i, assignExpr& me, nbool) override;
        nbool onVisit(const visitInfo& i, blockExpr& me, nbool) override;
        void onLeave(const visitInfo& i, blockExpr& me, nbool) override;
        void onLeave(const visitInfo& i, defVarExpr& me, nbool) override;
        void onLeave(const visitInfo& i, defPropExpr& me, nbool) override;
        void onLeave(const visitInfo& i, defAssignExpr& me, nbool) override;
        void onLeave(const visitInfo& i, defSeqExpr& me, nbool) override;
        void onLeave(const visitInfo& i, defArrayExpr& me, nbool) override;
        void onLeave(const visitInfo& i, defNestedFuncExpr& me, nbool) override;
        void onLeave(const visitInfo& i, FBOExpr& me, nbool) override;
        void onLeave(const visitInfo& i, FUOExpr& me, nbool) override;
        void onLeave(const visitInfo& i, getExpr& me, nbool) override;
        void onLeave(const visitInfo& i, retExpr& me, nbool) override;
        void onLeave(const visitInfo& i, evalExpr& me, nbool) override;
        void onTraverse(evalExpr& me, node& subject) override;
        nbool onVisit(const visitInfo& i, func& me, nbool) override;
        void onLeave(const visitInfo& i, func& me, nbool) override;
        nbool onVisit(const visitInfo& i, closure& me, nbool) override;
        void onLeave(const visitInfo& i, closure& me, nbool) override;
        nbool onVisit(const visitInfo& i, ctor& me, nbool) override;
        void onLeave(const visitInfo& i, ctor& me, nbool) override;
        nbool onVisit(const visitInfo& i, baseObj& me, nbool) override;
        void onLeave(const visitInfo& i, baseObj& me, nbool) override;
        nbool onVisit(const visitInfo& i, genericOrigin& me, nbool) override;
        void onLeave(const visitInfo& i, genericOrigin& me, nbool) override;
        nbool onVisit(const visitInfo& i, forExpr& me, nbool) override;
        void onLeave(const visitInfo& i, forExpr& me, nbool) override;
        nbool onVisit(const visitInfo& i, whileExpr& me, nbool) override;
        void onLeave(const visitInfo& i, breakExpr& me, nbool) override;
        void onLeave(const visitInfo& i, nextExpr& me, nbool) override;
        nbool onVisit(const visitInfo& i, ifExpr& me, nbool) override;
        void onLeave(const visitInfo& i, ifExpr& me, nbool) override;

    protected:
        void _prepare() override;
        void _onEndWork() override;

    private:
        std::string _asStr(const params& ps);

        // @param newInfo is not a heap instance.
        void _verifyMgdFuncImplicitReturn(func& me);
        void _onLeave(const visitInfo& i, const loopExpr& me);

    private:
        std::vector<loopExpr*> _recentLoops;
        std::vector<baseObj*> _orgs;
    };
} // namespace by
