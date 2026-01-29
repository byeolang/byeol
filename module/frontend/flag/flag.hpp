/// @file
#pragma once

#include <regex>

#include "frontend/common.hpp"
#include "frontend/flag/flagArgs.hpp"

namespace by {

    struct cli;

    typedef std::vector<std::string> strings;

    /** @ingroup byeol
     *  @brief Base class for command-line flag handling
     *  @details Handles flags commonly seen in shell-based programs. Note this class doesn't
     *  represent a flag like `--version`, but rather handles whether such flags exist and
     *  what actions to take.
     *
     *  @section Flag descriptions
     *  Each flag defines not only its name but also a description of its functionality.
     *  These are used by @ref helpFlag.
     *
     *  @section Regex pattern matching
     *  Each flag class has a defined pattern to search for. For example, @ref verFlag
     *  searches for the `--version` string in program arguments. If found, it retrieves
     *  version info from @ref buildFeature and outputs it. This behavior works by calling
     *  `flag::take()`, which uses the regex defined in each derived class's `_getRegExpr()`
     *  to find the desired pattern. Since regex is used, flag order is ignored.
     *
     *  Multiple patterns can be defined in regex:
     *  @code
     *      const strings& me::_getRegExpr() const {
     *          static strings inner{"^\\-S$", "^\\--show-structure$"};
     *          return inner;
     *      }
     *  @endcode
     *  Matches either `^\-S$` or `^\--show-structure$`.
     *
     *  @section Consuming multiple flag arguments
     *  Consider verFlag: when `--version` exists in arguments, it outputs version. After
     *  operating, it removes `--version` from @ref flagArgs to prevent verFlag from running
     *  again.
     *
     *  Now consider @ref bufferSrcFlag, used like:
     *  `--script "main() void: print(\"wow!\")"`
     *
     *  Breaking this down: `--script` indicates flag start, followed by space and
     *  `"<your code>"` which is the byeol code body argument to execute. Some flags consume
     *  not just the matched pattern but additional arguments. Each derived flag class can
     *  override `getArgCount()` to specify how many additional arguments to extract.
     *
     *  @section Terminating flags
     *  bufferSrcFlag needs preliminary work before program start. Conversely, verFlag
     *  immediately outputs version and terminates without interpretation once matched (as
     *  most programs do). To terminate immediately after matching, override `_onTake()` to
     *  return EXIT_PROGRAM. For continued operation like bufferSrcFlag, return MATCH.
     */
    class flag: public instance {
        BY(ADT(flag, instance))
        typedef std::vector<int> deleteIndices;

    public:
        enum res {
            MATCH,
            EXIT_PROGRAM,
            NOT_MATCH
        };

    public:
        virtual const nchar* getName() const = 0;
        virtual const nchar* getDescription() const = 0;

        /**
         *  @return false if flag wants to exit program.
         */
        virtual res take(interpreter& ip, starter& s, cli& c, flagArgs& a) const;

    protected:
        /**
         *  when this flag matched to one of args, the value of returning this func will be
         *  count of continuous argument of this flag.
         */
        virtual ncnt getArgCount() const;

        /**
         * @brief Deletes arguments from the `flagArgs` list.
         * @details This method is used internally to remove arguments that have been consumed
         *          by this flag, preventing them from being processed again by other flags.
         * @param a The `flagArgs` instance from which arguments are to be deleted.
         * @param indices A vector of indices indicating which arguments to delete.
         */
        void _delArgs(flagArgs& a, std::vector<int> indices) const;

        virtual const strings& _getRegExpr() const = 0;
        virtual res _onTake(const flagArgs& tray, cli& c, interpreter& ip, starter& s) const = 0;

    private:
        nbool _parseOption(flagArgs& a, flagArgs& tray, deleteIndices& deletes) const;
    };

    typedef std::vector<tstr<flag>> flags;
} // namespace by
