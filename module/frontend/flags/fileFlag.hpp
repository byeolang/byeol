/// @file
#pragma once

#include "flagStacker/flag/flag.hpp"
#include "frontend/common/dep.hpp"

namespace by {

    struct cli;

    /** @ingroup frontend
     *  @brief Flag for specifying a frontend source code file to load and execute
     *  @details This flag allows users to provide a file path containing frontend source code,
     *           which will then be parsed and executed by the interpreter. It typically consumes
     *           one additional argument: the path to the file.
     *  @code
     *      // Example usage:
     *      // frontend --file "my_script.frontend"
     *  @endcode
     */
    class fileFlag: public flag {
        BY(ME(fileFlag, flag))

    public:
        fileFlag(cli& c);

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
