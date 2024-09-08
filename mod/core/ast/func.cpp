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

    me::func(const modifier& mod, const params& ps, const node& retType):
        super(mod), _params(ps), _retType(retType), _blk(new blockExpr()) {}

    me::func(const modifier& mod, const params& ps, const node* retType):
        super(mod), _params(ps), _retType(retType), _blk(new blockExpr()) {}

    me::func(const modifier& mod, const params& ps, const node& retType, const blockExpr& newBlock):
        super(mod), _params(ps), _retType(retType), _blk(newBlock) {}

    me::func(const modifier& mod, const params& ps, const node* retType, const blockExpr& newBlock):
        super(mod), _params(ps), _retType(retType), _blk(newBlock) {}

    blockExpr& me::getBlock() { return *_blk; }

    const blockExpr& me::getBlock() const { return *_blk; }

    void me::setBlock(const blockExpr& new1) { _blk.bind(new1); }

    str me::getRet() const { return _retType; }

    nbool me::setRet(const node& newRet) { return _retType.bind(newRet); }

    scope& me::subs() { return _shares; }

    const params& me::getParams() const { return _params; }

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
                { return _postprocess(_blk->run(), exN); }
            }
        }
    }

    str me::_postprocess(str ret, nidx exN) {
        frame& fr = thread::get()._getNowFrame();
        node* retVal = &fr.getRet();
        if(nul(retVal)) retVal = &ret.get();
        if(nul(retVal)) return NM_E("retVal == null"), str();
        if(!thread::get().getEx().inErr(exN)) // if I got new exception, I just return it.
            ret = retVal->as(*getRet()->as<node>());
        fr.setRet();
        return ret;
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

    clonable* me::cloneDeep() const {
        me* ret = (me*) clone();
        // params:
        ret->_params.rel();
        for(auto e = _params.begin(); e; ++e)
            ret->_params.add((param*) e->cloneDeep());
        // shares:
        ret->_shares.rel();
        for(auto e = _shares.begin(); e; ++e)
            ret->_shares.add(e.getKey(), (node*) e->cloneDeep());
        // retType:
        ret->_retType.bind((node*) _retType->cloneDeep());
        // blk:
        ret->_blk.bind((blockExpr*) _blk->cloneDeep());

        return ret;
    }
} // namespace nm
