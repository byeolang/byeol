#pragma once

#define NAMU_INITIATOR(name, body) \
    static inline Initiator* NAMU_CONCAT(__initi__ ## name ##_, __COUNTER__) = new Initiator([]() body);

namespace nm {
    /// Initiator can runs statements before main().
    /// @Usage  just @refer NAMU_INITIATOR(myIniti) { <your codes> };
    ///         static<optional> Initiator <name>([]() { <your codes> });
    struct _nout Initiator {
        template <typename T>
        Initiator(T func) { func(); }
    };
}

