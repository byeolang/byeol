#pragma once

#include "meta/common/_nout.hpp"
#include "meta/common/dep.hpp"

namespace by {
    class type;
    typedef std::vector<type*> types;
}

#define NM_INIT_META(me) NM_INITIATOR(initMeta, { ::by::ttype<me>().init(); })

// byeolMeta macro's sub-commands:
//  INIT META: it redirects to NM_INIT_META macro.
#define __NM__DECL_INIT_META(ME) NM_INIT_META(ME)
