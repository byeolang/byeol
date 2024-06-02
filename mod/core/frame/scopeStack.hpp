#pragma once

#include "../ast/scopes.hpp"
#include "../builtin/container/native/tnchain.inl"

namespace namu {

    class _nout scopeStack : public instance {
        NAMU(CLASS(scopeStack, instance))

    public:
        tstr<scopes>& getHead();
        const tstr<scopes>& getHead() const NAMU_UNCONST_FUNC(getHead())
        tstr<scopes>& getTail();
        const tstr<scopes>& getTail() const NAMU_UNCONST_FUNC(getTail())

        ncnt len() const;
        ncnt chainLen() const;

        /// @param new1 this scope chain should be cloneChained if it's required.
        nbool addTail(scopes& new1);
        /// @param new1 this scope chain should be cloneChained if it's required.
        nbool addHead(scopes& new1);

        tstr<scopes> pop();

        void rel();

    private:
        tstr<scopes> _head;
        tstr<scopes> _tail;
    };
}
