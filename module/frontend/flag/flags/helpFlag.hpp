/// @file
#pragma once

#include "frontend/flag/flag.hpp"

namespace by {

    /** @ingroup byeol
     *  @brief Flag for displaying command-line interface help information
     *  @details This flag, when activated, outputs a description of all available CLI flags
     *           and their functionalities. It typically terminates the program after displaying help.
     *  @code
     *      // Example usage:
     *      // byeol --help
     *      // byeol -h
     *  @endcode
     */
    class helpFlag: public flag {
        BY(CLASS(helpFlag, flag))

    public:
        const nchar* getName() const override;
        const nchar* getDescription() const override;

    protected:
        const strings& _getRegExpr() const override;
        res _onTake(const flagArgs& tray, cli& c, interpreter& ip, starter& s) const override;
    };
}
