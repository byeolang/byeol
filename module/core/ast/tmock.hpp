/// @file
#pragma once

#include "core/ast/node.hpp"
#include "core/ast/dumScope.hpp"

namespace by {

    /** @ingroup core
     *  @brief Proxy template for type parameter T
     *  @details A kind of proxy class for given type parameter T. If you don't provide an instance to serve as the
     *  original via T* at creation, it operates as a dummy object that performs no actions.
     *
     *  Originally designed to optimize verification time. For example, for @ref verifier to verify whether a specific
     *  symbol exists in scope or not, it could create a tmock that redirects just type information instead of actually
     *  making that object. Currently that functionality is unused and it's used as a dummy object instead.
     */
    template <typename T> class _nout tmock: public T {
        BY(ME(tmock, T))
        friend class exprMaker;

    public:
        tmock(): super() {}

        tmock(const T& org): super(), _org(org) {}

        tmock(const T* org): super(), _org(org) {}

    public:
        const ntype& getType() const override {
            if(_org) return _org->getType();
            return ttype<node>::get();
        }

        using super::subs;

        scope& subs() override {
            if(_org) return _org->subs();
            return dumScope::singleton();
        }

        using super::prioritize;

        priorType prioritize(const args& a) const override {
            if(_org) return _org->prioritize(a);
            return NO_MATCH;
        }

        using super::eval;

        str eval(const args& a) override {
            if(_org) return _org->eval(a);
            return str();
        }

        clonable* clone() const override {
            if(_org) return _org->clone();
            return new me(*this);
        }

        const src& getSrc() const override {
            if(_org) return _org->getSrc();
            return super::getSrc();
        }

        using super::cast;

        void* cast(const type& to) override {
            if(ttype<me>::get().isSub(to)) return this;
            if(_org) return _org->cast(to);
            return nullptr;
        }

        clonable* cloneDeep() const override { return clone(); }

        void onCloneDeep(const clonable& from) override {}

    protected:
        using super::_setSrc;

        void _setSrc(const src& s) override {
            if(!_org) return;
            _org->_setSrc(s);
        }

    private:
        str _org;
    };

    typedef tmock<node> mockNode;
} // namespace by
