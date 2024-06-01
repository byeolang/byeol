#pragma once

#include "../ast/scopes.hpp"
#include "../builtin/container/native/tnchain.inl"

namespace namu {

    class _nout scopeStack : public instance {
        NAMU(CLASS(scopeStack, instance))

    public:
        tstr<scopes>& getTop();
        const tstr<scopes>& getTop() const NAMU_UNCONST_FUNC(getTop())
        tstr<scopes>& getTail();
        const tstr<scopes>& getTail() const NAMU_UNCONST_FUNC(getTail())

        ncnt len() const;
        ncnt chainLen() const;

        nbool push(scopes& new1);

        tstr<scopes> pop();

        void rel();

    private:
        tstr<scopes> _top;
        tstr<scopes> _tail;
    };
}
