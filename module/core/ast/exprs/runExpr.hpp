/// @file
#pragma once

#include "core/ast/expr.hpp"

namespace by {

    class visitor;

    /// @ingroup core
    /// @brief Function/method call expression
    /// @details Represents function and method invocation with arguments.
    /// Handles subject evaluation, argument passing, and function execution.
    class _nout runExpr: public expr {
        BY(CLASS(runExpr, expr, expr::exprType), VISIT())
        friend class parser;

    public:
        runExpr(const node* meObj, const node& subject, const args& a);
        runExpr(const node* meObj, const args& a);

    public:
        using super::run;
        str run(const args& a) override;

        str getEval() const override;

        node* getMe();
        const node* getMe() const BY_CONST_FUNC(getMe())

        void setMe(const node& newMe);
        void setMe(const node* it) BY_SIDE_FUNC(setMe);

        node& getSubj();
        const node& getSubj() const;

        void setSubj(const node& new1);
        void setSubj(const node* it) BY_SIDE_FUNC(setSubj);

        args& getArgs();
        const args& getArgs() const;

        void onCloneDeep(const clonable& from) override;

    private:
        str _getSub(str me) const;

    private:
        str _me;
        args _args;
        mutable str _subject;
    };
} // namespace by
