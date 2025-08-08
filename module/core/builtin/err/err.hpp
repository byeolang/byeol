#pragma once

#include "core/builtin/primitive/nStr.hpp"
#include "core/builtin/err/baseErr.hpp"

namespace by {
    class _nout err: public baseErr {
        NM(CLASS(err, baseErr))
        template <typename T, nbool> friend struct tmarshaling;

    public:
        err(const nStr& msg);
        err();

    public:
        nbool operator==(const super& rhs) const override;

    public:
        void log() const override;
        const std::string& getMsg() const override;

        const baseObj& getOrigin() const override;

    private:
        tstr<nStr> _msg;
    };
}
