#include "core/worker/visitor/visitor.hpp"
#include "core/ast.hpp"
#include "core/builtin.hpp"
#include "core/frame/frame.hpp"
#include "core/worker/worker.inl"

namespace by {

    template class _nout worker<void, node>;

    NM_DEF_ME(visitor)

    me::visitor(): me(false) {}

    me::visitor(nbool isReturnable): _isReturnable(isReturnable) {}

    nbool me::isReturnable() const { return _isReturnable; }

    void me::setReturnable(nbool isReturnable) { _isReturnable = isReturnable; }

#define X(T)                                                                     \
    void me::visit(const visitInfo& i, T& me) {                                  \
        nbool alreadyVisited = !_markVisited(me);                                \
        WHEN(alreadyVisited && !_isReturnable).ret();                            \
        if(onVisit(i, me, alreadyVisited) && !alreadyVisited) onTraverse(i, me); \
        onLeave(i, me, alreadyVisited);                                          \
    }                                                                            \
                                                                                 \
    nbool me::onVisit(const visitInfo& i, T& me, nbool alreadyVisited) {         \
        return onVisit(i, (T::super&) me, alreadyVisited);                       \
    }                                                                            \
                                                                                 \
    void me::onLeave(const visitInfo& i, T& me, nbool alreadyVisited) {          \
        onLeave(i, (T::super&) me, alreadyVisited);                              \
    }

#include "visitee.inl"
#undef X

    void me::_prepare() {
        super::_prepare();
        _visited.clear();
    }

    void me::visit(const visitInfo& i, node& me) {
        nbool alreadyVisited = !_markVisited(me);
        WHEN(alreadyVisited && !_isReturnable).ret();
        if(onVisit(i, me, alreadyVisited) && !alreadyVisited) onTraverse(i, me);
        onLeave(i, me, alreadyVisited);
    }

    nbool me::onVisit(const visitInfo& i, node& me, nbool alreadyVisited) { return true; }

    void me::onLeave(const visitInfo& i, node& me, nbool alreadyVisited) {}

    void me::_onWork() {
        WHEN_NUL(getTask()).ret();

        _visited.clear();
        getTask()->accept(visitInfo("#root", nullptr, 0, 1, 1), *this);
    }

    nbool me::_markVisited(node& me) {
        WHEN(_visited.find(&me) != _visited.end()).ret(false);

        return _visited[&me] = true;
    }

    void me::onTraverse(const visitInfo& i, node& me) {
        scope& subs = me.subs();
        ncnt len = subs.getContainer().len();
        const auto* next = subs.getNext();
        if(next) len += next->getContainer().len();
        WHEN(len <= 0).ret();

        auto e = subs.begin();
        for(int n = 0; n < len; n++, ++e) {
            node& val = e.getVal() OR_CONTINUE;
            val.accept(visitInfo(e.getKey(), &me, n, len, i.depth + 1), *this);
        }
    }

    void me::onTraverse(const visitInfo& i, getExpr& e) {
        // check me:
        const args& args = e.getArgs();
        nint n = 0;
        node* me = e._me.get();
        ncnt len = args.len() + (me ? 1 : 0);
        if(me) me->accept(visitInfo("me", &e, n++, len, i.depth + 1), *this);

        // check arguments:
        for(auto& elem: args)
            elem.accept(visitInfo("subArg", &e, n++, len, i.depth + 1), *this);
    }

    void me::onTraverse(const visitInfo& i, frame& f) {
        // do nothing.
    }

    void me::onTraverse(const visitInfo& i, runExpr& e) {
        int n = 0;
        node* me = e.getMe();
        node& subject = e.getSubj();
        const args& a = e.getArgs();
        int len = a.len() + (me ? 1 : 0) + 1;

        if(me) me->accept(visitInfo("me", &e, n++, len, i.depth + 1), *this);

        onTraverse(e, subject);

        subject.accept(visitInfo("subject", &e, n++, len, i.depth + 1), *this);

        for(auto& elem: a)
            elem.accept(visitInfo("arg", &e, n++, len, i.depth + 1), *this);
    }

    void me::onTraverse(runExpr& e, node& subject) {}

    void me::onTraverse(const visitInfo& i, func& f) {
        onTraverse(i, (func::super&) f);

        ends& es = f.getEnds();
        ncnt len = es.len() + 1;
        f.getBlock().accept(visitInfo("codes", &f, 0, len, i.depth + 1), *this);
        for(nint n = es.len() - 1; n >= 0; n--)
            es[n].accept(visitInfo("end", &f, n + 1, len, i.depth + 1), *this);
    }

