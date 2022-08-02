#pragma once

#include "../../builtin/res/tpair.hpp"

namespace namu {

    class obj;
    class src;
    class errReport;
    template <typename K, typename V> class tbicontainable;
    typedef tbicontainable<std::string, node> bicontainable;
    template <typename T, typename WRP> class tnarr;
    typedef tnarr<src, strTactic> srcs;

    class _nout packMakable {
        NAMU_DECL_ME(packMakable)
        NAMU_INIT_META(me)

    public:
        virtual tstr<srcs> parse(errReport& rpt, bicontainable& tray) = 0;
        virtual nbool verify(errReport& rpt, obj& pak) = 0;
    };
}
