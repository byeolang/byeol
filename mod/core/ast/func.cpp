#include "func.hpp"

#include "../builtin/container/native/tnchain.inl"
#include "../frame/frameInteract.hpp"
#include "../frame/thread.hpp"
#include "../worker/visitor/visitor.hpp"
#include "obj.hpp"
#include "params.hpp"
#include "../builtin/err/nerr.hpp"

namespace nm {

    NM(DEF_ME(func), DEF_VISIT())

    me::func(const modifier& mod, const mgdType& type):
        super(mod), _type(type), _blk(new blockExpr()) {}

    me::func(const modifier& mod, const mgdType& type, const blockExpr& newBlock):
        super(mod), _type(type), _blk(newBlock) {}

    const ntype& me::getType() const { return _type; }

    blockExpr& me::getBlock() { return *_blk; }

    const blockExpr& me::getBlock() const { return *_blk; }

    void me::setBlock(const blockExpr& new1) { _blk.bind(new1); }

    scope& me::subs() { return _shares; }

    str me::run(const args& a) {
        if(nul(a)) return NM_E("a == null"), str();
        if(!thread::get().isInteractable())
            return NM_E("thread isn't interactable"),
                   nerr::newErr(errCode::THERE_IS_NO_FRAMES_IN_THREAD);

        // s is from heap space. but freed by _outFrame() of this class.
        scope& s = *_evalArgs(a);
        if(nul(s)) return str();
        node& meObj = a.getMe();
        if(nul(meObj)) return NM_E("meObj == null"), str();

        str ret;
        nidx exN = thread::get().getEx().len() - 1;
        frameInteract f1(meObj);
        {
            frameInteract f2(*this, s);
            {
                frameInteract f3(*_blk);
                {
                    _runEnds();
                    return _postprocess(_blk->run(), exN);
                }
            }
        }
    }

    str me::_postprocess(str blkRes, nidx exN) {
        frame& fr = thread::get()._getNowFrame();
        str frRes = fr.getRet();
        str res = frRes ? frRes : blkRes;
        fr.setRet();
        if(nul(res)) return NM_E("res == null"), str();

        // if I got new exception, I just return it.
        if(thread::get().getEx().inErr(exN)) return res->as(*getRet().as<node>());
        if(frRes) return frRes; // if you 'ret' for retuning a func, retExpr will make a closure.

        // implicit closure:
        //  if you are returning func, then I'll make a closure for it.
        //  so don't think about that scenario. only I should care is last stmt of block, that is,
        //  'ret'.
        const node& lastStmt = *_blk->getStmts().last();
        const getExpr& get = safeGet(lastStmt, cast<getExpr>());
        if(!nul(get)) {
            // ok. implicit returning for last stmt was func. getExpr is suitable to make a closure.
            getExpr& get = _blk->getStmts().last()->cast<getExpr>();
            NM_WHENNUL(get).ex(CANT_RETURN_A_CLOSURE), str();
            return get.makeClosure();
        }

        return blkRes;
    }

    void me::_runEnds() {
        for(nidx n = _ends.len() - 1; n >= 0; n--)
            _ends[n].run();
    }

    scope* me::_evalArgs(const ucontainable& args) {
        const params& ps = getParams();
        if(args.len() != ps.len())
            return NM_E("length of args(%d) and typs(%d) doesn't match.", args.len(), ps.len()),
                   nullptr;

        scope* ret = new scope();
        int n = 0;
        for(const node& e: args) {
            const param& p = ps[n++];
            str evaluated = e.asImpli(*p.getOrigin().as<node>());
            if(!evaluated)
                return NM_E("evaluation of arg[%s] -> param[%s] has been failed.", e,
                           p.getOrigin()),
                       ret;

            ret->add(p.getName(), *evaluated);
        }
        return ret;
    }

    void me::inFrame(const bicontainable& args) {
        frame& fr = thread::get()._getNowFrame();
        if(nul(fr)) {
            NM_E("fr == null");
            return;
        }

        NM_DI("'%s func'._inFrame() frames.len[%d]", getSrc(), thread::get().getFrames().len());
        fr.setFunc(*this);
        fr.add(*this);
        fr.add(*scope::wrap<scope>(
            nul(args) ? nulOf<nbicontainer>() : (nbicontainer&) args)); // including 'me'
    }

    void me::outFrame(const bicontainable& args) {
        NM_DI("'%s func'._outFrame() frames.len[%d]", getSrc(), thread::get().getFrames().len());

        frame& fr = thread::get()._getNowFrame();
        baseFunc& f = fr.getFunc();
        if(nul(f) || &f != this) return;

        fr.setFunc();
        if(!nul(args)) fr.del();
        fr.del();
    }

    ends& me::getEnds() { return _ends; }

    void me::onCloneDeep(const clonable& from) {
        const me& rhs = (const me&) from;
        _type.onCloneDeep(rhs._type);
        _shares.onCloneDeep(rhs._shares);
        _blk.bind((blockExpr*) rhs._blk->cloneDeep());
    }
} // namespace nm
