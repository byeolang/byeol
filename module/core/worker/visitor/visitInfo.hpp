/// @file
#pragma once

#include "core/common.hpp"

namespace by {
    /** @ingroup core
     *  @brief Visitor context information
     *  @details Provides context information during AST traversal including name, parent, index, and depth.
     */
    class _nout visitInfo {
        BY(ME(visitInfo))

    public:
        visitInfo(const std::string* name, node* parent, nidx index, ncnt len, ncnt additionalLen, nint depth):
            me(name ? *name : std::string(""), parent, index, len, additionalLen, depth) {}

        visitInfo(const std::string& newName, node* newParent, nidx newIndex, ncnt newLen, ncnt newAdditionalLen,
            nint newDepth):
            name(newName),
            parent(newParent),
            index(newIndex),
            len(newLen),
            additionalLen(newAdditionalLen),
            depth(newDepth) {}

    public:
        std::string name;
        node* parent;
        nidx index;
        ncnt len;
        ncnt additionalLen;
        nint depth;
    };
}
