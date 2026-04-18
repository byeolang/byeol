/// @file
#pragma once

#include "flagStacker/flag/flag.hpp"
#include "frontend/common/dep.hpp"

namespace by {

    struct cli;

    /** @ingroup frontend
     *  @brief Flag for displaying application version information
     *  @details When this flag is activated, the program outputs details about its
     *           version, build date, and other relevant build information, then terminates.
     *  @code
     *      // Example usage:
     *      // frontend --version
     *  @endcode
     */
    class verFlag: public flag {
        BY(ME(verFlag, flag))

    public:
        const nchar* getName() const override;
        const nchar* getDescription() const override;

    protected:
        const strings& _getRegExpr() const override;
        res _onTake(const flagArgs& tray) const override;
    };
}
