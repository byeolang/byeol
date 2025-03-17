#include "tnchain.inl"
#include "../../../ast/node.hpp"

namespace nm {
    template class _nout tnchain<std::string, node, tnmap<std::string, node, strTactic>>;
}
