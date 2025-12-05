/// @file
#pragma once

#include "core/builtin/scalar/nStr.hpp"
#include "core/builtin/err/baseErr.hpp"

namespace by {
    /// @ingroup core
    /// @brief Standard error implementation
    /// @details Concrete error class with message support for runtime errors and exceptions.
    class _nout err: public baseErr {
        BY(CLASS(err, baseErr))
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
