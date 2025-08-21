/// @file
#pragma once

#include "indep/macro/byeolMeta.hpp"
#include "indep/macro/declThis.hpp"
#include "indep/macro/unconstFunc.hpp"
#include "indep/helper/typeTrait.hpp"
#include "indep/helper/tmedium.inl"
#include <optional>

namespace by {

    /// @ingroup indep
    /// @brief Optional value wrapper with byeol-style interface
    /// @details Wrapper around std::optional with improved function names and signatures
    /// consistent with byeol conventions. Provides safe value handling for return-by-value
    /// scenarios. Use tstr<T> or tweak<T> for return-by-reference cases.
    template <typename T> class tmay: private std::optional<T> {
        BY(ME(tmay, std::optional<T>))

    public:
        tmay() = default;

        tmay(const T& value);

        tmay(const tmedium<T>& value);

    public:
        T* operator->();
        const T* operator->() const BY_CONST_FUNC(operator->());

        T& operator*();
        const T& operator*() const BY_CONST_FUNC(operator*());

        operator T&();
        operator const T&() const;

        operator nbool() const;

    public:
        nbool has() const;

        T* get();
        const T* get() const BY_CONST_FUNC(get())

        virtual void rel();

        virtual void set(const T& arg);
    };

    // extension for OR macro:
    template <typename T, typename F> tmedium<T> operator|(T* t, F&& f) {
        f(t);
        // this returns null-reference but take it easy.
        // it'll never be used.
        return t;
    }

    template <typename T, typename F> tmedium<T> operator|(const T* t, F&& f) {
        f(t);
        // this may return null-reference but take it easy.
        // it'll never be used.
        return t;
    }

    template <typename T, typename F> T& operator|(tmay<T>& t, F&& f) {
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

    template <typename T, typename F> tmay<T> operator|(tmay<T>&& t, F&& f) {
        f(t);
        // this may return null-reference but take it easy.
        // it'll never be used.
        return t;
    }

    // extension for typeTrait:
    template <typename T> struct typeTrait<tmay<T>> {
        static nbool isNul(const tmay<T>& it) { return !it.has(); }

        static constexpr nbool is_ptr = false;
        static constexpr nbool is_ref = false;
        static constexpr nbool is_like_ptr = true;
    };

    template <typename T> struct typeTrait<tmay<T>&> {
        static nbool isNul(const tmay<T>& it) { return !it.has(); }

        static constexpr nbool is_ptr = false;
        static constexpr nbool is_ref = true;
        static constexpr nbool is_like_ptr = true;
    };

    // extension for TO macro:
    template <typename T, typename F>
    auto operator->*(tmay<T>& t, F&& f) -> decltype(typeTrait<decltype(f(*t))>::ret()) {
        return t ? f(*t) : typeTrait<decltype(f(*t))>::ret();
    }

    template <typename T, typename F>
    auto operator->*(const tmay<T>& t, F&& f) -> decltype(typeTrait<decltype(f(*t))>::ret()) {
        return t ? f(*t) : typeTrait<decltype(f(*t))>::ret();
    }

    template <typename T, typename F>
    auto operator->*(tmay<T>&& t, F&& f) -> decltype(typeTrait<decltype(f(*t))>::ret()) {
        return t ? f(*t) : typeTrait<decltype(f(*t))>::ret();
    }

} // namespace by
