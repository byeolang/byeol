#pragma once

#include "stream.hpp"

namespace nm {

    class _nout consoleStream : public stream {
        NM_ME(consoleStream, stream)

    public:
        consoleStream();

    public:
        //  Stream:
        const std::string& getName() const override;
        nbool logBypass(const char* message) override;
    };
}
