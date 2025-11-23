/// @file
#pragma once

#include "stela/common.hpp"

namespace by {
    /// @ingroup stela
    /// @brief Smart dedent handler for indentation-based parsing
    /// @details Manages automatic dedentation in Python-like indentation syntax.
    /// Tracks indentation levels and generates appropriate dedent tokens.
    class _nout stelaSmartDedent {
        BY(ME(stelaSmartDedent))

        enum Status {
            OFF = -1,
            CAUGHT = 0,
            ON = 1,
        };

    public:
        stelaSmartDedent();

    public:
        me& countDown();
        me& countUp();
        me& useLater();
        nint dedent();
        void rel();
        nbool canDedent() const;
        nbool isOn() const;

    private:
        nint _cnt;
        nbool _isEnable;
    };
}
