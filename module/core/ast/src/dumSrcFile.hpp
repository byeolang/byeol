/// @file
#pragma once

#include "core/ast/src/srcFile.hpp"

namespace by {
    /// @ingroup core
    /// @brief Dummy source file implementation
    /// @details Placeholder source file used when no actual source file is available.
    class _nout dumSrcFile: public srcFile {
        BY(ME(dumSrcFile, srcFile))

    public:
        dumSrcFile();

    public:
        static dumSrcFile& singleton();
    };
}
