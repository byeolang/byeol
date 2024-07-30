#pragma once

#include <fstream>

#include "stream.hpp"

namespace nm {

    class _nout fileLogStream: public stream {
        NM_ME(fileLogStream, stream)

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
