#pragma once

#include "worker.hpp"

namespace nm {

    template <typename R, typename T> struct workerAdapter { static R adaptWork(worker<R, T>& w); };

    template <typename T> struct workerAdapter<void, T> {
        static void adaptWork(worker<void, T>& w);
    };

#define TEMPLATE template <typename R, typename T>
#define ME worker<R, T>

    TEMPLATE ME::worker() { _rel(); }

    TEMPLATE errReport& ME::getReport() { return *_rpt; }

    TEMPLATE
    ME& ME::setReport(errReport& rpt) {
        _rpt.bind(rpt);
        return *this;
    }

    TEMPLATE
    ME& ME::setFlag(nint newFlag) {
        _logFlag = newFlag;
        return *this;
    }

    TEMPLATE
    ME& ME::addFlag(nint newFlag) {
        _logFlag |= newFlag;
        return *this;
    }

    TEMPLATE
    ME& ME::delFlag(nint clear) {
        _logFlag &= ~clear;
        return *this;
    }

    TEMPLATE nbool ME::isFlag(nint flag) const { return (_logFlag & flag) == flag; }

    TEMPLATE nint ME::getFlag() const { return _logFlag; }

    TEMPLATE
    ME& ME::setTask(const T& task) {
        _task.bind(task);
        return *this;
    }

    TEMPLATE T& ME::getTask() { return *_task; }

    TEMPLATE void ME::rel() { _rel(); }

    TEMPLATE
    void ME::_rel() {
        _rpt.bind(dummyErrReport::singletone);
        _task.rel();
        _logFlag = 0;
    }

    TEMPLATE
    void ME::_prepare() { _area.rel(); }

    TEMPLATE
    void ME::_report(err* e) {
        _rpt->add(e);
        if(isFlag(LOG_ON_EX)) {
            enablesZone zone(true);
            e->log();
        } else if(isFlag(DUMP_ON_EX)) {
            enablesZone zone(true);
            e->dump();
        }
    }

    TEMPLATE area& ME::_getArea() { return _area; }

    TEMPLATE nbool ME::isOk() const { return !_rpt->hasErr(); }

    TEMPLATE
    void ME::_onEndWork() {
        _area.rel();
        _onEndErrReport(*_rpt);
    }

    TEMPLATE
    R ME::work() { return workerAdapter<R, T>::adaptWork(*this); }

    TEMPLATE void ME::_setTask(const T& new1) { _task.bind(new1); }

    TEMPLATE void ME::_setTask(const T* new1) { _setTask(*new1); }

    TEMPLATE
    void ME::_onEndErrReport(const errReport& rpt) const {
        if(!isFlag(DUMP_ON_END | LOG_ON_END))

            if(!rpt) return; // ## print errors.
        NM_I("errors:");
        if(isFlag(DUMP_ON_END)) return rpt.dump(), void();
        if(isFlag(LOG_ON_END)) rpt.log();
    }

#undef ME
#define ME workerAdapter<R, T>

    TEMPLATE
    R ME::adaptWork(worker<R, T>& w) {
        if(w.isFlag(worker<R, T>::GUARD)) NM_I("|=== %s.work()... ==============|", w);

        w._prepare();

        R ret;
        enablesZone internal;
        if(!w.isFlag(worker<R, T>::INTERNAL)) internal.setEnable(false);
        ret = w._onWork();
        internal.setPrev().rel();

        if(w.isFlag(worker<R, T>::GUARD)) NM_I("|--- %s._onEndWork()... --------|", w);

        w._onEndWork();

        if(w.isFlag(worker<R, T>::GUARD)) NM_I("|=== %s ends! ==================|", w);
        return ret;
    }

#undef ME
#undef TEMPLATE
#define TEMPLATE template <typename T>
#define ME workerAdapter<void, T>

    TEMPLATE
    void ME::adaptWork(worker<void, T>& w) {
        if(w.isFlag(worker<void, T>::GUARD)) NM_I("|=== %s.work()... ==============|", w);

        w._prepare();

        enablesZone prev;
        {
            enablesZone internal;
            if(!w.isFlag(worker<void, T>::INTERNAL)) logger::get().setEnable(false);
            w._onWork();
        }

        if(w.isFlag(worker<void, T>::GUARD)) NM_I("|--- %s._onEndWork()... --------|", w);

        prev.setPrev();
        w._onEndWork();

        if(w.isFlag(worker<void, T>::GUARD)) NM_I("|=== %s ends! ==================|", w);
    }

#undef ME
#undef TEMPLATE
} // namespace nm
