/// @file
#pragma once

#include "core/ast/src/srcFile.hpp"

namespace by {
    class _nout dumSrcFile: public srcFile {
        BY(ME(dumSrcFile, srcFile))

    public:
        dumSrcFile();

    public:
        static dumSrcFile& singleton();
    };
}
