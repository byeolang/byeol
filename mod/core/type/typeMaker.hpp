#pragma once

#include "core/type/funcMgdType.hpp"

namespace nm {

    class _nout typeMaker {
        NM(ME(typeMaker))

    private:
        static mgdType _make(void*, const std::string& name, const type& super, const params& ps,
            nbool isAdt, const node& ret) {
            return mgdType(name, super, ps, isAdt, ret);
        }

        static funcMgdType _make(baseFunc*, const std::string& name, const type& super,
            const params& ps, nbool isAdt, const node& ret) {
            return funcMgdType(name, super, ps, isAdt, ret);
        }

    public:
        template <typename T> static auto make(const params& ps, const node& ret) {
            return make<T>(ttype<T>::get().getName(), ps, ret);
        }

        template <typename T> static auto make(const std::string& name) {
            return make<T>(name, params(), nulOf<node>());
        }

        template <typename T>
        static auto make(const std::string& name, const params& ps, const node& ret)
            -> decltype(_make((T*) nullptr, name, ttype<T>::get(), ps,
                !std::is_constructible<T>::value, ret)) {
            return _make((T*) nullptr, name, ttype<T>::get(), ps, !std::is_constructible<T>::value,
                ret);
        }
    };
}
