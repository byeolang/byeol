#include "defBlock.hpp"
#include "../../ast/node.hpp"

namespace namu {

    NAMU(DEF_ME(defBlock))

    me::defBlock(): asScope(new scope()), asPreCtor(new narr()) {}
}
