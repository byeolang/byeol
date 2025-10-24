/// @file
#pragma once

#include "core/type/ctorType.hpp"

namespace by {

    /// @ingroup core
    /// @brief Factory for creating managed types
    /// @details Template-based factory for generating type objects with proper metadata.
    class ctor;
    class _nout typeMaker {
        BY(ME(typeMaker))

    private:
        static mgdType _make(void*, const std::string& name, const type& super, const params& ps, nbool isAdt,
            const node* ret) {
            return mgdType(name, super, ps, isAdt, ret);
        }

        static funcType _make(baseFunc*, const std::string& name, const type& super, const params& ps, nbool isAdt,
            const node* ret) {
            return funcType(name, super, ps, isAdt, ret);
        }

        static ctorType _make(ctor*, const std::string&, const type& super, const params& ps, nbool isAdt,
            const node* ret) {
            return ctorType(super, ps, ret);
        }

    public:
        template <typename T> static auto make(const params& ps, const node* ret) {
            return make<T>(ttype<T>::get().getName(), ps, ret);
        }

        template <typename T> static auto make(const std::string& name) { return make<T>(name, params(), nullptr); }

        template <typename T>
        static auto make(const std::string& name, const params& ps, const node* ret)
            -> decltype(_make((T*) nullptr, name, ttype<T>::get(), ps, !std::is_constructible<T>::value, ret)) {
            return _make((T*) nullptr, name, ttype<T>::get(), ps, !std::is_constructible<T>::value, ret);
        }
    };
}
