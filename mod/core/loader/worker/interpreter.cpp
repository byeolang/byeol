#include "../../ast/exprs/blockExpr.hpp"
#include "../../ast/func.hpp"
#include "interpreter.hpp"
#include "../../visitor/graphVisitor.hpp"
#include "../../visitor/preEvaluator.hpp"
#include "../../frame/threadUse.hpp"
#include "worker.inl"

namespace namu {

    template class _nout worker<tstr<slot>, slot>;

    NAMU_DEF_ME(interpreter)

    me::interpreter(): _isParsed(false) {}

    me& me::addSupply(const srcSupply& supply) {
        _pser.addSupply(supply);
        return *this;
    }

    me& me::relSupplies() {
        _pser.relSupplies();
        return *this;
    }

    nbool me::isParsed() const {
        return _isParsed;
    }

    nbool me::isVerified() const {
        return isParsed() && (!nul(getReport()) && !getReport().hasErr());
    }

    node& me::getSubPack() {
        return _pser.getSubPack();
    }

    tstr<slot> me::_onWork() {
        _parse();
        if(getReport())
            return getTask();
        _preEval();
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

    nbool me::_isPackExist() {
        return !nul(_pser.getSubPack()) && !nul(getTask());
    }

    void me::_parse() {
        NAMU_DI("======================================");
        NAMU_DI("|               parse                |");
        NAMU_DI("======================================");

        _pser.setReport(getReport())
             .setTask(getTask())
             .work();

        if(nul(getTask()))
            setTask(_pser.getTask());

        _isParsed = _isPackExist() && _pser.isOk();
    }

    void me::_preEval() {
        NAMU_DI("======================================");
        NAMU_DI("|               preEval              |");
        NAMU_DI("======================================");

        if(nul(getTask()))
            return NAMU_E("_slot is null"), void();

        threadUse thr;
        preEvaluator evaler;
        evaler.setReport(getReport())
              .setFlag(getFlag())
              .setTask(getTask().getPack())
              .work();
    }

    void me::_verify() {
        NAMU_DI("======================================");
        NAMU_DI("|                verify              |");
        NAMU_DI("======================================");

        if(nul(getTask()))
            return NAMU_E("_slot is null"), void();

        // verify:
        threadUse thr;
        _veri.setReport(getReport())
             .setFlag(getFlag());
        if(isFlag(LOG_ON_END | DUMP_ON_END))
            _veri.delFlag(LOG_ON_END | DUMP_ON_END);
        _veri.setTask(getTask().getPack())
             .work();
    }
}
