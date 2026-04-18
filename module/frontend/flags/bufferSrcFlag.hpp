/// @file
#pragma once

#include "flagStacker/flag/flag.hpp"
#include "frontend/common/dep.hpp"

namespace by {

    struct cli;

    /** @ingroup frontend
     *  @brief Flag for providing frontend source code directly as a string argument
     *  @details This flag allows users to pass frontend source code as a string, which is then parsed
     *           and executed by the interpreter. It consumes one additional argument for the source code string.
     *  @code
     *      // Example usage:
     *      // frontend --script "main() void: print(\"Hello, world!\")"
     *  @endcode
     */
    class bufferSrcFlag: public flag {
        BY(ME(bufferSrcFlag, flag))

    public:
        bufferSrcFlag(cli& c);

    public:
        const nchar* getName() const override;
        const nchar* getDescription() const override;
        ncnt getArgCount() const override;

    protected:
        const strings& _getRegExpr() const override;
        res _onTake(const flagArgs& tray) const override;

    private:
        cli& _cli;
    };
}
