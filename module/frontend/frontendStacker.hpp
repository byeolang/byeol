#pragma once

#include "flagStacker.hpp"
#include "frontend/common/dep.hpp"

namespace by {
    class frontendStacker: public stacker {
        BY(ME(frontendStacker, stacker))

    public:
        frontendStacker(cli& c);

    protected:
        void _initFlags(flags& tray) const override;

    private:
        cli& _cli;
    };
}
