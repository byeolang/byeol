/// @file
#pragma once

#include "flagStacker/flag/flag.hpp"
#include "frontend/common/dep.hpp"

namespace by {

    struct cli;

    /** @ingroup frontend
     *  @brief Flag for enabling logging of the Abstract Syntax Tree (AST) or program structure
     *  @details When this flag is activated, the interpreter and starter will output detailed
     *           information about the parsed AST and its execution flow, useful for debugging.
     *  @code
     *      // Example usage:
     *      // frontend --show-structure
     *      // frontend -S
     *  @endcode
     */
    class logStructureFlag: public flag {
        BY(ME(logStructureFlag, flag))

    public:
        logStructureFlag(cli& c);

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
