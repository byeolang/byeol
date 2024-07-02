#pragma once

#include "../flag.hpp"

namespace nm {

    class bufferSrcFlag : public flag {
        NM(CLASS(bufferSrcFlag, flag))

    public:
        const nchar* getName() const override;
        const nchar* getDescription() const override;

    protected:
        const nchar* _getRegExpr() const override;
        nbool _onTake(const flagArgs& tray, cli& c, interpreter& ip, starter& s) const override;
        ncnt _onContinuousArgCount() const override;
    };
}
