/// @file
#pragma once

#include "core/type/ntype.hpp"

typedef union YYSTYPE YYSTYPE;
typedef struct lloc YYLTYPE;
typedef void* yyscan_t;

namespace by {

    class parser;

    /** @ingroup core
     *  @brief Interface for token scanning functionality
     *  @details Defines the contract for token scanning implementations.
     *  Provides hooks for custom token processing during parsing.
     */
    class _nout tokenScanable {
        BY(ME(tokenScanable))

    public:
        /**
         * @brief Default entry point for token scanning.
         * @details This non-virtual method acts as a wrapper that internally dispatches to the pure virtual `onScan`
         *          method, which must be implemented by concrete scanner strategies.
         * @param ps The parser instance.
         * @param val The YYSTYPE value pointer for the token.
         * @param loc The YYLTYPE location pointer for the token.
         * @param scanner The Flex scanner instance.
         * @return The token ID.
         */
        nint onScan(parser& ps, YYSTYPE* val, YYLTYPE* loc, yyscan_t scanner);
        /**
         * @brief Pure virtual method for concrete token scanning implementations.
         * @param ps The parser instance.
         * @param yylval The YYSTYPE value pointer for the token.
         * @param loc The YYLTYPE location pointer for the token.
         * @param yyscanner The Flex scanner instance.
         * @param isBypass Flag indicating if indentation bypass is active.
         * @return The token ID.
         */
        virtual nint onScan(parser& ps, YYSTYPE* yylval, YYLTYPE* loc, yyscan_t yyscanner, nbool& isBypass) = 0;
    };
}
