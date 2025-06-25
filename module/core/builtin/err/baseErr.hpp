#pragma once

#include "core/ast/baseObj.hpp"
#include "core/frame/callstack.hpp"

namespace nm {
    class _nout baseErr: public baseObj, public errorable {
        NM(ADT(baseErr, baseObj))
        template <typename T, nbool> friend struct tmarshaling;

    public:
        baseErr(errLv::level t);
        baseErr(const baseErr& rhs);

    public:
        me& operator=(const me& rhs);
        virtual nbool operator==(const me& rhs) const = 0;
        nbool operator!=(const me& rhs) const;

    public:
        const callstack& getStack() const;
        void logStack() const override;
        void dump() const override;
        errLv::level getLv() const override;
        static scope& makeSubs();
        const std::string& getMsg() const override;

    private:
        void _initStack();
        me& _assign(const me& rhs);

    private:
        tstr<callstack> _stack;
        errLv::level _lv;
    };
} // namespace nm
