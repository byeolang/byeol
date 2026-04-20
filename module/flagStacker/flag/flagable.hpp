#pragma once

#include "flagStacker/common/typedef.hpp"
#include "flagStacker/flag/flagArgs.hpp"

namespace by {
    class stacker;

    class _nout flagable {
    public:
        enum res {
            MATCH,
            EXIT_PROGRAM,
            NOT_MATCH
        };

    public:
        virtual res take(flagArgs& a, stacker& s) = 0;

        virtual nbool canTake(const std::string& pattern) const = 0;
    };
}
