/// @file
#pragma once

#include "leaf/parser/leafTokenScanable.hpp"

typedef union ZZSTYPE ZZSTYPE;
typedef struct lloc ZZLTYPE;
typedef void* zzscan_t;

namespace by {

    class leafParser;
    class leafTokenDispatcher;

    class _nout leafTokenScan: public leafTokenScanable {
        BY(ME(leafTokenScan))

    public:
        using leafTokenScanable::onScan;
        virtual nint onScan(leafParser& ps, ZZSTYPE* yylval, ZZLTYPE* loc, zzscan_t yyscanner,
            nbool& isBypass) override;
    };

    class _nout leafNormalScan: public leafTokenScan {
        BY(ME(leafNormalScan, leafTokenScan))
        friend class leafParser;

    public:
        using super::onScan;
        nint onScan(leafParser& ps, ZZSTYPE* yylval, ZZLTYPE* loc, zzscan_t yyscanner,
            nbool& isBypass) override;

    private:
        static leafNormalScan* _instance;
    };

    class _nout leafIndentScan: public leafTokenScan {
        BY(ME(leafIndentScan, leafTokenScan))
        friend class leafParser;

    public:
        using super::onScan;
        nint onScan(leafParser& ps, ZZSTYPE* yylval, ZZLTYPE* loc, zzscan_t yyscanner,
            nbool& isBypass) override;

    private:
        nint _onIndent(leafParser& ev, ncnt col, nint tok);
        nint _onDedent(leafParser& ev, ncnt col, nint tok);

    private:
        static leafIndentScan* _instance;
    };
} // namespace by
