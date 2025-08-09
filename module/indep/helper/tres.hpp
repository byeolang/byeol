#pragma once

#include "tmay.hpp"

namespace by {

    template <typename T, typename R> class tres: public tmay<T> {
        typedef tres<T, R> __me__;
        BY(ME(__me__, tmay<T>))

    public:
        tres() = default;
        explicit tres(const T& value);
        explicit tres(const R& value);

    public:
        R& getErr();
        const R& getErr() const BY_CONST_FUNC(getErr())

        void rel() override;

        void set(const T& arg) override;
        void setErr(const R& arg);

    private:
        std::optional<R> _err;
    };
}
