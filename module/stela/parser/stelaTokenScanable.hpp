/// @file
#pragma once

#include "stela/common.hpp"

typedef union ZZSTYPE ZZSTYPE;
typedef struct lloc ZZLTYPE;
typedef void* zzscan_t;

namespace by {

    class stelaParser;

    /// @ingroup stela
    /// @brief Interface for token scanning during stela parsing
    /// @details Provides callback interface for token scanning events.
    /// Used by the parser to process tokens during lexical analysis.
    class _nout stelaTokenScanable {
        BY(ME(stelaTokenScanable))

    public:
        nint onScan(stelaParser& ps, ZZSTYPE* val, ZZLTYPE* loc, zzscan_t scanner);
        virtual nint onScan(stelaParser& ps, ZZSTYPE* yylval, ZZLTYPE* loc, zzscan_t yyscanner, nbool& isBypass) = 0;
    };
}
