/// @file
#pragma once

#include "core/common.hpp"

namespace by {

    class obj;
    class src;
    class errReport;
    class pod;
    template <typename T, typename R, typename RSquare> class tucontainable;
    typedef tucontainable<node, node*, node&> ucontainable;
    template <typename T, typename WRP> class tnarr;
    typedef tnarr<src, strTactic> srcs;

    /** @ingroup core
     *  @brief Interface for pod creation and verification
     *  @details Provides pure virtual methods for parsing and verifying pod content.
     *  Implemented by different pod loading strategies to handle pod creation.
     */
    class _nout podMakable {
        BY_ME(podMakable)
        BY_INIT_META(me)

    public:
        /**
         * @brief Pure virtual method for parsing pod content into source representations.
         * @param rpt The error report to collect any parsing errors.
         * @param tray A bicontainable to store intermediate parsed results or components.
         * @return true if success.
         */
        virtual nbool parse(errReport& rpt, pod& pak) = 0;

        virtual nbool expand(errReport& rpt, pod& pak) = 0;
        /**
         * @brief Pure virtual method for verifying the parsed pod content.
         * @param rpt The error report to collect any verification errors.
         * @param pak The pod object to verify.
         * @return true if the pod content is successfully verified, false otherwise.
         */
        virtual nbool verify(errReport& rpt, pod& pak) = 0;
    };
}
