#pragma once

#define BY_INITIATOR(name, body) \
    static inline Initiator* BY_CONCAT(__initi__##name##_, __COUNTER__) = new Initiator([]() body);

namespace by {
    /// Initiator can runs statements before main().
    /// @Usage  just @refer BY_INITIATOR(myIniti) { <your codes> };
    ///         static<optional> Initiator <name>([]() { <your codes> });
    struct _nout Initiator {
        template <typename T> Initiator(T func) { func(); }
    };
}
