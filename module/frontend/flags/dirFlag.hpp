#pragma once

#include "flagStacker/flag/flag.hpp"
#include "frontend/common/dep.hpp"

namespace by {
    struct cli;

    class dirFlag: public flag {
        BY(ME(dirFlag, flag))

    public:
        dirFlag(cli& c);

    public:
        const nchar* getName() const override;
        const nchar* getDescription() const override;

    protected:
        const strings& _getRegExpr() const override;
        res _onTake(const flagArgs& tray) const override;

    private:
        cli& _cli;
    };
}
