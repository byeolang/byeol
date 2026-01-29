/// @file
#pragma once

#include "frontend/flag/flag.hpp"

namespace by {

    /** @ingroup byeol
     *  @brief Flag for displaying application version information
     *  @details When this flag is activated, the program outputs details about its
     *           version, build date, and other relevant build information, then terminates.
     *  @code
     *      // Example usage:
     *      // byeol --version
     *  @endcode
     */
    class verFlag: public flag {
        BY(CLASS(verFlag, flag))

    public:
        const nchar* getName() const override;
        const nchar* getDescription() const override;

    protected:
        const strings& _getRegExpr() const override;
        res _onTake(const flagArgs& tray, cli& c, interpreter& ip, starter& s) const override;
    };
}
