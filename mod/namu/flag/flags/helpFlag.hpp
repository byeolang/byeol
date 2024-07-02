#pragma once

#include "../flag.hpp"

namespace nm {

    class helpFlag : public flag {
        NM(CLASS(helpFlag, flag))

    public:
        const nchar* getName() const override;
        const nchar* getDescription() const override;

    protected:
        const nchar* _getRegExpr() const override;
        nbool _onTake(const flagArgs& tray, cli& c, interpreter& ip, starter& s) const override;
        void _printUsage() const;
    };
}
