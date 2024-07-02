#pragma once

#include "../../builtin/res/tpair.hpp"

namespace nm {

    class obj;
    class src;
    class errReport;
    template <typename K, typename V> class tbicontainable;
    typedef tbicontainable<std::string, node> bicontainable;
    template <typename T, typename WRP> class tnarr;
    typedef tnarr<src, strTactic> srcs;

    class _nout packMakable {
        NM_ME(packMakable)
        NM_INIT_META(me)

    public:
        virtual tstr<srcs> parse(errReport& rpt, bicontainable& tray) = 0;
        virtual nbool verify(errReport& rpt, obj& pak) = 0;
    };
}
