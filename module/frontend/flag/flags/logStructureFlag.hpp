/// @file
#pragma once

#include "frontend/flag/flag.hpp"

namespace by {

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
        BY(CLASS(logStructureFlag, flag))

    public:
        const nchar* getName() const override;
        const nchar* getDescription() const override;

    protected:
        const strings& _getRegExpr() const override;
        res _onTake(const flagArgs& tray, cli& c, interpreter& ip, starter& s, errReport& rpt) const override;
    };
}
