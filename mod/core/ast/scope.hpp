#pragma once

#include "../builtin/container/native/tnchain.inl"

namespace nm {
    typedef tnchain<std::string, node, tnmap<std::string, node, immutableTactic>> scope;
}
