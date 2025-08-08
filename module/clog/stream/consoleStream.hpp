#pragma once

#include "clog/stream/stream.hpp"

namespace by {

    class _nout consoleStream: public stream {
        BY_ME(consoleStream, stream)

    public:
        consoleStream();

    public:
        //  Stream:
        const std::string& getName() const override;

        using super::logBypass;
        nbool logBypass(const nchar* msg) override;
    };
}
