/// @file
#pragma once

#include "indep/helper/dumpable.hpp"
#include "indep/helper/errLv.hpp"

namespace by {

    /** @ingroup indep
     *  @brief Interface for objects that can report errors
     *  @details Provides methods for logging errors with different severity levels
     *  and stack trace information. Extends dumpable interface.
     */
    class _nout errorable: public dumpable {
    public:
        /**
         *  @brief Log error message
         */
        virtual void log() const = 0;

        /**
         *  @brief Log error in debug mode only
         */
        void dbgLog() const;

        /**
         *  @brief Log error with stack trace
         */
        virtual void logStack() const = 0;
        virtual const std::string& getMsg() const = 0;
        virtual errLv::level getLv() const = 0;

        /**
         *  @brief Get human-readable error level name
         */
        const std::string& getLevelName() const;
    };
}
