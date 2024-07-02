#pragma once

#include "../common.hpp"

namespace nm {

    class _nout stream {
        NAMU_ME(stream)

    public:
        stream(nbool isEnable = true);
        virtual ~stream();

    public:
        /// @return true means an error.
        virtual nbool logBypass(const nchar* message);
        nbool isNull() const;
        virtual nbool isEnable() const;
        /// @return previous set value.
        virtual void setEnable(nbool isEnable);
        virtual const std::string& getName() const = 0;
        virtual nbool init();
        virtual nbool isInit() const;
        virtual nbool rel();

    private:
        nbool _isEnable;
    };
}
