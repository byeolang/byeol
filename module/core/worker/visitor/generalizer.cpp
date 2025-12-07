#include "core/worker/visitor/generalizer.hpp"
#include "core/ast.hpp"
#include "core/worker/tworker.inl"

namespace by {

    BY(DEF_ME(generalizer))

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
        const auto& name = getTask()->getType().getName();
        auto argsKey = expr.getArgs().toStr();
        BY_I("exprName[%s<%s>] == originName[%s<%s>]", expr.getName(), argsKey, name, _paramsKey);
        WHEN(expr.getName() != name) .ret(str());
        WHEN(argsKey == _paramsKey) .ret(getTask());

        return str();
    }

    str me::_findOrigin(const node& toReplace) const {
        WHEN(&toReplace == _org.get()) .ret(getTask());
        const getGenericExpr* generic = toReplace.cast<getGenericExpr>();
        WHEN(generic) .ret(_findOriginFrom(*generic));
        const getExpr* get = toReplace.cast<getExpr>();
        WHEN(get) .ret(_findOriginFrom(*get));

        return str();
    }

    nbool me::onVisit(const visitInfo& i, asExpr& me, nbool) {
        str org = _findOrigin(me.getAs()) OR.ret(true);

        BY_DI("* inject 'as %s' --> 'as %s'", me.getAs(), org);
        me.setAs(*org);
        return true;
    }

    nbool me::onVisit(const visitInfo& i, blockExpr& me, nbool) {
        narr& stmts = me.getStmts();
        for(int n = 0; n < stmts.len(); n++) {
            const node& stmt = stmts[n];
            str org = _findOrigin(stmt) OR_CONTINUE;

            BY_DI("* inject 'stmt %s' --> 'stmt %s'", stmt, org);
            stmts.set(n, *org);
        }
        return true;
    }

    nbool me::onVisit(const visitInfo& i, defVarExpr& me, nbool) {
        str org = _findOrigin(me.getRight()) OR.ret(true);

        BY_DI("* inject '%s %s' --> '%s %s'", me.getName(), me.getRight(), me.getName(), org);
        me.setRight(*org);
        return true;
    }

    nbool me::onVisit(const visitInfo& i, evalExpr& me, nbool) {
        str org = _findOrigin(me.getMe());
        if(org) me.setMe(*org);

        org = _findOrigin(me.getSubj());
        if(org) me.setSubj(*org);

        args& as = me.getArgs();
        for(int n = 0; n < as.len(); n++) {
            node& a = as[n];
            str org = _findOrigin(a) OR_CONTINUE;

            BY_DI("* inject arg '%s' --> '%s'", a, *org);
            as.set(n, *org);
        }
        return true;
    }

    nbool me::onVisit(const visitInfo& i, params& me) {
        for(int n = 0; n < me.len(); n++) {
            param& p = me[n];
            str org = _findOrigin(p.getOrigin()) OR_CONTINUE;

            BY_DI("* inject %s() func's param: '%s' --> '%s'", i, p.getOrigin(), *org);
            p.setOrigin(*org);
        }
        return true;
    }

    nbool me::onVisit(const visitInfo& i, baseCtor& me, nbool) {
        baseObj* cast = getTask() TO(template cast<baseObj>());
        if(!cast) getReport().add(nerr::newErr(errCode::MAKE_GENERIC_FAIL, i.name.c_str()));
        else if(i.parent && i.parent == cast)
            // if this ctor belongs to root object(== generic obj):
            me._setOrigin(cast->getOrigin());

        onVisit(i, (baseFunc&) me, false);
        return true;
    }

    nbool me::onVisit(const visitInfo& i, baseFunc& me, nbool) {
        onVisit(i, (params&) me.getParams());

        str retOrg = _findOrigin(me.getRet());
        if(retOrg) {
            BY_DI("* inject func: retType of '%s(%s) %s' --> '%s'", i, me.getParams().toStr(), me.getRet()->infer(),
                *retOrg);
            me._getType().setRet(*retOrg);
            if(!i.parent) getReport().add(nerr::newErr(errCode::IS_NUL, "parent"));
        }

        onVisit(i, (baseFunc::super&) me, false);
        return true;
    }

    nbool me::onVisit(const visitInfo& i, ctor& me, nbool) {
        baseObj* cast = getTask() TO(template cast<baseObj>());
        if(!cast) getReport().add(nerr::newErr(errCode::MAKE_GENERIC_FAIL, i.name.c_str()));
        else if(i.parent && i.parent == cast)
            // if this ctor belongs to root object(== generic obj):
            me._getType().setRet(cast);

        return super::onVisit(i, me, false);
    }

    nbool me::onVisit(const visitInfo& i, baseObj& me, nbool) {
        scope& subs = me.subs();
        for(auto e = subs.begin(); e; ++e) {
            const node* prevVal = e.getVal();
            str org = _findOrigin(prevVal) OR_CONTINUE;

            BY_DI("* inject '%s' at '%s.%s' to '%s", prevVal, i, e.getKey(), *org);
            e.setVal(*org);
        }

        onVisit(i, (baseObj::super&) me, false);
        return true;
    }

    nbool me::onVisit(const visitInfo& i, FBOExpr& me, nbool) {
        str org = _findOrigin(me.getLeft());
        if(org) me.setLeft(*org);

        org = _findOrigin(me.getRight());
        if(org) me.setRight(*org);
        return true;
    }

    nbool me::onVisit(const visitInfo& i, getGenericExpr& me, nbool) {
        args& a = me._args OR.ret(true);

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
} // namespace by
