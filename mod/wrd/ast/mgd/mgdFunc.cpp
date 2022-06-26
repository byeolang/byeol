#include "mgdFunc.hpp"
#include "../obj.hpp"
#include "../../builtin/container/native/tnchain.inl"
#include "../../frame/thread.hpp"
#include "../params.hpp"
#include "../../loader/interpreter/tverification.hpp"
#include "../../loader/interpreter/verification.inl"
#include "../../loader/interpreter/verifier.hpp"
#include "../../frame/frameInteract.hpp"
#include "../typeNode.hpp"

namespace wrd {

    WRD_DEF_ME(mgdFunc)

    me::mgdFunc(const params& ps, const wtype& retType):
            super(), _params(ps), _retType(&retType), _blk(new blockExpr()) {}
    me::mgdFunc(const params& ps, const wtype& retType, const blockExpr& newBlock):
            super(), _params(ps), _retType(&retType), _blk(newBlock) {}

    blockExpr& me::getBlock() { return *_blk; }
    const blockExpr& me::getBlock() const { return *_blk; }
    const wtype& me::getRetType() const { return *_retType; }
    nbicontainer& me::subs() { return _shares; }
    const params& me::getParams() const { return _params; }

    str me::run(const ucontainable& args) {
        str ret;
        if(nul(args)) return WRD_E("args == null"), ret;

        // s is from heap space. but freed by _outFrame() of this class.
        scope& s = *_evalArgs(args);
        frame& fr = thread::get()._getNowFrame();
        if(nul(fr)) return WRD_E("fr == null"), ret;
        baseObj& meObj = fr.getObj();
        if(nul(meObj)) return WRD_E("meObj == null"), ret;

        frameInteract f1(meObj); {
            frameInteract f2(*this, s); {
                _blk->run();
                ret = thread::get()._getNowFrame().popReturn();
            }
        }
        return ret;
    }

    scope* me::_evalArgs(const ucontainable& args) {
        const params& ps = getParams();
        if(args.len() != ps.len())
            return WRD_E("length of args(%d) and typs(%d) doesn't match.", args.len(), ps.len()), nullptr;

        scope* ret = new scope();
        int n = 0;
        for(const node& e: args) {
            const param& p = ps[n++];
            str evaluated = e.as(p.getOrgType());
            if(!evaluated) return nullptr;

            ret->add(p.getName(), *evaluated);
        }
        return ret;
    }

    void me::_inFrame(const bicontainable& args) {
        frame& fr = thread::get()._getNowFrame();
        if(nul(fr)) {
            WRD_E("fr == null");
            return;
        }

        WRD_DI("%s._onInFrame()", getType().getName().c_str());
        fr.pushLocal(subs());
        fr.setFunc(*this);
        fr.pushLocal((nbicontainer&) args); // including 'me'
    }

    void me::_outFrame() {
        WRD_DI("%s._onOutFrame()", getType().getName().c_str());

        frame& fr = thread::get()._getNowFrame();
        // TODO: is it safe to delete below lines?
        fr.setFunc(nulOf<func>());
        fr.popLocal();
        fr.popLocal();
    }

    WRD_VERIFY({
        WRD_DI("verify: retType exists and stmts exist one at least");

        const wtype& retType = it.getRetType();
        if(nul(retType)) return _srcErr(errCode::NO_RET_TYPE);
        if(!retType.isSub(ttype<node>::get()))
            return _srcErr(errCode::WRONG_RET_TYPE, retType.getName().c_str());

        const blockExpr& blk = it.getBlock();
        if(nul(blk) || blk.getStmts().len() <= 0)
            return _err(blk.getPos(), errCode::NO_STMT_IN_FUNC);
    })

    namespace {
        void _prepareArgsAlongParam(const params& ps, scope& s) {
            for(const auto& p : ps)
                s.add(p.getName(), new typeNode(p.getOrgType()));
        }
    }

    // TODO: verify arguments

    WRD_VERIFY(mgdFunc, subNodes, {
        WRD_DI("verify: mgdFunc: %s iterateBlock[%d]", it.getType().getName().c_str(), it._blk->subs().len());
        scope* s = new scope();
        _prepareArgsAlongParam(it.getParams(), *s);

        frame& fr = (frame&) thread::get().getNowFrame();
        if(nul(fr)) return _err(errCode::FUNC_REDIRECTED_OBJ);
        baseObj& meObj = fr.getObj();
        if(nul(meObj)) return _err(errCode::FUNC_REDIRECTED_OBJ);

        frameInteract f1(meObj); {
            frameInteract f2(it, *s); {
                verify(*it._blk);
            }
        }
    })
}
