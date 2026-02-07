/// @file
#pragma once

#include "frontend/flag/flag.hpp"

namespace by {

    /** @ingroup frontend
     *  @brief Flag for enabling verbose logging output
     *  @details When this flag is activated, the interpreter and related components will produce
     *           more detailed and extensive log messages, useful for in-depth debugging and monitoring.
     *  @code
     *      // Example usage:
     *      // frontend --verbose
     *      // frontend -v
     *  @endcode
     */
    class verboseFlag: public flag {
        BY(CLASS(verboseFlag, flag))

    public:
        const nchar* getName() const override;
        const nchar* getDescription() const override;

    protected:
        const strings& _getRegExpr() const override;
        res _onTake(const flagArgs& tray, cli& c, interpreter& ip, starter& s, errReport& rpt) const override;
    };
}
