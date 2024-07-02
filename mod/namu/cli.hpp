#pragma once

#include "flag/flags.hpp"

namespace nm {

    struct cli {
        const flags& getFlags() const;

        nbool run(flagArgs& a);

        nint getRes() const;

    private:
        nint _res;
    };
}
