#pragma once

#include <fstream>

#include "clog/stream/stream.hpp"

namespace by {

    class _nout fileLogStream: public stream {
        BY_ME(fileLogStream, stream)

    public:
        //  fileLogStream:
        fileLogStream();
        explicit fileLogStream(const std::string& new_path);

    public:
        nbool setPath(const std::string& new_path);
        const std::string& getPath() const;
        //  stream:
        const std::string& getName() const override;

        using super::logBypass;
        nbool logBypass(const nchar* message) override;

    private:
        std::ofstream _file;
        std::string _path;
    };
}
