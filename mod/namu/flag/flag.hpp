#pragma once

#include "flagArgs.hpp"
#include "../common.hpp"
#include <regex>

namespace nm {

    struct cli;
    class flag : public instance {
        NAMU(ADT(flag, instance))

    public:
        virtual const nchar* getName() const = 0;
        virtual const nchar* getDescription() const = 0;

        /// @return how many argument taken by this flag.
        virtual nbool take(interpreter& ip, starter& s, cli& c, flagArgs& a) const;

    protected:
        /// when this flag matched to one of args, the value of returning this func will be
        /// count of continuous argument of this flag.
        virtual ncnt _onContinuousArgCount() const;

        void _delArgs(flagArgs& a, std::vector<int> indices) const;

        virtual const nchar* _getRegExpr() const = 0;
        virtual nbool _onTake(const flagArgs& tray, cli& c, interpreter& ip, starter& s) const = 0;
    };

    typedef std::vector<tstr<flag>> flags;
}
