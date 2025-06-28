#pragma once

#include "core/builtin/container/mgd/tarr.hpp"
#include "core/builtin/container/native/tnmap.hpp"
#include "core/builtin/err/errReport.hpp"
#include "core/frame/frames.hpp"

struct exprTest;

namespace nm {

    class slotLoader;

    class _nout thread: public node, public dumpable {
        NM(CLASS(thread, node))
        friend class verifier;  // for frames
        friend class baseObj;   // for frames.
        friend class func;      // for frames.
        friend class blockExpr; // for frames.
        friend class defPropExpr;
        friend class defVarExpr;
        friend class defNestedFuncExpr;
        friend class assignExpr;
        friend class verifier;
        friend class runExpr;
        friend class baseFunc; // for frames.
        friend class forExpr;
        friend class whileExpr;
        friend class retStateExpr;
        friend class starter;
        friend struct ::exprTest;
        friend class loopExpr;
        friend class closure; // for _getFrames()

    public:
        thread();
        thread(const errReport& new1);

    public:
        const frames& getFrames() const NM_CONST_FUNC(_getFrames())
        const frame* getNowFrame() const NM_CONST_FUNC(_getNowFrame())

        static thread& get();
        static void set(thread* new1);
        static void set(thread& new1);
        static const instancer* getInstancer();

        /// @return exception errReport instance.
        ///         what this contains are all runtime err and exceptions.
        errReport& getEx();
        const errReport& getEx() const NM_CONST_FUNC(getEx())
        virtual void setEx(const errReport& new1);
        void setEx(const errReport* it) NM_SIDE_FUNC(setEx);

        // node:
        scope& subs() override;

        using super::prioritize;
        priorType prioritize(const args& a) const override;

        using super::run;
        str run(const args& a) override;

        void rel() override;

        /// @return slot instances loaded by internal system.
        ///         you can cast these to 'slot' types.
        const nmap& getSlots() const;

        void dump() const override;

        virtual nbool isInteractable() const;

    protected:
        virtual frames& _getFrames();
        frame* _getNowFrame();

    private:
        void _loadBuiltIns(nmap& tray) const;
        tstr<nmap> _initSlots() const;

    private:
        frames _frames;
        tstr<errReport> _ex;
    };
} // namespace nm
