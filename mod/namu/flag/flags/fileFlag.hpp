#pragma once

#include "../flag.hpp"

namespace nm {

    class fileFlag: public flag {
        NM(CLASS(fileFlag, flag))

    public:
        const nchar* getName() const override;

        const nchar* getDescription() const override;

    protected:
        const strings& _getRegExpr() const override;

        res _onTake(const flagArgs& tray, cli& c, interpreter& ip, starter& s) const override;
    };
}
