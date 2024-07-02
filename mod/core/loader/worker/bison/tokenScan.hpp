#pragma once

#include "../tokenScanable.hpp"

typedef union YYSTYPE YYSTYPE;
typedef struct lloc YYLTYPE;
typedef void* yyscan_t;

namespace nm {

    class parser;
    class tokenDispatcher;

    class _nout tokenScan : public tokenScanable, public typeProvidable, public clonable {
        NAMU(ADT(tokenScan))

    public:
        using tokenScanable::onScan;
        virtual nint onScan(parser& ps, YYSTYPE* yylval, YYLTYPE* loc, yyscan_t yyscanner, nbool& isBypass) override;

    private:
        nbool _useSmartDedent;
    };

    class _nout normalScan: public tokenScan {
        NAMU(CLASS(normalScan, tokenScan))
        friend class parser;

    public:
        using super::onScan;
        nint onScan(parser& ps, YYSTYPE* yylval, YYLTYPE* loc, yyscan_t yyscanner, nbool& isBypass) override;

    private:
        static normalScan* _instance;
    };

    class _nout indentScan : public tokenScan {
        NAMU(CLASS(indentScan, tokenScan))
        friend class parser;

    public:
        using super::onScan;
        nint onScan(parser& ps, YYSTYPE* yylval, YYLTYPE* loc, yyscan_t yyscanner, nbool& isBypass) override;

    private:
        nint _onIndent(parser& ev, ncnt col, nint tok);
        nint _onDedent(parser& ev, ncnt col, nint tok);

    private:
        static indentScan* _instance;
    };
}
