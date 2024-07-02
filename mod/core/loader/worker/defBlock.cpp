#include "defBlock.hpp"
#include "../../ast/node.hpp"

namespace nm {

    NAMU(DEF_ME(defBlock))

    me::defBlock(): asScope(new scope()), asPreCtor(new narr()) {}
}
