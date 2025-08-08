#pragma once

#include "frontend/flag/flags.hpp"

namespace by {

    struct cli {
        const flags& getFlags() const;

        nbool run(flagArgs& a);

        nint getRes() const;

    private:
        nint _res;
    };
}
