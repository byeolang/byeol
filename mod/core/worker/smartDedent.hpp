#pragma once

#include "../type/ntype.hpp"

namespace nm {
    class _nout smartDedent: public typeProvidable, public clonable {
        NM(CLASS(smartDedent))

        enum Status {
            OFF = -1,
            CAUGHT = 0,
            ON = 1,
        };

    public:
        smartDedent();

    public:
        void countDown();
        void countUp();
        void setEnable(nbool to);
        void setDedentNow();
        nbool canDedent() const;
        nbool isOn() const;

    private:
        nint _cnt;
    };
}
