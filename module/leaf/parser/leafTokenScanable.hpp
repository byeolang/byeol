/// @file
#pragma once

#include "leaf/common.hpp"

typedef union ZZSTYPE ZZSTYPE;
typedef struct lloc ZZLTYPE;
typedef void* zzscan_t;

namespace by {

    class leafParser;

    /// @ingroup leaf
    /// @brief Interface for token scanning during leaf parsing
    /// @details Provides callback interface for token scanning events.
    /// Used by the parser to process tokens during lexical analysis.
    class _nout leafTokenScanable {
        BY(ME(leafTokenScanable))

    public:
        nint onScan(leafParser& ps, ZZSTYPE* val, ZZLTYPE* loc, zzscan_t scanner);
        virtual nint onScan(leafParser& ps, ZZSTYPE* yylval, ZZLTYPE* loc, zzscan_t yyscanner, nbool& isBypass) = 0;
    };
}
