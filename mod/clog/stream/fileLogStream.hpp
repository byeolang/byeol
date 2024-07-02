#pragma once

#include "stream.hpp"
#include <fstream>

namespace nm {

    class _nout fileLogStream : public stream {
        NAMU_ME(fileLogStream, stream)

    public:
        //  fileLogStream:
        fileLogStream();
        explicit fileLogStream(const std::string& new_path);

    public:
        nbool setPath(const std::string& new_path);
        const std::string& getPath() const;
        //  stream:
        const std::string& getName() const override;
        nbool logBypass(const char* message) override;

    private:
        std::ofstream _file;
        std::string _path;
    };
}
