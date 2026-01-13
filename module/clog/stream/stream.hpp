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
         *  @return true means an error.
         */
        virtual nbool logBypass(const nchar* message);

        nbool logBypass(const std::string& message);

        nbool isNull() const;
        virtual nbool isEnable() const;

        /**
         *  @return previous set value.
         */
        virtual void setEnable(nbool isEnable);
        virtual const std::string& getName() const = 0;
        virtual nbool init();
        virtual nbool isInit() const;
        virtual nbool rel();

    private:
        nbool _isEnable;
    };
}
