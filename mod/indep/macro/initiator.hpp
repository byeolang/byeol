#pragma once

#define NM_INITIATOR(name, body) \
    static inline Initiator* NM_CONCAT(__initi__ ## name ##_, __COUNTER__) = new Initiator([]() body);

namespace nm {
    /// Initiator can runs statements before main().
    /// @Usage  just @refer NM_INITIATOR(myIniti) { <your codes> };
    ///         static<optional> Initiator <name>([]() { <your codes> });
    struct _nout Initiator {
        template <typename T>
        Initiator(T func) { func(); }
    };
}

