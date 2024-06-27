#include "visitor.hpp"
#include "../ast.hpp"
#include "../frame/frame.hpp"
#include "../builtin.hpp"
#include "../loader/worker/worker.inl"

namespace namu {

    template class _nout worker<void, node>;

    NAMU_DEF_ME(visitor)

#define X(T) \
    void me::visit(visitInfo i, T& me) { \
        if(nul(me)) return; \
        \
        if(!_markVisited(me)) return; \
        \
        if(onVisit(i, me)) \
            onTraverse(i, me); \
        onLeave(i, me); \
    } \
    nbool me::onVisit(visitInfo i, T& me) { return onVisit(i, (T::super&) me); } \
    void me::onLeave(visitInfo i, T& me) { onLeave(i, (T::super&) me); }

#   include "visitee.inl"
#undef X

    void me::_prepare() {
        super::_prepare();
        _visited.clear();
    }

    void me::visit(visitInfo i, node& me) {
        if(nul(me)) return;
        if(!_markVisited(me)) return;

        if(onVisit(i, me))
            onTraverse(i, me);
        onLeave(i, me);
    }
    nbool me::onVisit(visitInfo i, node& me) { return true; }
    void me::onLeave(visitInfo i, node& me) {}

    void me::_onWork() {
        if(nul(getTask())) return;

        _visited.clear();
        getTask().accept(visitInfo {"#root", nullptr, 0, 1, 1}, *this);
    }

    nbool me::_markVisited(node& me) {
        if(_visited.find(&me) != _visited.end()) return false;

        return _visited[&me] = true;
    }

    void me::onTraverse(visitInfo i, node& me) {
        scope& subs = me.subs();
        ncnt len = subs.len();
        len = subs.getContainer().len();
        const auto& next = subs.getNext();
        if(!nul(next))
            len += next.getContainer().len();
        if(len <= 0) return;

        auto e = subs.begin();
        for(int n=0; n < len ;n++, ++e) {
            node& val = e.getVal();
            val.accept(visitInfo {e.getKey(), &me, n, len, i.depth+1}, *this);
        }
    }

    void me::onTraverse(visitInfo i, getExpr& e) {
        // check me:
        const args& args = e.getSubArgs();
        nint n = 0;
        node& me = *e._me;
        ncnt len = (nul(args) ? 0 : args.len()) + (nul(me) ? 0 : 1);
        if(!nul(me))
            me.accept(visitInfo {"me", &e, n++, len, i.depth+1}, *this);

        // check arguments:
        if(!nul(args))
            for(auto& elem : e.getSubArgs())
                elem.accept(visitInfo {"subArg", &e, n++, len, i.depth+1}, *this);
    }

    void me::onTraverse(visitInfo i, frame& f) {
        // do nothing.
    }

    void me::onTraverse(visitInfo i, runExpr& e) {
        int n = 0;
        node& me = e.getMe();
        node& subject = e.getSubject();
        int len = e.getArgs().len() + (nul(me) ? 0 : 1) + (nul(subject) ? 0 : 1);
        if(!nul(me))
            me.accept(visitInfo {"me", &e, n++, len, i.depth+1}, *this);

        onTraverse(e, subject);

        if(!nul(subject))
            subject.accept(visitInfo {"subject", &e, n++, len, i.depth+1}, *this);

        for(auto& elem : e.getArgs())
            elem.accept(visitInfo {"arg", &e, n++, len, i.depth+1}, *this);
    }

    void me::onTraverse(runExpr& e, node& subject) {}

    void me::onTraverse(visitInfo i, func& f) {
        f.getBlock().accept(visitInfo {"codes", &f, 0, 1, i.depth+1}, *this);
    }

    void me::onTraverse(visitInfo i, blockExpr& b) {
        int n = 0;
        narr& stmts = b.getStmts();
        int len = stmts.len();
        for(auto& stmt : stmts)
            stmt.accept(visitInfo {"", &b, n++, len, i.depth+1}, *this);
    }

