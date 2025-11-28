/// @file
#pragma once

#include <regex>

#include "frontend/common.hpp"
#include "frontend/flag/flagArgs.hpp"

namespace by {

    struct cli;

    typedef std::vector<std::string> strings;

    class flag: public instance {
        BY(ADT(flag, instance))

    public:
        enum res {
            MATCH,
            EXIT_PROGRAM,
            NOT_MATCH
        };

    public:
        virtual const nchar* getName() const = 0;
        virtual const nchar* getDescription() const = 0;

        /// @return false if flag wants to exit program.
        virtual res take(interpreter& ip, starter& s, cli& c, flagArgs& a) const;

    protected:
        /// when this flag matched to one of args, the value of returning this func will be
        /// count of continuous argument of this flag.
        virtual ncnt getArgCount() const;

        void _delArgs(flagArgs& a, std::vector<int> indices) const;

        virtual const strings& _getRegExpr() const = 0;
        virtual res _onTake(const flagArgs& tray, cli& c, interpreter& ip, starter& s) const = 0;
    };

    typedef std::vector<tstr<flag>> flags;
}
