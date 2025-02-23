#include "retExpr.hpp"

#include "../../builtin/primitive/nVoid.hpp"
#include "../../frame/thread.hpp"
#include "../../worker/visitor/visitor.hpp"
#include "../../builtin/err/nerr.hpp"
#include "../closure.hpp"

namespace nm {

    NM(DEF_ME(retExpr), DEF_VISIT())

    me::retExpr(const node& ret): _ret(ret) {}

    me::retExpr(): _ret(nVoid::singletone()) {}

    namespace {
        tstr<baseErr> _returnEx(tstr<baseErr> e) {
            thread& thr = thread::get();
            thr.getEx().add(*e);
            thr.getNowFrame().setRet(*e);
            return e;
        }
    }

    str me::run(const args& a) {
        auto& fr = thread::get().getNowFrame();
        if(!_ret) return str(nVoid::singletone());

        // check retValue is null or not:
        //  ret should be void if there is no value to return. so 'null' not allowed here.
        str ret = _ret->as<node>() orRet _returnEx(
            nerr::newErr(errCode::RETURN_VALUE_IS_NUL, getSrc().getName().c_str()));
        NM_DI("retExpr: ret[%s]", ret);

        // check exception occured during running func.
        str fRet = fr.getFunc().getRet();
        if(_isEx(*ret, *fRet)) return _returnEx(ret->cast<baseErr>());

        // implicit closure:
        if(!ret->isSub<baseObj>()) {
            closure* closure = closure::make(*ret);
            if(closure) ret.bind(closure);
        }

        NM_DI("retExpr: frame.setRet(%s)", ret);
        fr.setRet(*ret);
        return ret;
    }

    nbool me::_isEx(const node& got, const node& funcRet) {
        return got.isSub<baseErr>() && !funcRet.isSub<baseErr>();
    }

    node& me::getRet() { return *_ret; }

    priorType me::prioritize(const args& a) const {
        if(_ret) return _ret->prioritize(a);
        return super::prioritize(a);
    }

    str me::getEval() const {
        // why you don't 'getEval()' to '_ret'?:
        //  think about a below codes:
        //      for strArr in {{"hello"}}
        //          for s in strArr
        //              ret s
        //
        //  in this case, when you getEval() to first 'for', which type should be returned?
        //  you may think like "isn't it 'strType'? because there is 'ret' keyword inside of nested
        //  loop." and I would say no. when 'forExpr.getEval()' called, forExpr class need to return
        //  array type of stmt's eval type. so normally, first for loop should be evaluated as
        //  'arr<arr<str>>' because its nested for loop will be evaluated to 'arr<str>'.
        //
        //  however, you may notice, if there is any 'ret' something in any level of depth to for
        //  loop, then final evaluated type of all nested for loop should be what 'ret' evaluated
        //  inside. the problem is, of course, most outer for loop can't know immedately whether
        //  there is 'ret' inside of its block stmt without DFS.
        //
        //  so I return 'retExpr' itself when getEval() called.
        //  by doing this, each for loop can judge its block stmt has 'ret' or not. it affects for
        //  'for loop' which type does they should evalutes.
        //
        //  when func gets 'retExpr', they don't do anything.
        //  you may concern that type mismatching between 'ret' and return type of 'func'.
        //  but don't worry. that part will be handled by perspective of each 'ret' keyword.
        //  @see 'verifier::onVisit(visitInfo, retExpr&)' for more info.
        return this;
    }
} // namespace nm
