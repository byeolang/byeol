#include "generalizer.hpp"
#include "../ast.hpp"
#include "../loader/worker/worker.inl"

namespace namu {

    NAMU(DEF_ME(generalizer))

    me& me::add(const param& newParam) {
        _params.add(newParam);
        return *this;
    }

    const node& me::_findOrigin(const node& toReplace) const {
        const getGenericExpr& generic = toReplace.cast<getGenericExpr>();
        if(!nul(generic)) return nulOf<node>();
        const getExpr& cast = toReplace.cast<getExpr>();
        if(nul(cast)) return nulOf<node>();

        for(param p : _params)
            if(cast.getSubName() == p.getName())
                return p.getOrigin();
        return nulOf<node>();
    }

    nbool me::onVisit(visitInfo i, asExpr& me) {
        const node& org = _findOrigin(me.getAs());
        if(nul(org)) return true;

        me.setAs(org);
        return true;
    }

    nbool me::onVisit(visitInfo i, blockExpr& me) {
        narr& stmts = me.getStmts();
        for(int n=0; n < stmts.len() ;n++) {
            const node& org = _findOrigin(stmts[n]);
            if(nul(org)) continue;

            stmts.set(n, org);
        }
        return true;
    }

    nbool me::onVisit(visitInfo i, defPropExpr& me) {
        const node& org = _findOrigin(me.getOrigin());
        if(nul(org)) return true;

        me.setOrigin(org);
        return true;
    }

    nbool me::onVisit(visitInfo i, runExpr& me) {
        const node* org = &_findOrigin(me.getMe());
        if(!nul(org))
            me.setMe(*org);

        org = &_findOrigin(me.getSubject());
        if(!nul(org))
            me.setSubject(*org);

        args& a = me.getArgs();
        for(int n=0; n < a.len(); n++) {
            const node& org = _findOrigin(a[n]);
            if(nul(org)) continue;

            a.set(n, org);
        }
        return true;
    }

    nbool me::onVisit(visitInfo i, params& me) {
        NAMU_DI("generic: params[%d]", me.len());

        for(int n=0; n < me.len(); n++) {
            param& p = me[n];
            const node& org = _findOrigin(p.getOrigin());
            if(nul(org)) continue;

            p.setOrigin(org);
        }
        return true;
    }

    nbool me::onVisit(visitInfo i, ctor& me) {
        baseObj& cast = getTask().cast<baseObj>();
        if(nul(cast))
            getReport().add(err::newErr(errCode::MAKE_GENERIC_FAIL, i.name.c_str()));
        else {
            if(i.parent && i.parent == &cast)
                // if this ctor belongs to root object(== generic obj):
                me._setOrigin(cast.getOrigin());
        }

        onVisit(i, (baseFunc&) me);
        return true;
    }

    nbool me::onVisit(visitInfo i, baseFunc& me) {
        NAMU_DI("generic: func[%s, %x]", i.name.c_str(), &me);

        onVisit(i, (params&) me.getParams());

        const node& retOrg = _findOrigin(*me.getRet());
        if(!nul(retOrg)) {
            me.setRet(retOrg);
            if(nul(i.parent))
                getReport().add(err::newErr(errCode::IS_NULL, "parent"));
        }

        onVisit(i, (baseFunc::super&) me);
        return true;
    }

    nbool me::onVisit(visitInfo i, baseObj& me) {
        nbicontainer& subs = me.subs();
        for(auto e=subs.begin(); e ;++e) {
            const node& org = _findOrigin(e.getVal());
            if(nul(org)) continue;

            e.setVal(org);
        }

        onVisit(i, (baseObj::super&) me);
        return true;
    }

    nbool me::onVisit(visitInfo i, FBOExpr& me) {
        const node* org = &_findOrigin(me.getLeft());
        if(!nul(org))
            me.setLeft(*org);

        org = &_findOrigin(me.getRight());
        if(!nul(org))
            me.setRight(*org);
        return true;
    }

    nbool me::onVisit(visitInfo i, getGenericExpr& me) {
        args& a = *me._args;
        if(nul(a)) return true;

        for(nint n=0; n < a.len(); n++) {
            const node& org = _findOrigin(a[n]);
            if(!nul(org))
                a.set(n, org);
        }
        return true;
    }
}
