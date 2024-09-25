#pragma once

#include "func.hpp"

namespace nm {
    /// nested func:
    ///     when you eval nested func, actually it will clone a new func with the current
    ///     shallow copied block scope. so it can captures local scope at that time.
    ///
    ///     it doesn't know about local variables after you define it.
    ///     it contains copied block scope. and will inject just before runs block stmt.
}
