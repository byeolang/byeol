#pragma once

#include "indep/common.hpp"
#include "indep/macro/namuMeta.hpp"
#include "indep/macro/declThis.hpp"
#include "indep/macro/unconstFunc.hpp"
#include <optional>

namespace nm {

    /// this is a wrapper class for std::optional.
    /// improved `optional`'s func name and signature because they are somewhat inconsistent
    /// from the perspective of namu and convention rules.
    ///
    /// in namu, there are two cases where data must be returned with an error.
    ///     1. When returning by ref: use a binder such as tstr<T> or tweak<T>.
    ///     2. When returning by value: use tmay<T>.
    ///
    /// for details, refer to the return type rules of namu.
    template <typename T> class tmay {
        NM(ME(tmay))

    public:
        tmay() = default;

        explicit tmay(const T& value);

    public:
        T* operator->();

        const T* operator->() const NM_CONST_FUNC(operator->());

        T& operator*();

        const T& operator*() const NM_CONST_FUNC(operator*());

        operator nbool() const;

    public:
        nbool has() const;

        T& get();
        const T& get() const NM_CONST_FUNC(get())

        virtual void rel();

        virtual void set(const T& arg);

    private:
        std::optional<T> _subj;
    };
} // namespace nm
