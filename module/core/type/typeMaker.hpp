/// @file
#pragma once

#include "core/type/funcType.hpp"

namespace by {

    /** @ingroup core
     *  @brief Factory for creating managed types
     *  @details Template-based factory for generating type objects with proper metadata.
     */
    class _nout typeMaker {
        BY(ME(typeMaker))

    private:
        static mgdType _make(void*, const std::string& typeName, const type& super, const params& ps, nbool isAdt,
            const node* ret) {
            return mgdType(typeName, super, ps, isAdt, ret);
        }

        static funcType _make(baseFunc*, const std::string& typeName, const type& super, const params& ps, nbool isAdt,
            const node* ret) {
            return funcType(typeName, super, ps, isAdt, ret);
        }

    public:
        template <typename T> static auto make(const params& ps, const node* ret) {
            return make<T>(ttype<T>::get().getName(), ps, ret);
        }

        template <typename T> static auto make(const std::string& typeName) {
            return make<T>(typeName, params(), nullptr);
        }

        template <typename T> static auto make() { return make<T>(ttype<T>::get().getName()); }

        template <typename T>
        static auto make(const std::string& typeName, const params& ps, const node* ret)
            -> decltype(_make((T*) nullptr, typeName, ttype<T>::get(), ps, !std::is_constructible<T>::value, ret)) {
            return _make((T*) nullptr, typeName, ttype<T>::get(), ps, !std::is_constructible<T>::value, ret);
        }
    };
}
