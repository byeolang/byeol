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
    /// as with std::optional, calling get() on an object that doesn't actually have a value
    /// will cause the app to crash.
    ///
    /// in namu, there are two cases where data must be returned with an error.
    ///     1. When returning by ref: use a binder such as tstr<T> or tweak<T>.
    ///     2. When returning by value: use tmay<T>.
    ///
    /// for details, refer to the return type rules of namu.
    template <typename T> class tmay: private std::optional<T> {
        NM(ME(tmay, std::optional<T>))

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
    };

    // func for OR macro:
    template <typename T, typename F> const T& operator|(tmay<T>& t, F&& f) {
        f(t);
        // this may return null-reference but take it easy.
        // it'll never be used.
        return *t;
    }

    template <typename T, typename F> const T& operator|(const tmay<T>& t, F&& f) {
        f(t);
        // this may return null-reference but take it easy.
        // it'll never be used.
        return *t;
    }

} // namespace nm
