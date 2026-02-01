/// @file
#pragma once

#include "clog/common.hpp"

namespace by {

    typedef std::map<std::string, nbool> enables;

    /** @ingroup clog
     *  @brief Temporary scope-based enable/disable zone for logging streams
     *  @details The @ref logger class has multiple @ref stream objects, and you can control
     *  output destinations by enabling or disabling specific streams. However, it's very
     *  common to need to control streams only within a specific code block or function,
     *  then restore the original state when leaving that block.
     *
     *  enablesZone is designed for this use case. Implemented using the RAII idiom, it
     *  records the enable state of streams upon object creation, then restores each stream's
     *  enable status to its initial state when the enablesZone is destroyed.
     *
     *  @section usage Usage
     *  Typical usage pattern:
     *  @code
     *      // Assume all streams in logger are enabled
     *      {
     *          enablesZone zone;
     *          logger& log = logger::get();
     *          log.getStream(0).setEnable(false); // Disable stream 0
     *          logger::getStream("consoleStream").setEnable(false); // Disable console logging
     *          BY_E("error message1"); // This log won't go to stream 0 or console
     *
     *          logger::setEnable(false); // Disable all streams
     *          BY_E("this message won't log on any stream");
     *      }
     *
     *      BY_E("this message will definitely be log on entire stream"); // Back to normal
     *  @endcode
     */
    class _nout enablesZone {
        BY(ME(enablesZone))

    public:
        enablesZone();
        /**
         * @brief Constructs an enablesZone and immediately sets all logging streams
         *        to the specified enable state, without capturing their previous states.
         * @param newVal The new enable state for all streams (true for enabled, false for disabled).
         */
        enablesZone(nbool newVal);
        ~enablesZone();

    public:
        const enables& getPrev() const;
        /**
         * @brief Captures the current enable states of all streams as the new "previous" state.
         *        This state will be restored when the enablesZone object is destroyed.
         * @return A reference to this enablesZone instance.
         */
        me& setPrev();
        /**
         * @brief Manually releases the enablesZone, preventing restoration of stream states on destruction.
         * @return A reference to this enablesZone instance.
         */
        me& rel();

        /**
         * @brief Sets the enable state for all streams managed by the logger.
         * @param newVal The new enable state (true for enabled, false for disabled).
         * @return A reference to this enablesZone instance.
         */
        me& setEnable(nbool newVal);
        /**
         * @brief Sets the enable state for a specific named stream.
         * @param streamName The name of the stream to modify.
         * @param newVal The new enable state (true for enabled, false for disabled).
         * @return A reference to this enablesZone instance.
         */
        me& setEnable(const std::string& streamName, nbool newVal);

    private:
        enables _enables;
    };
}
