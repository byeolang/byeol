/// @file
#pragma once

#include "core/common.hpp"

namespace by {

    class node;
    template <typename K, typename V> class tbicontainable;
    typedef tbicontainable<std::string, node> bicontainable;

    /// @ingroup core
    /// @brief Interface for frame interaction
    /// @details Abstract interface for objects that can enter and exit execution frames.
    class _nout frameInteractable {
    public:
        virtual ~frameInteractable() = default;

    public:
        virtual void inFrame(const bicontainable* args) const = 0;

        void inFrame() const { inFrame(nullptr); }

        virtual void outFrame() const = 0;
    };
}
