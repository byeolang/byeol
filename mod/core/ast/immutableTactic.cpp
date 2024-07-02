#include "immutableTactic.hpp"
#include "node.hpp"

namespace nm {

    NM_DEF_ME(immutableTactic)

    me me:: singletone;

    nbool me::bind(binder& me, const instance& it) {
        const node* new1 = &it.cast<node>(); // I can guarantee that it is a node type.
        nbool needClone = new1->getType().isImmutable() &&
            new1->getBindTag().getStrongCnt() > 0;
        if(needClone)
            new1 = (const node*) new1->clone();

        return super::bind(me, *new1);
    }
}
