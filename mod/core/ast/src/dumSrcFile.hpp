#pragma once

#include "srcFile.hpp"

namespace nm {
    class _nout dumSrcFile : public srcFile {
        NM(ME(dumSrcFile, srcFile))

    public:
        dumSrcFile();

    public:
        static dumSrcFile& singletone();
    };
}
