/// @file
#pragma once

#include "core/builtin/container/mgd/tarr.hpp"
#include "core/builtin/container/native/tnmap.hpp"
#include "core/builtin/err/errReport.hpp"
#include "core/frame/frames.hpp"

struct exprTest;

namespace by {

    class podLoader;

    /** @ingroup core
     *  @brief Thread execution context for byeol runtime
     *  @details Represents a single program execution flow. Currently only supports single threading. thread has @ref
     *  frames and @ref errReport, allowing it to construct @ref frame when executing a program and collect @ref err
     *  that occurs during execution.
     *
     *  @section builtin Builtin
     *  The builtin pod consists of types/functions provided by default in the byeol language. They are always
     *  accessible without separate manifest declaration. It's similar to the standard pod, but this is a distinctly
     *  separate pod written in byeol code. However, the builtin pod is entirely in the form of exposing C++ native
     *  classes using @ref tbridger. The types included here are only the most basic symbols like `int`, `str`, `arr`,
     *  `err`, `print()`.
     *
     *  @section singleton_property Singleton Property
     *  thread provides `static thread& get()`. Through this, @ref scope or @ref expr can access the currently active
     *  thread object. When not using the default thread and creating a thread instance directly to run a program, it's
     *  necessary to register the thread object and replace it with the original thread instance when the program ends.
     *  To make this easier, use @ref threadUse.
     *
     *  @code
     *      if(main.canEval(a)) {
     *          threadUse thr(getReport());
     *
     *          // doSomething() with new thread...
     *      }
     *      // When exiting, it's replaced with the original thread instance.
     *      // The program execution state before entering the block is restored.
     *  @endcode
     *
     *  As shown in the above example, threadUse automatically creates a thread object internally upon creation.
     */
    class _nout thread: public node, public dumpable {
        BY(CLASS(thread, node))
        friend class coreInternal;
        friend struct ::exprTest;

    public:
        thread();
        /**
         * @brief Constructs a thread object with a custom error report instance.
         * @param new1 The `errReport` instance to be used by this thread for error collection.
         */
        thread(const errReport& new1);

    public:
        const frames& getFrames() const BY_CONST_FUNC(_getFrames())
        const frame* getNowFrame() const BY_CONST_FUNC(_getNowFrame())

        static thread& get();
        static void set(thread* new1);
        static void set(thread& new1);
        static const instancer* getInstancer();

        /**
         *  @return exception errReport instance.
         *          what this contains are all runtime err and exceptions.
         */
        errReport& getEx();
        const errReport& getEx() const BY_CONST_FUNC(getEx())
        /**
         * @brief Sets the error report instance for this thread.
         * @param new1 The `errReport` instance to be used by this thread.
         */
        virtual void setEx(const errReport& new1);
        void setEx(const errReport* it) BY_SIDE_FUNC(setEx);

        // node:
        scope& subs() override;

        using super::prioritize;
        priorType prioritize(const args& a) const override;

        using super::eval;
        str eval(const args& a) override;

        void rel() override;

        /**
         *  @return pod instances loaded by internal system.
         *          you can cast these to 'pod' types.
         */
        const nmap& getPods() const;

        void dump() const override;

        /**
         * @brief Checks if the thread is currently in an interactable state.
         * @return true if the thread is interactable, false otherwise.
         * @details An interactable thread typically means it's ready to process user input
         *          or continue execution.
         */
        virtual nbool isInteractable() const;

    protected:
        virtual frames& _getFrames();
        frame* _getNowFrame();

    private:
        void _loadBuiltIns(nmap& tray) const;
        tstr<nmap> _initPods() const;

    private:
        frames _frames;
        mutable tstr<errReport> _ex;
    };
} // namespace by
