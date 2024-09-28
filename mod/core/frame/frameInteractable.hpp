#pragma once

#include "../common.hpp"

namespace nm {

    class node;
    template <typename K, typename V> class tbicontainable;
    typedef tbicontainable<std::string, node> bicontainable;

    class _nout frameInteractable {
    public:
        virtual void inFrame(const bicontainable& args) const = 0;

        void inFrame() const { inFrame(nulOf<bicontainable>()); }

        virtual void outFrame(const bicontainable& args) const = 0;

        void outFrame() const { outFrame(nulOf<bicontainable>()); }
    };
}
