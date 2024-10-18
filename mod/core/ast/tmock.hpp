#pragma once

#include "node.hpp"
#include "dumScope.hpp"

namespace nm {

    template <typename T> class _nout tmock: public T {
        NM(ME(tmock, T))
        friend class exprMaker;

    public:
        tmock(): super() {}

        tmock(const T& org): super(), _org(org) {}

    public:
        const ntype& getType() const override {
            if(_org) return _org->getType();
            return ttype<node>::get();
        }

        using super::subs;

        scope& subs() override {
            if(_org) return _org->subs();
            static dumScope inner;
            return inner;
        }

        priorType prioritize(const args& a) const override;

        str run(const args& a) override {
            if(_org) return _org->run(a);
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

        void* cast(const type& to) override {
            if(ttype<me>::get().isSub(to)) return this;
            if(_org) return _org->cast(to);
            return nullptr;
        }

        clonable* cloneDeep() const override { return clone(); }

        void onCloneDeep(const clonable& from) override {}

    protected:
        void _setSrc(const src& s) override {
            if(!_org) return;
            _org->_setSrc(s);
        }

    private:
        str _org;
    };

    typedef tmock<node> mockNode;
} // namespace nm
