#pragma once

#include "../../ast/baseObj.hpp"
#include "../../type/dumpable.hpp"
#include "../../frame/callstack.hpp"

namespace nm {
    class _nout baseErr: public baseObj, public dumpable {
        NM(ADT(baseErr, baseObj))
        template <typename T, nbool> friend struct tmarshaling;

    public:
        baseErr(logLv::level t, const baseObj& org);
        baseErr(const baseErr& rhs);

    protected:
        baseErr(const baseObj& org, nbool);

    public:
        me& operator=(const me& rhs);
        virtual nbool operator==(const me& rhs) const = 0;
        nbool operator!=(const me& rhs) const;

    public:
        virtual const std::string& getMsg() const = 0;

        virtual void log() const = 0;
        void dbgLog() const;
        const callstack& getStack() const;
        virtual void logStack() const;
        void dump() const override;
        const std::string& getLevelName() const;
        logLv::level getLv() const;
        static scope& makeSubs();

    private:
        void _initStack();
        me& _assign(const me& rhs);

    private:
        tstr<callstack> _stack;
        logLv::level _lv;
    };
} // namespace nm
