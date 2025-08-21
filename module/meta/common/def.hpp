/// @file
#pragma once

#include "meta/common/_nout.hpp"
#include "meta/common/dep.hpp"

namespace by {
    class type;
    typedef std::vector<type*> types;
}

#define BY_INIT_META(me) BY_INITIATOR(initMeta, { ::by::ttype<me>().init(); })

// byeolMeta macro's sub-commands:
//  INIT META: it redirects to BY_INIT_META macro.
#define __BY__DECL_INIT_META(ME) BY_INIT_META(ME)
