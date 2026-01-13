/** @file
 *  Pre-main initialization macros and utilities
 */
#pragma once

#define BY_INITIATOR(name, body) \
    static inline Initiator* BY_CONCAT(__initi__##name##_, __COUNTER__) = new Initiator([]() body);

namespace by {
    /** @ingroup indep
     *  @brief Utility for executing code before main() function
     *  @details Initiator can runs statements before main().
     *  Usage: just use BY_INITIATOR(myIniti) { <your codes> };
     *         or static<optional> Initiator <name>([]() { <your codes> });
     */
    struct _nout Initiator {
        template <typename T> Initiator(T func) { func(); }
    };
}
