/// @file
#pragma once

#include "frontend/flag/flag.hpp"

namespace by {

    class verFlag: public flag {
        BY(CLASS(verFlag, flag))

    public:
        const nchar* getName() const override;
        const nchar* getDescription() const override;

    protected:
        const strings& _getRegExpr() const override;
        res _onTake(const flagArgs& tray, cli& c, interpreter& ip, starter& s) const override;
    };
}
