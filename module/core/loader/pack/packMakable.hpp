/// @file
#pragma once

#include "core/common.hpp"

namespace by {

    class obj;
    class src;
    class errReport;
    template <typename K, typename V> class tbicontainable;
    typedef tbicontainable<std::string, node> bicontainable;
    template <typename T, typename WRP> class tnarr;
    typedef tnarr<src, strTactic> srcs;

    /// @ingroup core
    /// @brief Interface for pack creation and verification
    /// @details Provides pure virtual methods for parsing and verifying pack content.
    /// Implemented by different pack loading strategies to handle pack creation.
    class _nout packMakable {
        BY_ME(packMakable)
        BY_INIT_META(me)

    public:
        virtual tstr<srcs> parse(errReport& rpt, bicontainable& tray) = 0;
        virtual nbool verify(errReport& rpt, obj& pak) = 0;
    };
}
