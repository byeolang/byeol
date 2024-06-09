#pragma once

#include "../builtin/container/mgd/tarr.hpp"
#include "../builtin/container/native/tnmap.hpp"
#include "frames.hpp"
#include "../loader/errReport.hpp"

struct exprTest;

namespace namu {

    class slotLoader;
    class _nout thread : public node, public dumpable {
        NAMU(CLASS(thread, node))
        friend class verifier; // for frames
        friend class baseObj; // for frames.
        friend class func; // for frames.
        friend class blockExpr; // for frames.
        friend class defPropExpr; // for frames
        friend class defAssignExpr;
        friend class assignExpr;
        friend class verifier;
        friend class runExpr;
        friend class baseFunc; // for frames.
        friend class forExpr;
        friend class whileExpr;
        friend class retStateExpr;
        friend class starter;
        friend struct ::exprTest;

    public:
        thread();
        thread(const errReport& new1);

    public:
        const frames& getFrames() const NAMU_UNCONST_FUNC(_getFrames())
        const frame& getNowFrame() const NAMU_UNCONST_FUNC(_getNowFrame())

        static thread& get();
        static void set(thread* new1);
        static void set(thread& new1);
        static void set();
        static const instancer& getInstancer();

        /// @return exception errReport instance.
        ///         what this contains are all runtime err and exceptions.
        errReport& getEx();
        const errReport& getEx() const NAMU_UNCONST_FUNC(getEx())
        virtual void setEx(const errReport& new1);

        // node:
        scope& subs() override;

        priorType prioritize(const args& a) const override;
        str run(const args& a) override;

        void rel() override;

        /// @return slot instances loaded by internal system.
        ///         you can cast these to 'slot' types.
        const nmap& getSlots() const;

        void dump() const override;

        virtual nbool isInteractable() const;

    protected:
        virtual frames& _getFrames();
        frame& _getNowFrame();

    private:
        void _loadBuiltIns(nmap& tray) const;
        tstr<nmap> _initSlots() const;

    private:
        thread_local static thread* _instance;
        frames _frames;
        tstr<errReport> _ex;
    };
}
