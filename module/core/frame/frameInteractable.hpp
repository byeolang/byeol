#pragma once

#include "core/common.hpp"

namespace nm {

    class node;
    template <typename K, typename V> class tbicontainable;
    typedef tbicontainable<std::string, node> bicontainable;

    class _nout frameInteractable {
    public:
        virtual ~frameInteractable() = default;

    public:
        virtual void inFrame(const bicontainable* args) const = 0;

        void inFrame() const { inFrame(nullptr); }

        virtual void outFrame(const bicontainable* args) const = 0;

        void outFrame() const { outFrame(nullptr); }
    };
}
