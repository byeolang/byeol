#pragma once

#include "core/type/ntype.hpp"
#include "core/builtin/err/errReport.hpp"
#include "core/builtin/err/nerr.hpp"

namespace by {

#define myExErr_1(code) exErr(errCode::code, getReport())
#define myExErr_2(me, code) exErr(me, errCode::code, getReport())
#define myExErr_3(me, code, a1) exErr(me, errCode::code, getReport(), a1)
#define myExErr_4(me, code, a1, a2) exErr(me, errCode::code, getReport(), a1, a2)
#define myExErr_5(me, code, a1, a2, a3) exErr(me, errCode::code, getReport(), a1, a2, a3)
#define myExErr(...) BY_OVERLOAD(myExErr, __VA_ARGS__)

#define myExWarn_1(code) exErr(errCode::code, getReport())
#define myExWarn_2(me, code) exErr(me, errCode::code, getReport())
#define myExWarn_3(me, code, a1) exErr(me, errCode::code, getReport(), a1)
#define myExWarn_4(me, code, a1, a2) exErr(me, errCode::code, getReport(), a1, a2)
#define myExWarn_5(me, code, a1, a2, a3) exErr(me, errCode::code, getReport(), a1, a2, a3)
#define myExWarn(...) BY_OVERLOAD(myExWarn, __VA_ARGS__)

    template <typename R, typename T> class worker: public typeProvidable, public clonable {
        BY(ADT(worker))
        template <typename R1, typename T1> friend struct workerAdapter;
        typedef R RetType;

    public:
        enum logFlag {
            LOG_ON_EX = 1,        // logs the err instance when it just got reported.
            DUMP_ON_EX = 1 << 1,  // log and leave callstack from an err instance when it just got reported.
            GUARD = 1 << 2,       // logs when func in, out.
            INTERNAL = 1 << 3,    // logs all except above case.
            LOG_ON_END = 1 << 4,  // log all report info when the work ends.
            DUMP_ON_END = 1 << 5, // log and leave callstack for all report info when the work ends.
#if BY_IS_DBG
            DEFAULT = DUMP_ON_EX | GUARD | INTERNAL | LOG_ON_END
#else
            DEFAULT = LOG_ON_EX
#endif
        };

    public:
        worker();

    public:
        errReport& getReport();
        const errReport& getReport() const BY_CONST_FUNC(getReport())
        me& setReport(errReport& rpt);

        me& setFlag(nint newFlag);
        me& addFlag(nint flag);
        me& delFlag(nint clear);
        nbool isFlag(nint flag) const;
        nint getFlag() const;

        me& setTask(const T& root);
        me& setTask(const T* it) BY_SIDE_FUNC(it, setTask(*it), *this);
        T* getTask();
        const T* getTask() const BY_CONST_FUNC(getTask())

        virtual void rel();

        R work();

        const area& getArea() const BY_CONST_FUNC(_getArea())

        nbool isOk() const;

    protected:
        virtual void _prepare();

        void _report(baseErr* e);

        virtual void _onEndWork();
        virtual R _onWork() = 0;

        area& _getArea();

        void _onEndErrReport(const errReport& rpt) const;

    private:
        void _rel();

    private:
        tstr<errReport> _rpt;
        area _area;
        tstr<T> _task;
        nint _logFlag;
    };
} // namespace by
