#pragma once

#include "../../type/ntype.hpp"
#include "../errReport.hpp"

namespace nm {

    template <typename R, typename T>
    class worker : public typeProvidable, public clonable {
        NM(ADT(worker))
        template <typename R1, typename T1>
        friend struct workerAdapter;
        typedef R RetType;

    public:
        enum logFlag {
            LOG_ON_EX = 1, // logs the err instance when it just got reported.
            DUMP_ON_EX = 1 << 1, // log and leave callstack from an err instance when it just got reported.
            GUARD = 1 << 2, // logs when func in, out.
            INTERNAL = 1 << 3, // logs all except above case.
            LOG_ON_END = 1 << 4, // log all report info when the work ends.
            DUMP_ON_END = 1 << 5, // log and leave callstack for all report info when the work ends.
#if NM_IS_DBG
            DEFAULT = DUMP_ON_EX | GUARD | INTERNAL | LOG_ON_END
#else
            DEFAULT = DUMP_ON_EX | LOG_ON_END
#endif
        };

    public:
        worker();

    public:
        errReport& getReport();
        const errReport& getReport() const NM_CONST_FUNC(getReport())
        me& setReport(errReport& rpt);

        me& setFlag(nint newFlag);
        me& addFlag(nint flag);
        me& delFlag(nint clear);
        nbool isFlag(nint flag) const;
        nint getFlag() const;

        me& setTask(const T& root);
        T& getTask();
        const T& getTask() const NM_CONST_FUNC(getTask())

        template <typename... Args> void error(Args... args) { _report(err::newErr(args...)); }
        template <typename... Args> void warn(Args... args) { _report(err::newWarn(args...)); }
        template <typename... Args> void info(Args... args) { _report(err::newInfo(args...)); }

        virtual void rel();

        R work();

        const area& getArea() const NM_CONST_FUNC(_getArea())

        nbool isOk() const;

    protected:
        virtual void _prepare();

        void _report(err* e);

        virtual void _onEndWork();
        virtual R _onWork() = 0;

        area& _getArea();

        void _setTask(const T& new1);
        void _setTask(const T* new1);
        void _onEndErrReport(const errReport& rpt) const;

    private:
        void _rel();

    private:
        tstr<errReport> _rpt;
        area _area;
        tstr<T> _task;
        nint _logFlag;
    };
}
