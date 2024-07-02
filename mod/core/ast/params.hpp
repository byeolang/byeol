#pragma once

#include "param.hpp"
#include "../builtin/container/native/tnarr.inl"
#include "../ast/scope.hpp"

namespace nm {
    class _nout params : public tnarr<param> {
        NAMU(CLASS(params, tnarr<param>))

    public:
        /// @param  elems   instances to derived type of T.
        ///                 should be created on Heap.
        template <typename... Es>
        explicit params(const Es&... elems) {
            static_assert(areBaseOfT<param, Es...>::value, "some of type of args are not base of type 'T'");
            add( { (param*) &elems... } );
        }

    public:
        std::string toStr() const;
    };
}
