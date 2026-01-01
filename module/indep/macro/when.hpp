/// @file
/// Conditional compilation and execution macros
#pragma once

#include <utility>
#include <cstdlib>
#include "indep/def/_nout.hpp"
#include "indep/common/typedef.hpp"
#include "indep/helper/typeTrait.hpp"
#include "indep/macro/declThis.hpp"
#include "indep/macro/byeolMeta.hpp"
#include "indep/helper/tmay.hpp"

namespace by {

    /// @ingroup indep
    /// @brief Early-return pattern macro for exception handling
    /// @details The byeol project actively applies the early-return pattern throughout.
    /// This helps reduce code depth, improve code flow clarity, and handle exceptional
    /// situations immediately. However, traditional `if` statements make it difficult
    /// to distinguish between normal branching logic and early-return exception handling.
    ///
    /// @section Problem
    /// Consider this traditional early-return code:
    /// @code
    ///     str me::eval(const args& a) {
    ///         std::string key = _makeKey(a);
    ///         if(key.empty()) {
    ///             BY_E("key is empty");
    ///             return tstr<obj>();
    ///         }
    ///         if(_isSelfMaking(key)) {
    ///             BY_E("error: you tried to clone self generic object.");
    ///             return tstr<obj>();
    ///         }
    ///
    ///         if(!_cache.count(key))
    ///             _makeGeneric(key, params::make(_paramNames, a));
    ///
    ///         return _cache[key];
    ///     }
    /// @endcode
    ///
    /// The WHEN macro solves this by explicitly marking early-return cases. It is used
    /// exclusively for early-return patterns. Additionally, since over 90% of early-returns
    /// involve logging an error and returning an error value, WHEN supports chaining to
    /// express both operations in a single line.
    ///
    /// @section Solution
    /// The same code becomes much clearer with WHEN:
    /// @code
    ///     str me::eval(const args& a) {
    ///         std::string key = _makeKey(a);
    ///         WHEN(key.empty()).err("key is empty").ret(tstr<obj>());
    ///         WHEN(_isSelfMaking(key))
    ///             .err("error: you tried to clone self generic object.")
    ///             .ret(tstr<obj>());
    ///
    ///         if(!_cache.count(key)) _makeGeneric(key, params::make(_paramNames, a));
    ///         return _cache[key];
    ///     }
    /// @endcode
    ///
    /// Now the purpose of each `if` is clear, and exception handling is visually distinct
    /// from normal branching logic.
    ///
    /// @remark WHEN macro is used very frequently throughout the project, so it's
    /// important to understand it well.
    ///
    ///
    /// @remark __WHEN_OBJECT__ customization
    /// Since byeol uses a multi-layered architecture, different layers may need different
    /// behavior when WHEN conditions are met. Low-level layers simply output logs to the
    /// screen, but high-level layers require more complex processing like creating exception
    /// objects with stacktrace information. This is solved by redefining `__WHEN_OBJECT__`
    /// in each layer.

#define __WHEN_OBJECT__ __indep_when__

    class _nout __WHEN_OBJECT__ {
        BY(ME(__WHEN_OBJECT__))

    public:
        static const me& get();

        template <typename R> R& ret([[maybe_unused]] R& r) const { return r; }

        template <typename R> R* ret([[maybe_unused]] R* r) const { return r; }

        template <typename R> R&& ret([[maybe_unused]] R&& r) const { return std::move(r); }

        void ret() const;

        template <typename T> tmay<T> retMay() const { return tmay<T>(); }

        template <typename T, typename... Ts> tmay<T> retMay(Ts... args) const { return tmay<T>(args...); }

        void crash() const;

        template <typename R> R& crash([[maybe_unused]] R& r) const {
            abort();
            return r;
        }

        template <typename R> R&& crash([[maybe_unused]] R&& r) const {
            abort();
            return std::move(r);
        }
    };

#define BY_WHEN ::by::__WHEN_OBJECT__::get()

#define __WHEN_POSTFIX__ return BY_WHEN
#define WHEN(condition) \
    if(condition) __WHEN_POSTFIX__
#define WHEN_NUL_1(v1) \
    if(nul(v1)) __WHEN_POSTFIX__
#define WHEN_NUL_2(v1, v2) \
    if(nul(v1) || nul(v2)) __WHEN_POSTFIX__
#define WHEN_NUL_3(v1, v2, v3) \
    if(nul(v1) || nul(v2) || nul(v3)) __WHEN_POSTFIX__
#define WHEN_NUL_4(v1, v2, v3, v4) \
    if(nul(v1) || nul(v2) || nul(v3) || nul(v4)) __WHEN_POSTFIX__
#define WHEN_NUL_5(v1, v2, v3, v4, v5) \
    if(nul(v1) || nul(v2) || nul(v3) || nul(v4) || nul(v5)) __WHEN_POSTFIX__
#define WHEN_NUL_6(v1, v2, v3, v4, v5, v6) \
    if(nul(v1) || nul(v2) || nul(v3) || nul(v4) || nul(v5) || nul(v6)) __WHEN_POSTFIX__
#define WHEN_NUL(...) BY_OVERLOAD(WHEN_NUL, __VA_ARGS__)

} // namespace by
