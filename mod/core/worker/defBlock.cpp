#include "defBlock.hpp"

#include "../ast/node.hpp"

namespace nm {

    NM(DEF_ME(defBlock))

    me::defBlock(): asScope(new scope()), asPreCtor(new narr()) {}
}
