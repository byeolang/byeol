/// @file
#pragma once

#include "core/type/ntype.hpp"

namespace by {
    /** @ingroup core
     *  @brief Smart indentation management for code parsing
     *  @details A class that @ref parser uses to count and manage in an array how many leading spaces there are each
     *  time it creates a scope with indentation. For example, when parsing the following byeol code:
     *
     *  @code
     *  def config
     *      def device
     *        name := "my device"
     *        # 1)
     *  @endcode
     *
     *  If the parser is parsing at position 1, smartDedent's internal array value is [0, 4, 6]. That is, the number of
     *  spaces for the outermost scope is smartDedent[0] = 0, and the number of spaces proving it's the scope of the
     *  innermost device scope is smartDedent[smartDedent.len() - 1] = 6, meaning there should be 6 leading spaces.
     */
    class _nout smartDedent: public typeProvidable, public clonable {
        BY(CLASS(smartDedent))

        enum Status {
            OFF = -1,
            CAUGHT = 0,
            ON = 1,
        };

    public:
        smartDedent();

    public:
        /**
         * @brief Decrements the internal count, typically indicating a pending dedent.
         * @return A reference to this smartDedent instance.
         */
        me& countDown();
        /**
         * @brief Increments the internal count, typically indicating a pending dedent.
         * @return A reference to this smartDedent instance.
         */
        me& countUp();
        /**
         * @brief Enables dedent tracking for future parsing, deferring any pending dedents.
         * @return A reference to this smartDedent instance.
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
}
