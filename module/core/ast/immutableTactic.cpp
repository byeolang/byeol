#include "core/builtin/container/native/tnarr.inl"
#include "core/builtin/container/native/tnmap.inl"
#include "core/ast/immutableTactic.hpp"

#include "core/ast/node.hpp"

namespace by {

    BY_DEF_ME(immutableTactic)

    me me::singleton;

    nbool me::bind(binder& me, const instance& it) {
        const node* new1 = it.cast<node>(); // I can guarantee that it is a node type.
        nbool needClone = new1->getType().isImmutable() && new1->getBindTag()->getStrongCnt() > 0;
        if(needClone) new1 = (const node*) new1->clone();

        return super::bind(me, *new1);
    }
}
