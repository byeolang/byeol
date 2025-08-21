/// @file
#pragma once

#include "leaf/common.hpp"

namespace by {
    class _nout leafSmartDedent {
        BY(ME(leafSmartDedent))

        enum Status {
            OFF = -1,
            CAUGHT = 0,
            ON = 1,
        };

    public:
        leafSmartDedent();

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
