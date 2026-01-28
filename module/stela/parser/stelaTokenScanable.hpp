/// @file
#pragma once

#include "stela/common.hpp"

typedef union ZZSTYPE ZZSTYPE;
typedef struct lloc ZZLTYPE;
typedef void* zzscan_t;

namespace by {

    class stelaParser;

    /** @ingroup stela
     *  @brief Interface for token scanning during stela parsing
     *  @details Provides callback interface for token scanning events.
     *  Used by the parser to process tokens during lexical analysis.
     */
    class _nout stelaTokenScanable {
        BY(ME(stelaTokenScanable))

    public:
        /**
         * @brief Default entry point for token scanning.
         * @details This non-virtual method acts as a wrapper that internally dispatches to the pure virtual `onScan`
         *          method, which must be implemented by concrete scanner strategies.
         * @param ps The stelaParser instance.
         * @param val The YYSTYPE value pointer for the token.
         * @param loc The YYLTYPE location pointer for the token.
         * @param scanner The Flex scanner instance.
         * @return The token ID.
         */
        nint onScan(stelaParser& ps, ZZSTYPE* val, ZZLTYPE* loc, zzscan_t scanner);
        /**
         * @brief Pure virtual method for concrete token scanning implementations.
         * @param ps The stelaParser instance.
         * @param yylval The YYSTYPE value pointer for the token.
         * @param loc The YYLTYPE location pointer for the token.
         * @param yyscanner The Flex scanner instance.
         * @param isBypass Flag indicating if indentation bypass is active.
         * @return The token ID.
         */
        virtual nint onScan(stelaParser& ps, ZZSTYPE* yylval, ZZLTYPE* loc, zzscan_t yyscanner, nbool& isBypass) = 0;
    };
}
