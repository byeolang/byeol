/// @file
#pragma once

#include "frontend/flag.hpp"

struct cliTest;

namespace by {

    struct programRes {
        errReport rpt;
        nint res;
    };

    /** @ingroup frontend
     *  @brief Command-line interface for frontend language system
     *  @details Core class of the frontend module. Uses @ref interpreter from the core module
     *  to parse, verify, and evaluate code, coordinating appropriate classes for these
     *  processes. Rather than implementing algorithms itself, cli orchestrates well-designed
     *  classes.
     *
     *  Receives user-entered arguments to the CLI program as parameters and parses them to
     *  perform additional commands. See the `flags` folder for details.
     *
     *  @section core_algorithm Core algorithm
     *  When user calls eval() with flagArgs parameter, execution proceeds as follows:
     *  1. Create interpreter, @ref errReport, @ref starter objects
     *  2. Set flags on interpreter and starter (refers to @ref worker flags, different from
     *     cli's @ref flag. See worker for details)
     *  3. Parse flagArgs to perform preliminary work (executed by calling `take()` on flag
     *     object)
     *  4. Execute interpreter
     *  5. Check interpretation results. If issues exist, output results and terminate
     *  6. If no issues, put verified AST into starter and execute
     *  7. Return starter's result
     */
    struct cli: public tworker<programRes, flagArgs> {
        typedef tworker<programRes, flagArgs> __super26;
        BY(CLASS(cli, __super26));
        friend struct ::cliTest;

    public:
        cli();
        cli(errReport* rpt);

    public:
        const flags& getFlags() const;

    protected:
        programRes _onWork() override;

    private:
        flag::res _evalArgs(interpreter& ip, flagArgs& a, starter& s, errReport& rpt);
    };
}
