/// @file
#pragma once

#include "clog/common.hpp"

namespace by {

    /** @ingroup clog
     *  @brief Base class for all logging output streams
     *  @details Provides the fundamental interface for logging destinations
     *  such as console output, file output, or network logging.
     */
    class _nout stream {
        BY_ME(stream)

    public:
        stream(nbool isEnable = true);
        virtual ~stream();

    public:
        /**
         * @brief Log message directly without formatting
         * @return true on success, false on error
         */
        virtual nbool logBypass(const nchar* message);

        nbool logBypass(const std::string& message);

        /**
         * @brief Checks if this stream is a null object.
         * @return true if the stream is a null object, false otherwise.
         */
        nbool isNull() const;
        virtual nbool isEnable() const;
        virtual void setEnable(nbool isEnable);
        virtual const std::string& getName() const = 0;
        /**
         * @brief Initializes the stream, transitioning its state from RELEASED to INITIALIZED.
         * @return true on successful initialization, false otherwise (e.g., already initialized).
         */
        virtual nbool init();
        virtual nbool isInit() const;
        /**
         * @brief Releases the stream's resources, transitioning its state from INITIALIZED to RELEASED.
         * @return true on successful release, false otherwise.
         */
        virtual nbool rel();

    private:
        nbool _isEnable;
    };
}
