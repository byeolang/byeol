#pragma once

#include "frontend/flag/flag.hpp"

namespace by {

    /// flag which takes one flag value.
    /// e.g. '-o fast'
    ///      in above example, '-v' is flag, 'fast' is value.
    struct singleFlag: public flag {
        // TODO:
    };
}
