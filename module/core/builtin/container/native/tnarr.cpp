#include "core/builtin/container/native/tnarr.inl"
#include "core/builtin/container/native/tnmap.inl"
#include "core/ast/node.hpp"

namespace by {
    template class _nout tnarr<node>;
    template class _nout tucontainable<node>;
}
