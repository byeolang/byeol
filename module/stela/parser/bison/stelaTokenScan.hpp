/// @file
#pragma once

#include "stela/parser/stelaTokenScanable.hpp"

typedef union ZZSTYPE ZZSTYPE;
typedef struct lloc ZZLTYPE;
typedef void* zzscan_t;

namespace by {

    class stelaParser;
    class stelaTokenDispatcher;

    /// @ingroup stela
    /// @brief Base class for token scanning strategies
    /// @details As mentioned in @ref stelaParser, the parser must dynamically change scan
    /// mode to precisely measure indentation. tokenScan handles one such scan mode strategy.
    ///
    /// @section Command tokens
    /// Among tokens exist command tokens like `SCAN_AGAIN`, `SCAN_EXIT` that give commands
    /// to the scanner or parser. See zztokenType enum in `stelaParser.hpp` for details.
    ///
    /// @section isBypass
    /// IndentScan ignores most tokens and focuses solely on counting whitespace. However,
    /// if multiple tokens were pushed from the previous line, it operates in bypass mode
    /// internally. During this time, it skips indentation counting and simply reads and
    /// returns the stored tokens.
    class _nout stelaTokenScan: public stelaTokenScanable {
        BY(ME(stelaTokenScan))

    public:
        using stelaTokenScanable::onScan;
        virtual nint onScan(stelaParser& ps, ZZSTYPE* yylval, ZZLTYPE* loc, zzscan_t yyscanner,
            nbool& isBypass) override;
    };

    /// @ingroup stela
    /// @brief Normal token scanning mode
    /// @details Standard token scanning that processes tokens without
    /// special indentation handling. Used for regular parsing contexts.
    class _nout stelaNormalScan: public stelaTokenScan {
        BY(ME(stelaNormalScan, stelaTokenScan))

    public:
        using super::onScan;
        nint onScan(stelaParser& ps, ZZSTYPE* yylval, ZZLTYPE* loc, zzscan_t yyscanner, nbool& isBypass) override;

    public:
        static stelaNormalScan instance;
    };

    /// @ingroup stela
    /// @brief Indentation-aware token scanning mode
    /// @details Specialized token scanning that handles Python-like indentation.
    /// Generates indent and dedent tokens based on whitespace changes.
    class _nout stelaIndentScan: public stelaTokenScan {
        BY(ME(stelaIndentScan, stelaTokenScan))

    public:
        using super::onScan;
        nint onScan(stelaParser& ps, ZZSTYPE* yylval, ZZLTYPE* loc, zzscan_t yyscanner, nbool& isBypass) override;

    private:
        nint _onIndent(stelaParser& ev, ncnt col, nint tok);
        nint _onDedent(stelaParser& ev, ncnt col, nint tok);

    public:
        static stelaIndentScan instance;
    };
} // namespace by
