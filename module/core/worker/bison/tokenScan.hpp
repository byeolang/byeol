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
    /// @details @ref parser dynamically switches scan modes to precisely measure indentation as mentioned in its
    /// documentation. tokenScan represents one such scan mode strategy.
    ///
    /// @section command_tokens Command Tokens
    /// Among tokens, there are command tokens like `SCAN_AGAIN` and `SCAN_EXIT` that give commands to the scanner or
    /// parser. See the `// mode:` section in the middle of `lowparser.y` for details.
    ///
    /// @section is_bypass isBypass
    /// In the case of IndentScan, most tokens are ignored and it focuses only on counting how many spaces there are.
    /// However, if multiple tokens were pushed from the previous line, it operates in bypass mode internally. In this
    /// case, it skips the indentation counting operation and reads and returns the stored tokens as is.
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
