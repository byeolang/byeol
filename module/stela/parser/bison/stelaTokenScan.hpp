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
    /// @details Abstract base class for different token scanning modes.
    /// Provides the foundation for normal and indentation-aware scanning.
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
        friend class stelaParser;

    public:
        using super::onScan;
        nint onScan(stelaParser& ps, ZZSTYPE* yylval, ZZLTYPE* loc, zzscan_t yyscanner,
            nbool& isBypass) override;

    private:
        static stelaNormalScan* _instance;
    };

    /// @ingroup stela
    /// @brief Indentation-aware token scanning mode
    /// @details Specialized token scanning that handles Python-like indentation.
    /// Generates indent and dedent tokens based on whitespace changes.
    class _nout stelaIndentScan: public stelaTokenScan {
        BY(ME(stelaIndentScan, stelaTokenScan))
        friend class stelaParser;

    public:
        using super::onScan;
        nint onScan(stelaParser& ps, ZZSTYPE* yylval, ZZLTYPE* loc, zzscan_t yyscanner,
            nbool& isBypass) override;

    private:
        nint _onIndent(stelaParser& ev, ncnt col, nint tok);
        nint _onDedent(stelaParser& ev, ncnt col, nint tok);

    private:
        static stelaIndentScan* _instance;
    };
} // namespace by
