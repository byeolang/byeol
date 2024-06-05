#pragma once

#include "../common.hpp"

namespace namu {

    class node;
    template <typename K, typename V> class tbicontainable;
    typedef tbicontainable<std::string, node> bicontainable;

    class _nout frameInteractable {
    public:
        virtual void inFrame(const bicontainable& args) = 0;
        void inFrame() {
            inFrame(nulOf<bicontainable>());
        }

        virtual void outFrame(const bicontainable& args) = 0;
        void outFrame() {
            outFrame(nulOf<bicontainable>());
        }
    };
}
