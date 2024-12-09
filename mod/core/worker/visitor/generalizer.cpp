#include "generalizer.hpp"

#include "../../ast.hpp"
#include "../worker.inl"

namespace nm {

#define _VISIT if(alreadyVisited) return false

    NM(DEF_ME(generalizer))

    me& me::add(const obj& origin) {
        _org.bind(origin);
        return *this;
    }

    me& me::add(const param& newParam) {
        _params.add(newParam);
        _paramsKey = _makeParamsKey();
        return *this;
    }

    me& me::add(const params& newParams) {
        _params.add(newParams);
        _paramsKey = _makeParamsKey();
        return *this;
    }

    str me::_findOriginFrom(const getExpr& expr) const {
        const auto& name = expr.getName();
        for(const auto& p: _params)
            if(name == p.getName()) return p.getOrigin();
        return str();
    }

    str me::_findOriginFrom(const getGenericExpr& expr) const {
        const auto& name = getTask().getType().getName();
        auto argsKey = expr.getArgs().toStr();
        NM_I("exprName[%s<%s>] == originName[%s<%s>]", expr.getName(), argsKey, name, _paramsKey);
        if(expr.getName() != name) return str();
        if(argsKey == _paramsKey) return getTask();

        return str();
    }

    str me::_findOrigin(const node& toReplace) const {
        if(&toReplace == &_org.get()) return getTask();
        const getGenericExpr& generic = toReplace.cast<getGenericExpr>();
        if(!nul(generic)) return _findOriginFrom(generic);
        const getExpr& get = toReplace.cast<getExpr>();
        if(!nul(get)) return _findOriginFrom(get);

        return str();
    }

    nbool me::onVisit(const visitInfo& i, asExpr& me, nbool alreadyVisited) {
        _VISIT;
        str org = _findOrigin(me.getAs()) orRet true;

        NM_DI("* inject 'as %s' --> 'as %s'", me.getAs(), org);
        me.setAs(*org);
        return true;
    }

    nbool me::onVisit(const visitInfo& i, blockExpr& me, nbool alreadyVisited) {
        _VISIT;
        narr& stmts = me.getStmts();
        for(int n = 0; n < stmts.len(); n++) {
            const node& stmt = stmts[n];
            str org = _findOrigin(stmt) orContinue;

            NM_DI("* inject 'stmt %s' --> 'stmt %s'", stmt, org);
            stmts.set(n, *org);
        }
        return true;
    }

    nbool me::onVisit(const visitInfo& i, defVarExpr& me, nbool alreadyVisited) {
        _VISIT;
        str org = _findOrigin(me.getRight()) orRet true;

        NM_DI("* inject '%s %s' --> '%s %s'", me.getName(), me.getRight(), me.getName(), org);
        me.setRight(*org);
        return true;
    }

    nbool me::onVisit(const visitInfo& i, runExpr& me, nbool alreadyVisited) {
        _VISIT;
        str org = _findOrigin(me.getMe());
        if(org) me.setMe(*org);

        org = _findOrigin(me.getSubj());
        if(org) me.setSubj(*org);

        args& as = me.getArgs();
        for(int n = 0; n < as.len(); n++) {
            node& a = as[n];
            str org = _findOrigin(a) orContinue;

            NM_DI("* inject arg '%s' --> '%s'", a, *org);
            as.set(n, *org);
        }
        return true;
    }

    nbool me::onVisit(const visitInfo& i, params& me) {
        for(int n = 0; n < me.len(); n++) {
            param& p = me[n];
            str org = _findOrigin(p.getOrigin()) orContinue;

            NM_DI("* inject %s() func's param: '%s' --> '%s'", i, p.getOrigin(), *org);
            p.setOrigin(*org);
        }
        return true;
    }

    nbool me::onVisit(const visitInfo& i, baseCtor& me, nbool alreadyVisited) {
        _VISIT;
        baseObj& cast = getTask().cast<baseObj>();
        if(nul(cast)) getReport().add(nerr::newErr(errCode::MAKE_GENERIC_FAIL, i.name.c_str()));
        else if(i.parent && i.parent == &cast)
            // if this ctor belongs to root object(== generic obj):
            me._setOrigin(cast.getOrigin());

        onVisit(i, (baseFunc&) me, alreadyVisited);
        return true;
    }

    nbool me::onVisit(const visitInfo& i, baseFunc& me, nbool alreadyVisited) {
        _VISIT;
        onVisit(i, (params&) me.getParams());

        str retOrg = _findOrigin(me.getRet());
        if(retOrg) {
            NM_DI("* inject func: retType of '%s(%s) %s' --> '%s'", i, me.getParams().toStr(),
                me.getRet().getEval(), *retOrg);
            me._getType().setRet(*retOrg);
            if(nul(i.parent)) getReport().add(nerr::newErr(errCode::IS_NUL, "parent"));
        }

        onVisit(i, (baseFunc::super&) me, alreadyVisited);
        return true;
    }

    nbool me::onVisit(const visitInfo& i, ctor& me, nbool alreadyVisited) {
        _VISIT;
        baseObj& cast = getTask().cast<baseObj>();
        if(nul(cast)) getReport().add(nerr::newErr(errCode::MAKE_GENERIC_FAIL, i.name.c_str()));
        else if(i.parent && i.parent == &cast)
            // if this ctor belongs to root object(== generic obj):
            me._getType().setRet(cast);

        return super::onVisit(i, me, alreadyVisited);
    }

    nbool me::onVisit(const visitInfo& i, baseObj& me, nbool alreadyVisited) {
        _VISIT;
        scope& subs = me.subs();
        for(auto e = subs.begin(); e; ++e) {
            const node& prevVal = e.getVal();
            str org = _findOrigin(prevVal) orContinue;

            NM_DI("* inject '%s' at '%s.%s' to '%s", prevVal, i, e.getKey(), *org);
            e.setVal(*org);
        }

        onVisit(i, (baseObj::super&) me, alreadyVisited);
        return true;
    }

    nbool me::onVisit(const visitInfo& i, FBOExpr& me, nbool alreadyVisited) {
        _VISIT;
        str org = _findOrigin(me.getLeft());
        if(org) me.setLeft(*org);

        org = _findOrigin(me.getRight());
        if(org) me.setRight(*org);
        return true;
    }

    nbool me::onVisit(const visitInfo& i, getGenericExpr& me, nbool alreadyVisited) {
        _VISIT;
        args& a = *me._args orRet true;

        for(nint n = 0; n < a.len(); n++) {
            str org = _findOrigin(a[n]);
            if(org) a.set(n, *org);
        }
        return true;
    }

    std::string me::_makeParamsKey() const {
        nint n = 0;
        std::string msg;
        ncnt len = _params.len();
        for(const param& p: _params)
            msg += p.getName() + (++n >= len ? "" : ",");
        return msg;
    }
} // namespace nm
