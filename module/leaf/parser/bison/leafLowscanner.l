%top {
/*  ============================================================================================
    |                                          PROLOGUE                                        |
    ============================================================================================  */
#include "leaf/parser/bison/leafLowparser.hpp"
#include "leaf/parser/leafParser.hpp"
#include <iostream>
#ifdef NM_BUILD_PLATFORM_IS_WINDOWS
#   define YY_NO_UNISTD_H 1
#endif

namespace {
    static constexpr int TAB_WIDTH = 4;
    char unescape(const char* seq);
    void append(ZZSTYPE* val, const char* new1);
    void append(ZZSTYPE* val, char new1);
}

#define YY_DECL int zzlexOrigin(ZZSTYPE* yylval_param, ZZLTYPE* yylloc_param, zzscan_t yyscanner)
#define YY_USER_ACTION yylloc->start.row = yylloc->end.row = zzget_lineno(yyscanner); \
                       yylloc->start.col = yylloc->colcnt; yylloc->end.col = yylloc->colcnt+yyleng-1; \
                       yylloc->colcnt += yyleng;
#define PS (*zzget_extra(yyscanner))

typedef ZZSTYPE YYSTYPE;
typedef ZZLTYPE YYLTYPE;
}

/*  ============================================================================================
    |                                      FLEX DEFINITIONS                                    |
    ============================================================================================  */
%option reentrant bison-bridge bison-locations
%option extra-type="by::leafParser*"
%option yylineno
%option nounistd
%option noyywrap
%option prefix="zz"


/*  ============================================================================================
    |                                           STATE                                          |
    ============================================================================================  */
%x stateBlockComment
%x stateComment
%x stateString


/*  ============================================================================================
    |                                           RULES                                          |
    ============================================================================================  */
%%

[\/\.\{] { return PS.onIgnoreIndent(yytext[0]); }
[\]\;\_] { return yytext[0]; }
\} { return '}'; }
[,] { return PS.onTokenComma(yytext[0]); }
[\:] { return PS.onTokenColon(yytext[0]); }
":=" { return DEFASSIGN; }

[ ]+ ;

\t {
    yylloc->start.col = (yylloc->start.col + TAB_WIDTH) & ~(TAB_WIDTH-1);
    yylloc->colcnt = (yylloc->colcnt + TAB_WIDTH) & ~(TAB_WIDTH-1);
}

\n {
    yylloc->colcnt = yylloc->start.col = yylloc->end.col = 0;
    return PS.onTokenNewLine(NEWLINE);
}

def { return DEF; }
int { return _INT_; }
str { return _STR_; }
bool { return _BOOL_; }
byte { return _BYTE_; }
flt { return FLT; }
true {
    yylval->asBool = true;
    NM_DI("scanner: BOOLVAL=true");
    return BOOLVAL;
}

false {
    yylval->asBool = false;
    NM_DI("scanner: BOOLVAL=false");
    return BOOLVAL;
}

[[:digit:]]+ {
    yylval->asInt = std::stoi(yytext, nullptr, 0);
    NM_DI("scanner: INTVAL=%s", yytext);
    return INTVAL;
}

0x[0-9a-fA-F]+ {
    yylval->asInt = std::stoi(yytext, nullptr, 0);
    NM_DI("scanner: INTVAL=%s(%d)", yytext, yylval->asInt);
    return INTVAL;
}

[a-zA-Z][a-zA-Z0-9_]* {
    yylval->asStr = new std::string(yytext);
    NM_DI("scanner: NAME=%s", yytext);
    return NAME;
}

([1-9][0-9]*.[0-9]+|0.[0-9]+) {
    yylval->asFlt = atof(yytext);
    return FLTVAL;
}

([0-9]+.[0-9]+.[0-9]+) {
    yylval->asStr = new std::string(yytext);
    return VERVAL;
}

\" {
    yylval->asStr = nullptr;
    yylloc->strCol = yylloc->start.col;
    BEGIN(PS.pushState(stateString));
}

<stateString>\\([0-9]+) {
    append(yylval, unescape(yytext + 1));
    NM_DI("scanner: STRVAL=%s", yylval->asStr);
}
<stateString>\\0x([0-9a-fA-F]+) {
    append(yylval, unescape(yytext + 1));
    NM_DI("scanner: STRVAL=%s", yylval->asStr);
}
<stateString>\\[a-zA-Z]   {
    NM_DI("\\[\\w] STRVAL=%s", yytext);
    append(yylval, unescape(yytext + 1));
}
<stateString>[^"\\\$]+ {
    NM_DI("[^\"\\$]+ STRVAL=%s", yytext);
    append(yylval, yytext);
}
<stateString>\\\n     { /* A backslash at the end of the line. Do nothing */ }
<stateString>\"       {
    if(!yylval->asStr) yylval->asStr = new std::string();
    yylloc->start.col = yylloc->strCol;
    BEGIN(PS.popState());
    return STRVAL;
}

<<EOF>> {
    NM_DI("scanner: ENDOFFILE");
    yylloc->colcnt = 0;
    return ENDOFFILE;
}

# {
    BEGIN(PS.pushState(stateComment));
}
<stateComment>\n {
    BEGIN(PS.popState());
    yylloc->colcnt = yylloc->start.col = yylloc->end.col = 0;
    return PS.onTokenNewLine(NEWLINE);
}
<stateComment>. {}

## {
    BEGIN(PS.pushState(stateBlockComment));
}
<stateBlockComment>## { BEGIN(PS.popState()); }
<stateBlockComment>. {}

. {
    NM_DI(". yytext=%s", yytext);
    return PS.onScanUnexpected(yytext);
}

%%
/*  ============================================================================================
    |                                         EPILOGUE                                         |
    ============================================================================================  */
int zzlex(ZZSTYPE* yylval_param, ZZLTYPE* yylloc_param, zzscan_t yyscanner) {
    by::leafParser& ps = PS;
    if(!ps.isInit()) // init: tokenScanable
        ps.setScan<by::leafIndentScan>();

    int tok;
    do
        tok = ps.onScan(ps, yylval_param, yylloc_param, yyscanner);
    while(tok == SCAN_AGAIN ||
          tok == SCAN_MODE_INDENT ||
          tok == SCAN_MODE_NORMAL);

    return tok;
}

namespace {
    void append(ZZSTYPE* val, const char* new1) {
        NM_DI("str append: %s += %s", val->asStr, new1);
        if(!val->asStr) {
            val->asStr = new std::string(new1);
            return;
        }

        (*val->asStr) += new1;
    }

    void append(ZZSTYPE* val, char new1) {
        NM_DI("str append: %s += %c", val->asStr, new1);
        if(!val->asStr) {
            val->asStr = new std::string(1, new1);
            return;
        }

        (*val->asStr) += new1;
    }

    char unescape(const char* seq) {
        if(!seq) return '\0';

        NM_DI("unescape(%s)", seq);

        switch(seq[0]) {
            case 'r': return '\r';
            case 'n': return '\n';
            case 't': return '\t';
            case 'f': return '\f';
            case 'a': return '\a';
            case 'v': return '\v';
            case 'b': return '\b';
            // decimal:
            case '1': case '2': case '3': case '4': case '5':
            case '6': case '7': case '8': case '9': case '0':
                return std::stoi(seq, nullptr, 0);

            default:
                return seq[0];
        }
    }
}
