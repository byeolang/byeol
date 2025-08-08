#pragma once

#include "core/builtin/container/native/tnchain.inl"

namespace by {
    typedef tnchain<std::string, node, tnmap<std::string, node, immutableTactic>> scope;
    extern template class _nout
        tnchain<std::string, node, tnmap<std::string, node, immutableTactic>>;
}