    void me::onTraverse(const visitInfo& i, blockExpr& b) {
        int n = 0;
        narr& stmts = b.getStmts();
        int len = stmts.len();
        for(auto& stmt: stmts)
            stmt.accept(visitInfo("", &b, n++, len, i.depth + 1), *this);
    }

    void me::onTraverse(const visitInfo& i, retExpr& b) {
        int n = 0;
        node& ret = b.getRet();
        ret.accept(visitInfo("", &b, n++, 1, i.depth + 1), *this);
    }

    void me::onTraverse(const visitInfo& i, asExpr& a) {
        int n = 0;
        node& me = (node&) a.getMe();
        node& as = (node&) a.getAs();
        me.accept(visitInfo("me", &a, n++, 2, i.depth + 1), *this);
        as.accept(visitInfo("as", &a, n++, 2, i.depth + 1), *this);
    }

    void me::onTraverse(const visitInfo& i, assignExpr& a) {
        int n = 0;
        node& left = (node&) a.getLeft();
        node& right = (node&) a.getRight();
        left.accept(visitInfo("lhs", &a, n++, 2, i.depth + 1), *this);
        right.accept(visitInfo("rhs", &a, n++, 2, i.depth + 1), *this);
    }

    void me::onTraverse(const visitInfo& i, defVarExpr& d) {
        node& r = d.getRight() OR.ret();
        r.accept(visitInfo(d.getName(), &d, 0, 1, i.depth + 1), *this);
    }

    void me::onTraverse(const visitInfo& i, FBOExpr& f) {
        int n = 0;
        node& left = (node&) f.getLeft();
        node& right = (node&) f.getRight();
        left.accept(visitInfo("lhs", &f, n++, 2, i.depth + 1), *this);
        right.accept(visitInfo("rhs", &f, n++, 2, i.depth + 1), *this);
    }

    void me::onTraverse(const visitInfo& i, forExpr& f) {
        str con = f.getContainer();
        ncnt len = con ? 2 : 1;
        ncnt n = 0;
        if(con) con->accept(visitInfo("condition", &f, n++, len, i.depth + 1), *this);
        f.getBlock() TO(accept(visitInfo("codes", &f, n++, len, i.depth + 1), *this));
    }

    void me::onTraverse(const visitInfo& i, retStateExpr& r) {}

    void me::onTraverse(const visitInfo& i, ifExpr& f) {
        blockExpr* elseBlk = f.getElse();
        int len = elseBlk ? 3 : 2;

        f.getCondition().accept(visitInfo("condition", &f, 0, len, i.depth + 1), *this);

        f.getThen().accept(visitInfo("then", &f, 1, len, i.depth + 1), *this);
        if(elseBlk) elseBlk->accept(visitInfo("else", &f, 2, len, i.depth + 1), *this);
    }

    void me::onTraverse(const visitInfo& i, whileExpr& w) {
        w.getCondition().accept(visitInfo("condition", &w, 0, 2, i.depth + 1), *this);
        w.getBlock() TO(accept(visitInfo("codes", &w, 1, 2, i.depth + 1), *this));
    }

    void me::onTraverse(const visitInfo& i, defArrayExpr& d) {
        narr& elems = d.getElems();
        ncnt len = elems.len();
        nidx n = 0;
        for(node& e: elems) {
            e.accept(visitInfo("arg" + std::to_string(n), &d, n, len, i.depth + 1), *this);
            n++;
        }
    }

    void me::onTraverse(const visitInfo& i, defNestedFuncExpr& d) {
        d._getOrigin().accept(visitInfo("origin", &d, 0, 1, i.depth + 1), *this);
    }

    void me::onTraverse(const visitInfo& i, genericOrigin& g) {
        auto& cache = (std::map<std::string, tstr<obj>>&) g.getCache();
        ncnt len = cache.size();
        nint n = 0;
        for(auto& e: cache)
            e.second->accept(visitInfo(e.first, &g, n++, len, i.depth + 1), *this);
    }

    void me::onTraverse(const visitInfo& i, obj& o) {
        onTraverse(i, (obj::super&) o);

        auto* cc = o.getCallComplete();
        if(cc) cc->accept(visitInfo("callComplete", &o, 0, 1, i.depth + 1), *this);
    }
} // namespace by
