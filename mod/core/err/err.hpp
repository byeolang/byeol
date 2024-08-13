#pragma once

#include "baseErr.hpp"
#include "builtin/primitive/nStr.hpp"

namespace nm {
    class err : public baseErr {
        NM(CLASS(err, baseErr))
        template <typename T, nbool> friend struct tmarshaling;

    public:
        err(const nStr& msg);
    private:
        err();

    public:
        nbool operator==(const super& rhs) const override;

    public:
        using super::subs;
        scope& subs() override;

        const baseObj& getOrigin() const override;
        void log() const override;
        const std::string& getMsg() const override;

    private:
        tstr<nStr> _msg;
    };
}
