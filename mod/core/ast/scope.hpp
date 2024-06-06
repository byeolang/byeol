#pragma once

#include "../builtin/container/native/tnchain.hpp"

namespace namu {
    typedef tnchain<std::string, node, tnmap<std::string, node, immutableTactic>> scope;
}
