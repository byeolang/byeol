/// @file
#pragma once

#include "core/worker/tokenScanable.hpp"

typedef union YYSTYPE YYSTYPE;
typedef struct lloc YYLTYPE;
typedef void* yyscan_t;

namespace by {

    class parser;
    class tokenDispatcher;

    /// @ingroup core
    /// @brief Base class for token scanning strategies
    /// @details Abstract base class for different token scanning modes in the parser.
    /// Handles token processing and provides foundation for specialized scanning.
    class _nout tokenScan: public tokenScanable, public typeProvidable, public clonable {
        BY(ADT(tokenScan))

    public:
        using tokenScanable::onScan;
        virtual nint onScan(parser& ps, YYSTYPE* yylval, YYLTYPE* loc, yyscan_t yyscanner,
            nbool& isBypass) override;
    };

    /// @ingroup core
    /// @brief Normal token scanning mode
    /// @details Standard token scanning without special indentation handling.
    /// Used for regular parsing contexts.
    class _nout normalScan: public tokenScan {
        BY(CLASS(normalScan, tokenScan))

    public:
        using super::onScan;
        nint onScan(parser& ps, YYSTYPE* yylval, YYLTYPE* loc, yyscan_t yyscanner,
            nbool& isBypass) override;

    public:
        static normalScan instance;
    };

    /// @ingroup core
    /// @brief Indentation-aware token scanning mode
    /// @details Specialized scanning that handles Python-like indentation.
    /// Generates indent and dedent tokens based on whitespace changes.
    class _nout indentScan: public tokenScan {
        BY(CLASS(indentScan, tokenScan))

    public:
        using super::onScan;
        nint onScan(parser& ps, YYSTYPE* yylval, YYLTYPE* loc, yyscan_t yyscanner,
            nbool& isBypass) override;

    private:
        nint _onIndent(parser& ev, ncnt col, nint tok);
        nint _onDedent(parser& ev, ncnt col, nint tok);

    public:
        static indentScan instance;
    };
} // namespace by
