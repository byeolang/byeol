#include "core/builtin/container/native/tnarr.inl"
#include "core/builtin/container/native/tnmap.inl"
#include "core/ast/tpriorities.inl"

#include "core/ast/node.hpp"

namespace by {
    const _nout nchar* getPriorTypeName(priorType type) {
        switch(type) {
            case EXACT_MATCH: return "EXACT_MATCH";
            case NUMERIC_MATCH: return "NUMERIC_MATCH";
            case IMPLICIT_MATCH: return "IMPLICIT_MATCH";
            case NO_MATCH: return "NO_MATCH";
        }
        return "unexepected prior type";
    }
}
