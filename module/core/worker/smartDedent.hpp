/// @file
#pragma once

#include "core/type/ntype.hpp"

namespace by {
    class _nout smartDedent: public typeProvidable, public clonable {
        BY(CLASS(smartDedent))

        enum Status {
            OFF = -1,
            CAUGHT = 0,
            ON = 1,
        };

    public:
        smartDedent();

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
