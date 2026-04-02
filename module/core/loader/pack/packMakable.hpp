/// @file
#pragma once

#include "core/common.hpp"

namespace by {

    class obj;
    class src;
    class errReport;
    class pack;
    template <typename T, typename R, typename RSquare> class tucontainable;
    typedef tucontainable<node, node*, node&> ucontainable;
    template <typename T, typename WRP> class tnarr;
    typedef tnarr<src, strTactic> srcs;

    /** @ingroup core
     *  @brief Interface for pack creation and verification
     *  @details Provides pure virtual methods for parsing and verifying pack content.
     *  Implemented by different pack loading strategies to handle pack creation.
     */
    class _nout packMakable {
        BY_ME(packMakable)
        BY_INIT_META(me)

    public:
        /**
         * @brief Pure virtual method for parsing pack content into source representations.
         * @param rpt The error report to collect any parsing errors.
         * @param tray A bicontainable to store intermediate parsed results or components.
         * @return true if success.
         */
        virtual nbool parse(errReport& rpt, pack& pak) = 0;
        /**
         * @brief Pure virtual method for verifying the parsed pack content.
         * @param rpt The error report to collect any verification errors.
         * @param pak The pack object to verify.
         * @return true if the pack content is successfully verified, false otherwise.
         */
        virtual nbool verify(errReport& rpt, pack& pak) = 0;
    };
}
