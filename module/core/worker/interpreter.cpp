#include "core/worker/interpreter.hpp"
#include "core/ast/exprs/blockExpr.hpp"
#include "core/ast/func.hpp"
#include "core/ast/origin.hpp"
#include "core/frame/threadUse.hpp"
#include "core/worker/visitor/graphVisitor.hpp"
#include "core/worker/visitor/expander.hpp"
#include "core/worker/worker.inl"

namespace by {

    template class _nout worker<tstr<slot>, slot>;

    BY_DEF_ME(interpreter)

    me::interpreter(): _isParsed(false) {}

    me& me::addSupply(const srcSupply& supply) {
        _pser.addSupply(supply);
        return *this;
    }

    me& me::relSupplies() {
        _pser.relSupplies();
        return *this;
    }

    nbool me::isParsed() const { return _isParsed; }

    nbool me::isVerified() const { return isParsed() && !getReport().inErr(); }

    node* me::getSubPack() { return _pser.getSubPack(); }

    tstr<slot> me::_onWork() {
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
        if(isFlag(LOG_STRUCTURE) && _pser.getSubPack() && getTask())
            graphVisitor().setShowData(showData).setFlag(0).setTask(getTask()).work();
    }

    void me::rel() {
        this->super::rel();
        _isParsed = false;
        _veri.rel();
        _pser.rel();
    }

    nbool me::_isPackExist() { return _pser.getSubPack() && getTask(); }

    namespace {
        template <typename T, typename E> nbool _visit(interpreter& i, T& v, E* task) {
            BY_DI("======================================");
            BY_DI("|               %s                   |", v.getType().getName().c_str());
            BY_DI("======================================");

            v.setReport(i.getReport())
                .setFlag(i.getFlag())
                .delFlag(interpreter::LOG_ON_END | interpreter::DUMP_ON_END)
                .setTask(task)
                .work();

            return true;
        }
    }

    void me::_parse() {
        WHEN(!_visit(*this, _pser, getTask())) .ret();
        if(!getTask()) setTask(_pser.getTask());

        _isParsed = _isPackExist() && _pser.isOk();
    }

    void me::_expand() {
        threadUse thr;
        expander evaler;
        WHEN_NUL(getTask()).err("_slot is null").ret();
        _visit(*this, evaler, getTask() TO(getPack()));
    }

    void me::_verify() {
        threadUse thr;
        WHEN_NUL(getTask()).err("_slot is null").ret();
        _visit(*this, _veri, getTask() TO(getPack()));
    }
} // namespace by