    void me::onTraverse(visitInfo i, retExpr& b) {
        int n = 0;
        node& ret = b.getRet();
        if(!nul(ret))
            ret.accept(visitInfo {"", &b, n++, 1, i.depth+1}, *this);
    }

    void me::onTraverse(visitInfo i, asExpr& a) {
        int n = 0;
        node& me = (node&) a.getMe();
        node& as = (node&) a.getAs();
        ncnt len = (nul(me) ? 0 : 1) + (nul(as) ? 0 : 1);
        if(!nul(me))
            me.accept(visitInfo {"me", &a, n++, len, i.depth+1}, *this);
        if(!nul(as))
            as.accept(visitInfo {"as", &a, n++, len, i.depth+1}, *this);
    }

    void me::onTraverse(visitInfo i, assignExpr& a) {
        int n = 0;
        node& left = (node&) a.getLeft();
        node& right = (node&) a.getRight();
        ncnt len = (nul(left) ? 0 : 1) + (nul(right) ? 0 : 1);
        if(!nul(left))
            left.accept(visitInfo {"lhs", &a, n++, len, i.depth+1}, *this);
        if(!nul(right))
            right.accept(visitInfo {"rhs", &a, n++, len, i.depth+1}, *this);
    }

    void me::onTraverse(visitInfo i, defPropExpr& d) {
        node& r = (node&) d.getRight();
        if(!nul(r))
            r.accept(visitInfo {d.getName(), &d, 0, 1, i.depth+1}, *this);
    }

    void me::onTraverse(visitInfo i, FBOExpr& f) {
        int n = 0;
        node& left = (node&) f.getLeft();
        node& right = (node&) f.getRight();
        ncnt len = (nul(left) ? 0 : 1) + (nul(right) ? 0 : 1);
        if(!nul(left))
            left.accept(visitInfo {"lhs", &f, n++, len, i.depth+1}, *this);
        if(!nul(right))
            right.accept(visitInfo {"rhs", &f, n++, len, i.depth+1}, *this);
    }

    void me::onTraverse(visitInfo i, forExpr& f) {
        node& con = *f.getContainer();
        ncnt len = nul(con) ? 1 : 2;
        ncnt n = 0;
        if(!nul(con))
            con.accept(visitInfo {"condition", &f, n++, len, i.depth+1}, *this);
        f.getBlock().accept(visitInfo {"codes", &f, n++, len, i.depth+1}, *this);
    }

    void me::onTraverse(visitInfo i, retStateExpr& r) {}

    void me::onTraverse(visitInfo i, ifExpr& f) {
        blockExpr& elseBlk = f.getElseBlk();
        int len = !nul(elseBlk) ? 3 : 2;

        f.getCondition().accept(visitInfo {"condition", &f, 0, len, i.depth+1}, *this);

        f.getThenBlk().accept(visitInfo {"then", &f, 1, len, i.depth+1}, *this);
        if(!nul(elseBlk)) {
            onTraverse(f, elseBlk);
            elseBlk.accept(visitInfo {"else", &f, 2, len, i.depth+1}, *this);
        }
    }

    void me::onTraverse(ifExpr& e, blockExpr& blk) {}

    void me::onTraverse(visitInfo i, whileExpr& w) {
        w.getCondition().accept(visitInfo {"condition", &w, 0, 2, i.depth+1}, *this);
        w.getBlock().accept(visitInfo {"codes", &w, 1, 2, i.depth+1}, *this);
    }

    void me::onTraverse(visitInfo i, defArrayExpr& d) {
        narr& elems = d.getElems();
        int len = elems.len(),
            n = 0;
        for(node& e : elems)
            e.accept(visitInfo {"arg" + std::to_string(n), &d, n++, len, i.depth+1}, *this);
    }

    void me::onTraverse(visitInfo i, genericObj& g) {
        auto& cache = (std::map<std::string, tstr<obj>>&) g.getCache();
        ncnt len = cache.size();
        nint n = 0;
        for(auto& e : cache)
            e.second->accept(visitInfo{e.first, &g, n++, len, i.depth+1}, *this);
    }
}
