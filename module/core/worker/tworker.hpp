/// @file
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

#define BY_WORKER_GUARD(msg)                                 \
    BY_I("▶ '%s' %s@%s: " msg, i, me.getType(), &me); \
    line::incLv();                                  \
    _stepN = 0;                                     \
    BY_END_BLOCK({ \
        line::decLv(); \
        BY_I("◀ '%s' %s@%s: " msg, i, me.getType(), &me); \
    });


    /** @ingroup core
     *  @brief Base worker template for processing tasks
     *  @details tworker is optimized for performing large batch operations. It can collect errors that occur during
     *  this process with @ref errReport and can change some operations during work by specifying various flags. Classes
     *  that perform large operations such as @ref verifier, @ref visitor, and @ref parser run based on tworker.
     *
     *  @section work_and_task Work and Task
     *  tworker exists literally to `work()`. At this time, the input that is the target of the work is called `task`.
     *  `_prepare()` is called before work and `_onEndWork()` is called when work ends.
     *
     *  @section err_report errReport
     *  Since large amounts of work must be performed with deep function depth, errReport must be used to sufficiently
     *  detect errors deep in the call stack. tworker can receive an errReport object from outside through
     *  `setReport()`. If errReport is not allocated, @ref dummyErrReport is used instead.
     *
     *  @section log_flag Log Flag
     *  tworker has various flags for logging during work.
     *
     *  * LOG_ON_EX:    Leaves a one-line log when an error occurs
     *  * DUMP_ON_EX:   Dumps the @ref err object including callstack when an error occurs
     *  * GUARD:        Logs when function starts and ends
     *  * INTERNAL:     Logs all logs except errors or GUARD
     *  * LOG_ON_END:   When work is fully completed, leaves a one-line log for all collected errors with `log()`
     *  * DUMP_ON_END:  When work is fully completed, `dump()` all collected errors
     */
    template <typename R, typename T> class tworker: public typeProvidable, public clonable {
        BY(ADT(tworker))
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
        tworker();

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
        /**
         * @brief Protected virtual method for preparation before starting the work.
         * @details This hook allows derived classes to perform any necessary setup or initialization
         *          before `_onWork()` is invoked.
         */
        virtual void _prepare();

        void _report(baseErr* e);

        /**
         * @brief Protected virtual method called after the work is completed.
         * @details This hook allows derived classes to perform any necessary cleanup or
         *          finalization after `_onWork()` has finished.
         */
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
