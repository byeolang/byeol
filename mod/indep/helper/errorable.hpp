#pragma once

#include "indep/helper/dumpable.hpp"
#include "indep/helper/errLv.hpp"

namespace nm {

    class _nout errorable: public dumpable {
    public:
        virtual void log() const = 0;
        void dbgLog() const;
        virtual void logStack() const = 0;
        virtual const std::string& getMsg() const = 0;
        virtual errLv::level getLv() const = 0;
        const std::string& getLevelName() const; // { return errLv::getName(_lv); }
    };
}
