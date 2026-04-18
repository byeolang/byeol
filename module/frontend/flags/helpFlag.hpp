/// @file
#pragma once

#include "flagStacker/flag/flag.hpp"
#include "frontend/common/dep.hpp"

namespace by {

    struct cli;

    /** @ingroup frontend
     *  @brief Flag for displaying command-line interface help information
     *  @details This flag, when activated, outputs a description of all available CLI flags
     *           and their functionalities. It typically terminates the program after displaying help.
     *  @code
     *      // Example usage:
     *      // frontend --help
     *      // frontend -h
     *  @endcode
     */
    class helpFlag: public flag {
        BY(ME(helpFlag, flag))

    public:
        helpFlag(cli& c);

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
