#include "generalizer.hpp"

#include "../../ast.hpp"
#include "../worker.inl"

namespace nm {

    NM(DEF_ME(generalizer))

    me& me::add(const param& newParam) {
        _params.add(newParam);
        return *this;
    }

    me& me::add(const params& newParams) {
        _params.add(newParams);
        return *this;
    }

    const node& me::_findOrigin(const node& toReplace) const {
        if(!nul(toReplace.cast<getGenericExpr>())) return nulOf<node>();
        const auto& name = toReplace.cast<getExpr>() THEN(getName());
        if(nul(name)) return nulOf<node>();

        for(const auto& p: _params)
            if(name == p.getName()) return p.getOrigin();
        return nulOf<node>();
    }

    nbool me::onVisit(const visitInfo& i, asExpr& me) {
        const node& org = _findOrigin(me.getAs()) orRet1 true;

        NM_DI("* inject 'as %s' --> 'as %s'", me.getAs(), org);
        me.setAs(org);
        return true;
    }

    nbool me::onVisit(const visitInfo& i, blockExpr& me) {
        narr& stmts = me.getStmts();
        for(int n = 0; n < stmts.len(); n++) {
            const node& stmt = stmts[n];
            const node& org = getOr(_findOrigin(stmt)) orDo continue;

            NM_DI("* inject 'stmt %s' --> 'stmt %s'", stmt, org);
            stmts.set(n, org);
        }
        return true;
    }

    nbool me::onVisit(const visitInfo& i, defVarExpr& me) {
        const node& org = _findOrigin(me.getRight()) orRet1 true;

        NM_DI("* inject '%s %s' --> '%s %s'", me.getName(), me.getRight(), me.getName(), org);
        me.setRight(org);
        return true;
    }

    nbool me::onVisit(const visitInfo& i, runExpr& me) {
        const node* org = &_findOrigin(me.getMe());
        if(!nul(org)) me.setMe(*org);

        org = &_findOrigin(me.getSubj());
        if(!nul(org)) me.setSubj(*org);

        args& as = me.getArgs();
        for(int n = 0; n < as.len(); n++) {
            node& a = as[n];
            const node& org = getOr(_findOrigin(a)) orDo continue;

            NM_DI("* inject arg '%s' --> '%s'", a, org);
            as.set(n, org);
        }
        return true;
    }

    nbool me::onVisit(const visitInfo& i, params& me) {
        for(int n = 0; n < me.len(); n++) {
            param& p = me[n];
            const node& org = getOr(_findOrigin(p.getOrigin())) orDo continue;

            NM_DI("* inject %s() func's param: '%s' --> '%s'", i, p.getOrigin(), org);
            p.setOrigin(org);
        }
        return true;
    }

    nbool me::onVisit(const visitInfo& i, baseCtor& me) {
        baseObj& cast = getTask().cast<baseObj>();
        if(nul(cast)) getReport().add(nerr::newErr(errCode::MAKE_GENERIC_FAIL, i.name.c_str()));
        else if(i.parent && i.parent == &cast)
            // if this ctor belongs to root object(== generic obj):
            me._setOrigin(cast.getOrigin());

        onVisit(i, (baseFunc&) me);
        return true;
    }

    nbool me::onVisit(const visitInfo& i, baseFunc& me) {
        onVisit(i, (params&) me.getParams());

        const node& retOrg = _findOrigin(me.getRet());
        if(!nul(retOrg)) {
            NM_DI("* inject func: retType of '%s(%s) %s' --> '%s'", i, me.getParams().toStr(),
                me.getRet().getEval(), retOrg);
            me._getType().setRet(retOrg);
            if(nul(i.parent)) getReport().add(nerr::newErr(errCode::IS_NUL, "parent"));
        }

        onVisit(i, (baseFunc::super&) me);
        return true;
    }

    nbool me::onVisit(const visitInfo& i, ctor& me) {
        baseObj& cast = getTask().cast<baseObj>();
        if(nul(cast)) getReport().add(nerr::newErr(errCode::MAKE_GENERIC_FAIL, i.name.c_str()));
        else if(i.parent && i.parent == &cast)
            // if this ctor belongs to root object(== generic obj):
            me._getType().setRet(cast);

        return super::onVisit(i, me);
    }

    nbool me::onVisit(const visitInfo& i, baseObj& me) {
        scope& subs = me.subs();
        for(auto e = subs.begin(); e; ++e) {
            const node& prevVal = e.getVal();
            const node& org = getOr(_findOrigin(prevVal)) orDo continue;

            NM_DI("* inject '%s' at '%s.%s' to '%s", prevVal, i, e.getKey(), org);
            e.setVal(org);
        }

        onVisit(i, (baseObj::super&) me);
        return true;
    }

    nbool me::onVisit(const visitInfo& i, FBOExpr& me) {
        const node* org = &_findOrigin(me.getLeft());
        if(!nul(org)) me.setLeft(*org);

        org = &_findOrigin(me.getRight());
        if(!nul(org)) me.setRight(*org);
        return true;
    }

    nbool me::onVisit(const visitInfo& i, getGenericExpr& me) {
        args& a = *me._args orRet1 true;

        for(nint n = 0; n < a.len(); n++) {
            const node& org = _findOrigin(a[n]);
            if(!nul(org)) a.set(n, org);
        }
        return true;
    }
} // namespace nm
