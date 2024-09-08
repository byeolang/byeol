#include "interpreter.hpp"

#include "../ast/exprs/blockExpr.hpp"
#include "../ast/func.hpp"
#include "../ast/origin.hpp"
#include "../frame/threadUse.hpp"
#include "visitor/graphVisitor.hpp"
#include "visitor/expander.hpp"
#include "worker.inl"

namespace nm {

    template class _nout worker<tstr<slot>, slot>;

    NM_DEF_ME(interpreter)

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
        return isParsed() && (!nul(getReport()) && !getReport().inErr());
    }

    node& me::getSubPack() { return _pser.getSubPack(); }

    tstr<slot> me::_onWork() {
        _parse();
        if(getReport()) return getTask();
        _expand();
        _verify();
        _showGraph();

        return getTask();
    }

    void me::_showGraph() const {
        // run with dumThread.
        if(isFlag(LOG_STRUCTURE) && !nul(_pser.getSubPack()) && !nul(getTask()))
            graphVisitor().setFlag(0).setTask(getTask()).work();
    }

    void me::rel() {
        this->super::rel();
        _isParsed = false;
        _veri.rel();
        _pser.rel();
    }

    nbool me::_isPackExist() { return !nul(_pser.getSubPack()) && !nul(getTask()); }

    void me::_parse() {
        NM_DI("======================================");
        NM_DI("|               parse                |");
        NM_DI("======================================");

        _pser.setReport(getReport())
            .setFlag(getFlag())
            .delFlag(LOG_ON_END | DUMP_ON_END)
            .setTask(getTask())
            .work();

        if(nul(getTask())) setTask(_pser.getTask());

        _isParsed = _isPackExist() && _pser.isOk();
    }

    void me::_expand() {
        NM_DI("======================================");
        NM_DI("|               expand               |");
        NM_DI("======================================");

        if(nul(getTask())) return NM_E("_slot is null"), void();

        threadUse thr;
        expander evaler;
        evaler.setReport(getReport())
            .setFlag(getFlag())
            .delFlag(LOG_ON_END | DUMP_ON_END)
            .setTask(getTask().getPack())
            .work();
    }

    void me::_verify() {
        NM_DI("======================================");
        NM_DI("|                verify              |");
        NM_DI("======================================");

        if(nul(getTask())) return NM_E("_slot is null"), void();

        // verify:
        threadUse thr;
        _veri.setReport(getReport())
            .setFlag(getFlag())
            .delFlag(LOG_ON_END | DUMP_ON_END)
            .setTask(getTask().getPack())
            .work();
    }
} // namespace nm
