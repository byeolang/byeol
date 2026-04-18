/// @file
#pragma once

#include "flagStacker/flag/flag.hpp"
#include "frontend/common/dep.hpp"

namespace by {

    struct cli;

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
        BY(ME(verboseFlag, flag))

    public:
        verboseFlag(cli& c);

    public:
        const nchar* getName() const override;
        const nchar* getDescription() const override;

    protected:
        const strings& _getRegExpr() const override;
        res _onTake(const flagArgs& tray) const override;

    private:
        cli& _cli;
    };
}
