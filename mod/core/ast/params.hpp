#pragma once

#include "core/ast/scope.hpp"
#include "core/builtin/container/native/tnarr.hpp"
#include "core/ast/param.hpp"

namespace nm {
    typedef std::vector<std::string> strings;

    class _nout params: public tnarr<param> {
        NM(CLASS(params, tnarr<param>))

    public:
        /// @param  elems   instances to derived type of T.
        ///                 should be created on Heap.
        template <typename... Es> explicit params(const Es&... elems) {
            static_assert(areBaseOfT<param, Es...>::value,
                "some of type of args are not base of type 'T'");
            add({(param*) &elems...});
        }

    public:
        nbool operator==(const me& rhs) const;
        nbool operator!=(const me& rhs) const;

    public:
        /// @remark in order to call this func, you must have prepared thread and frames.
        std::string toStr() const;

        static me make(const strings& names, const narr& args);
        static me make(const narr& args);
    };

    class _nout dumParams: public params {
        NM(CLASS(dumParams, params))

    public:
        using tucontainable<param>::set;
        using tarrayable<param>::set;

        nbool set(const iter& at, const param& new1) override { return true; }

        nbool set(nidx n, const param& new1) override { return true; }

        using tucontainable<param>::add;
        using tarrayable<param>::add;

        nbool add(const iter& at, const param& new1) override { return true; }

        nbool add(nidx n, const param& new1) override { return true; }

        void add(const iter& here, const iter& from, const iter& to) override {}
    };
} // namespace nm
