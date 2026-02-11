/// @file
#pragma once

#include "stela/common.hpp"

namespace by {
    /** @ingroup stela
     *  @brief Smart dedent handler for indentation-based parsing
     *  @details A class that counts and manages in an array how many leading spaces exist
     *  whenever @ref stelaParser creates a scope based on indentation.
     *
     *  For example, when parsing this stela code:
     *  @code
     *      def config
     *          def device
     *            name := "my device"
     *            # position 1
     *  @endcode
     *
     *  If the parser is parsing at position 1, smartDedent's internal array is [0, 4, 6].
     *  This means the outermost scope's whitespace count is smartDedent[0] = 0, and the
     *  innermost device scope's whitespace count is smartDedent[smartDedent.len() - 1] = 6,
     *  meaning 6 leading spaces are required.
     */
    class _nout stelaSmartDedent {
        BY(ME(stelaSmartDedent))

        enum Status {
            OFF = -1,
            CAUGHT = 0,
        };

    public:
        stelaSmartDedent();

    public:
        /**
         * @brief Decrements the internal count, typically indicating a pending dedent.
         * @return A reference to this stelaSmartDedent instance.
         */
        me& countDown();
        /**
         * @brief Increments the internal count, typically indicating a pending dedent.
         * @return A reference to this stelaSmartDedent instance.
         */
        me& countUp();
        /**
         * @brief Enables dedent tracking for future parsing, deferring any pending dedents.
         * @return A reference to this stelaSmartDedent instance.
         */
        me& useLater();
        /**
         * @brief Generates and returns a dedent token based on the internal count.
         * @return The token ID for DEDENT, or 0 if no dedent is pending.
         */
        nint dedent();
        void rel();
        nbool canDedent() const;
        nbool isOn() const;

    private:
        nint _cnt;
        nbool _isEnable;
    };
} // namespace by
