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

    nbool me::isVerified() const {
        return isParsed() && !getReport().inErr();
    }

    node* me::getSubPack() { return _pser.getSubPack(); }

    tstr<slot> me::_onWork() {
        // TODO: don't use static variable '_cache':
        //  instead, put cache onto origin object, and if arr instance is origin, remove the cache.
        arr::_cache.clear();

        _parse();
        WHEN(getReport()).ret(getTask());

        _showGraph(false);
        _expand();
        _verify();
        _showGraph(true);

        return getTask();
    }

    void me::_showGraph(nbool showData) const {
        // run with dumThread.
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

    void me::_parse() {
        BY_DI("======================================");
        BY_DI("|               parse                |");
        BY_DI("======================================");

        _pser.setReport(getReport())
            .setFlag(getFlag())
            .delFlag(LOG_ON_END | DUMP_ON_END)
            .setTask(getTask())
            .work();

        if(!getTask()) setTask(_pser.getTask());

        _isParsed = _isPackExist() && _pser.isOk();
    }

    void me::_expand() {
        BY_DI("======================================");
        BY_DI("|               expand               |");
        BY_DI("======================================");

        WHEN_NUL(getTask()).err("_slot is null").ret();

        threadUse thr;
        expander evaler;
        evaler.setReport(getReport())
            .setFlag(getFlag())
            .delFlag(LOG_ON_END | DUMP_ON_END)
            .setTask(getTask() TO(getPack()))
            .work();
    }

    void me::_verify() {
        BY_DI("======================================");
        BY_DI("|                verify              |");
        BY_DI("======================================");

        WHEN_NUL(getTask()).err("_slot is null").ret();

        // verify:
        threadUse thr;
        _veri.setReport(getReport())
            .setFlag(getFlag())
            .delFlag(LOG_ON_END | DUMP_ON_END)
            .setTask(getTask() TO(getPack()))
            .work();
    }
} // namespace by
