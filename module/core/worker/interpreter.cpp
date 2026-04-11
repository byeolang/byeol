#include "core/worker/interpreter.hpp"
#include "core/ast/exprs/blockExpr.hpp"
#include "core/ast/func.hpp"
#include "core/ast/origin.hpp"
#include "core/frame/threadUse.hpp"
#include "core/worker/visitor/graphVisitor.hpp"
#include "core/worker/visitor/expander.hpp"
#include "core/worker/tworker.inl"

namespace by {

    template class _nout tworker<tstr<pod>, pod>;

    BY_DEF_ME(interpreter)

    me::interpreter(): _isParsed(false) {}

    parser& me::getParser() { return _pser; }

    verifier& me::getVerifier() { return _veri; }

    nbool me::isParsed() const { return _isParsed; }

    nbool me::isVerified() const { return isParsed() && !getReport().inErr(); }

    node* me::getSubPod() { return _pser.getSubPod(); }

    tstr<pod> me::_onWork() {
        // TODO: don't use static variable '_cache':
        //  instead, put cache onto origin object, and if arr instance is origin, remove the cache.
        arr::_cache.clear();

        _parse();
        WHEN(getReport()) .ret(getTask());

        _showGraph(false);
        _expand();
        _verify();
        _showGraph(true);

        return getTask();
    }

    void me::_showGraph(nbool showData) const {
        // eval with dumThread.
        if(isFlag(LOG_STRUCTURE) && _pser.getSubPod() && getTask())
            graphVisitor().setShowData(showData).setFlag(0).setTask(getTask()).work();
    }

    void me::rel() {
        this->super::rel();
        _isParsed = false;
        _veri.rel();
        _pser.rel();
    }

    nbool me::_isPodExist() { return _pser.getSubPod() && getTask(); }

    namespace {
        template <typename T, typename E> nbool _visit(interpreter& i, T& v, E* task) {
            BY_DI("======================================");
            BY_DI("|               %s                   |", v.getType().getName().c_str());
            BY_DI("======================================");

            v.setReport(i.getReport())
                .setFlag(i.getFlag()) // uses thread's errReport.
                .delFlag(interpreter::LOG_ON_END | interpreter::DUMP_ON_END)
                .setTask(task)
                .work();

            return true;
        }
    }

    void me::_parse() {
        WHEN(!_visit(*this, _pser, getTask())) .ret();
        if(!getTask()) setTask(_pser.getTask());

        _isParsed = _isPodExist() && _pser.isOk();
    }

    void me::_expand() {
        threadUse thr(getReport());
        expander evaler;
        WHEN_NUL(getTask()).err("_pod is null").ret();
        _visit(*this, evaler, getTask());
    }

    void me::_verify() {
        threadUse thr(getReport());
        WHEN_NUL(getTask()).err("_pod is null").ret();
        _visit(*this, _veri, getTask());
    }
} // namespace by
