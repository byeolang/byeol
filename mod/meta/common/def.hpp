#pragma once

#include "dep.hpp"
#include "common/_nout.hpp"

namespace nm {
    class type;
    typedef std::vector<type*> types;
}

#define NAMU_INIT_META(me) \
    NAMU_INITIATOR(initMeta, { \
        ::nm::ttype<me>().init(); \
    })

// namuMeta macro's sub-commands:
//  INIT META: it redirects to NAMU_INIT_META macro.
#define __NAMU__DECL_INIT_META(ME) NAMU_INIT_META(ME)
