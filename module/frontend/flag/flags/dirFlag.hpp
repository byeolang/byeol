#pragma once

#include "frontend/flag/flag.hpp"

namespace by {
    class dirFlag : public flag {
        BY(CLASS(dirFlag, flag))

    public:
        const nchar* getName() const override;
        const nchar* getDescription() const override;

    protected:
        const strings& _getRegExpr() const override;
        res _onTake(const flagArgs& tray, cli& c, interpreter& ip, starter& s, errReport& rpt) const override;
    };
}
