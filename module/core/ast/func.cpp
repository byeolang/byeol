#include "core/ast/func.hpp"

#include "core/builtin/container/native/tnchain.inl"
#include "core/frame/frameInteract.hpp"
#include "core/frame/thread.hpp"
#include "core/worker/visitor/visitor.hpp"
#include "core/ast/closure.hpp"
#include "core/ast/obj.hpp"
#include "core/ast/params.hpp"
#include "core/builtin/err/nerr.hpp"

namespace by {

    NM(DEF_ME(func), DEF_VISIT())

    me::func(const modifier& mod, const funcMgdType& type):
        super(mod), _type(type), _blk(new blockExpr()) {}

    me::func(const modifier& mod, const funcMgdType& type, const blockExpr& newBlock):
        super(mod), _type(type), _blk(newBlock) {}

    me::func(const modifier& mod, const funcMgdType& type, const scope& subs,
        const blockExpr& newBlock):
        super(mod), _type(type), _subs(subs), _blk(newBlock) {}

    const ntype& me::getType() const { return _type; }

    blockExpr& me::getBlock() { return *_blk; }

    const blockExpr& me::getBlock() const { return *_blk; }

    void me::setBlock(const blockExpr& new1) { _blk.bind(new1); }

    scope& me::subs() { return _subs; }

    str me::run(const args& a) {
        NM_I("@%s prepare to run `%s(%s)`...", this, getSrc(), getParams());
        WHEN(!thread::get().isInteractable())
            .err("thread isn't interactable")
            .ret(nerr::newErr(errCode::THERE_IS_NO_FRAMES_IN_THREAD));

        // s is from heap space. but freed by _outFrame() of this class.
        tstr<scope> s = _evalArgs(a) OR.ret(str());
        node& meObj = a.getMe() OR.err("meObj == null").ret(str());
        return _interactFrame(meObj, *s, thread::get().getEx().len() - 1);
    }

    str me::_interactFrame(node& meObj, scope& s, nidx exN) {
        frameInteract f1(meObj);
        frameInteract f2(*this, s);
        frameInteract f3(*_blk);
        return _run(exN);
    }

    str me::_run(nidx exN) {
        _runEnds();
        NM_I("@%s --> run `%s(%s)", this, getSrc(), getParams());
        str ret = _blk->run();
        NM_I("@%s <-- ended `%s(%s)", this, getSrc(), getParams());
        ret = _postprocess(ret, exN);
        NM_I("@%s `%s <--ret-- %s(%s)`", this, ret, getSrc(), getParams());
        return ret;
    }

    str me::_postprocess(const str& blkRes, nidx exN) {
        frame& fr = thread::get()._getNowFrame() OR.exErr(THERE_IS_NO_FRAMES_IN_THREAD).ret(str());
        str frRes = fr.getRet();
        str res = frRes ? frRes : blkRes;
        fr.setRet(nullptr);

        WHEN_NUL(res).err("res == null").ret(str());
        const errReport& errs = thread::get().getEx();
        WHEN(errs.inErr(exN)).ret(*errs.last()); // if new exception, I just return it.

        auto* closure = closure::make(*res);
        if(closure) res.bind(closure);
        return res ? res->asImpli(getRet()->as<node>().get()) : res;
    }

    void me::_runEnds() {
        for(nidx n = _ends.len() - 1; n >= 0; n--)
            _ends[n].run();
    }

    void me::_setOrigin(const baseObj& org) { _org.bind(org); }

    scope* me::_evalArgs(const args& a) {
        scope* ret = new scope();
        const params& ps = getParams();
        tmay<args> evaluated = a.evalAll(ps) OR.ret(nullptr);

        for(int n = 0; n < ps.len(); n++)
            ret->add(ps[n].getName(), evaluated->get(n));
        return ret;
    }

    void me::inFrame(const bicontainable* args) const {
        frame& fr = thread::get()._getNowFrame() OR.err("fr == null").ret();

        NM_DI("'%s'._inFrame() frames.len[%d]", *this, thread::get().getFrames().len());
        fr.addFunc(*this);
        fr.add(*this);
        if (args && args->len() > 0) fr.add(scope::wrap<scope>((nbicontainer*) args));
    }

    void me::outFrame() const {
        NM_DI("'%s func'._outFrame() frames.len[%d]", getSrc(), thread::get().getFrames().len());

        frame& fr = thread::get()._getNowFrame() OR.exErr(THERE_IS_NO_FRAMES_IN_THREAD).ret();
        baseFunc* f = fr.getFunc();
        WHEN(f != this).ret();

        fr.delFunc();
        fr.del();
        if(getParams().len() > 0) fr.del();
    }

    const baseObj& me::getOrigin() const { return *_org; }

    ends& me::getEnds() { return _ends; }

    void me::onCloneDeep(const clonable& from) {
        const me& rhs = (const me&) from;
        _type.onCloneDeep(rhs._type);
        _subs.onCloneDeep(rhs._subs);
        _blk.bind((blockExpr*) rhs._blk->cloneDeep());
    }

    nbool me::isAbstract() const { return _blk ? _blk->isAbstract() : true; }
} // namespace by
