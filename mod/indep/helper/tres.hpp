#pragma once

#include "tmay.hpp"

namespace nm {

    template <typename T, typename R>
    class tres : public tmay<T> {
        typedef tres<T, R> __me__;
        NM(ME(__me__, tmay<R>))

    public:
        tres() = default;
        explicit tres(const T& value);

    public:
        R& getErr();
        const R& getErr() const NM_CONST_FUNC(getErr())

        void rel() override;

        void set(const T& arg) override;
        void setErr(const R& arg);

    private:
        std::optional<R> _err;
    };
}
