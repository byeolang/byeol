/// @file
#pragma once

#include "indep/macro/byeolMeta.hpp"
#include "indep/macro/declThis.hpp"
#include "indep/macro/unconstFunc.hpp"
#include "indep/helper/typeTrait.hpp"
#include "indep/helper/tmedium.inl"
#include <optional>

namespace by {

    /** @ingroup indep
     *  @brief Optional value wrapper for error indication without exceptions
     *  @details Provides a way to indicate errors in return-by-value functions without
     *  using slow exceptions. Functionally almost identical to std::optional<T>, but with
     *  API naming adapted to byeol project conventions.
     *
     *  For normal operation, pass the value as T& to the constructor. For error situations,
     *  pass a tmedium class created as nullptr.
     *
     *  @remark Why use tmedium?
     *  See @ref tmedium for details on why this intermediate type is used.
     *
     *
     *  @remark Common naming conventions
     *  The functions `has()`, `get()`, `rel()`, and `set()` follow naming conventions used
     *  throughout the entire project.
     *
     *  @section usage Usage
     *  Example of using tmay for error handling:
     *  @code
     *      tmay<std::string> loadConfig(const std::string& path) {
     *          if(!fileExists(path))
     *              return tmay<std::string>(); // Return empty for error
     *
     *          std::string content = readFile(path);
     *          return tmay<std::string>(content); // Return value for success
     *      }
     *
     *      auto config = loadConfig("config.txt");
     *      if(config.has()) {
     *          std::string& value = config.get();
     *          processConfig(value);
     *      }
     *  @endcode
     *
     *  @see Use tstr<T> or tweak<T> for return-by-reference cases
     */
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
        /**
         * @brief Gets the contained value.
         * @warning Calling get() on an empty tmay instance results in undefined behavior.
         *          It is recommended to check with has() before calling this.
         * @return Pointer to the contained value.
         */
        T* get();
        const T* get() const BY_CONST_FUNC(get())
        /**
         * @brief Releases/clears the contained value, making the instance empty.
         */
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
