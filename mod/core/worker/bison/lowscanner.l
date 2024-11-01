%top {
/*  ============================================================================================
    |                                          PROLOGUE                                        |
    ============================================================================================  */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"
#include "lowparser.hpp"
#include "../parser.hpp"
#include <iostream>
#ifdef NM_BUILD_PLATFORM_IS_WINDOWS
#   define YY_NO_UNISTD_H 1
#endif
namespace {
    static constexpr int TAB_WIDTH = 4;
    char unescape(const std::string& seq);
    void append(YYSTYPE* val, const char* new1);
}

#define YY_DECL int yylexOrigin(YYSTYPE* yylval_param, YYLTYPE* yylloc_param, yyscan_t yyscanner)
#define YY_USER_ACTION yylloc->start.row = yylloc->end.row = yyget_lineno(yyscanner); \
                       yylloc->start.col = yylloc->colcnt; yylloc->end.col = yylloc->colcnt+yyleng-1; \
                       yylloc->colcnt += yyleng;
#define PS (*yyget_extra(yyscanner))
}


/*  ============================================================================================
    |                                      FLEX DEFINITIONS                                    |
    ============================================================================================  */

%option reentrant bison-bridge bison-locations
%option extra-type="nm::parser*"
%option yylineno
%option nounistd
%option noyywrap


/*  ============================================================================================
    |                                           STATE                                          |
    ============================================================================================  */

%x stateBlockComment
%x stateComment
%x stateString


/*  ============================================================================================
    |                                           RULES                                          |
    ============================================================================================  */

/* reusable pattern: */

/* actual-pattern: */
%%

[\%\*\+\-\/\.\{\[\^\|\&\~\!] { return PS.onIgnoreIndent(yytext[0]); }
[\]<>\;\_\@\$] { return yytext[0]; }
\} {
    if(!PS.onTokenEndOfBraces()) return '}'; // it wasn't str-templating.

    unput('"');
    unput('+'); // inputStream = + "
    unput('r');
    unput('t');
    unput('s');
    unput(' '); // inputStream = str + "
    unput('s');
    unput('a'); // inputStream = as str + "
    return ')';
}
[,] { return PS.onTokenComma(yytext[0]); }
[\:] { return PS.onTokenColon(yytext[0]); }
[\(] { return PS.onTokenLParan(yytext[0]); }
[\)] { return PS.onTokenRParan(yytext[0]); }
\-\- { return DOUBLE_MINUS; }
\+\+ { return DOUBLE_PLUS; }
\.\. { return DOUBLE_DOT; }
\[\] { return OPEN_CLOSE_SQUARE_BRACKET; }
\= { return ASSIGN; }
">=" { return GE; }
"<=" { return LE; }
"==" { return EQ; }
"!=" { return NE; }
":=" { return DEFASSIGN; }
"&&" { return LOGICAL_AND; }
"||" { return LOGICAL_OR; }
"+=" { return ADD_ASSIGN; }
"-=" { return SUB_ASSIGN; }
"*=" { return MUL_ASSIGN; }
"/=" { return DIV_ASSIGN; }
"%=" { return MOD_ASSIGN; }
"|=" { return OR_ASSIGN; }
"&=" { return AND_ASSIGN; }
"^=" { return XOR_ASSIGN; }
"<<" { return LSHIFT; }
">>" { return RSHIFT; }
[ ]+ ;
\t {
    yylloc->start.col = (yylloc->start.col + TAB_WIDTH) & ~(TAB_WIDTH-1);
    yylloc->colcnt = (yylloc->colcnt + TAB_WIDTH) & ~(TAB_WIDTH-1);
}

\n {
    yylloc->colcnt = yylloc->start.col = yylloc->end.col = 0;
    return PS.onTokenNewLine(NEWLINE);
}

[[:digit:]]+ {
    yylval->asInt = std::stoi(yytext, nullptr, 0);
    NM_DI("scanner: INTVAL=%s", yytext);
    return INTVAL;
}

[[:digit:]]+\.[[:digit:]]+ {
    yylval->asFlt = std::stod(yytext);
    NM_DI("scanner: FLTVAL=%f", yylval->asFlt);
    return FLTVAL;
}

if { return IF; }
else { return _ELSE_; }
ret { return RET; }
next { return NEXT; }
break { return BREAK; }
in { return _IN_; }
is { return IS; }
while { return _WHILE_; }
for { return FOR; }
def { return DEF; }
with { return WITH; }
as { return AS; }
enum { return ENUM; }
only { return ONLY; }
end { return END; }
void { return _VOID_; }
int { return _INT_; }
str { return _STR_; }
bool { return _BOOL_; }
byte { return _BYTE_; }
flt { return FLT; }
nul { return NUL; }
pack { return PACK; }
ctor { return CTOR; }
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

<stateString>\\([0-9]+) {
    std::string str(yytext + 1);
    NM_E("\\0-9 yytext=%s", yytext + 1);
    std::string res(1, unescape(str));
    NM_E("\\0-9 res=%s", ges);
    append(yylval, res.c_str());
    NM_DI("scanner: STRVAL=%s", yylval->asStr);
}

<stateString>\\0x([0-9a-fA-F]+) {
    std::string str(yytext + 1);
    NM_E("\\0x yytext=%s", yytext + 1);
    std::string res(1, unescape(str));
    NM_E("\\0x res=%s", res);
    append(yylval, res.c_str());
    NM_DI("scanner: STRVAL=%s", yylval->asStr);
}

\" {
    yylval->asStr = nullptr;
    yylloc->strCol = yylloc->start.col;
    NM_DI("!!!!!!!!! begin string !!!!!!!!!");
    BEGIN(PS.pushState(stateString));
}

<stateString>[^"\\\$]+ {
    NM_DI("!!!!!!!! string = %s !!!!!!", yytext);
    append(yylval, yytext);
}
<stateString>\$[a-zA-Z_]+ {
    std::string text = yytext;
    unput('"'); // inputStream = "
    unput('+'); // inputStream = + "
    unput(')'); // inputStream = ) + "
    unput('r');
    unput('t');
    unput('s'); // inputStream = str) + "
    unput(' ');
    unput('s');
    unput('a'); // inputStream = as str) + "
    unput(' ');
    for(int n= yyleng - 1; n > 0 ;n--) {
        unput(text[n]); // inputStream = <yourText> as str) + "
    }
    unput('('); // inputStream = (<yourtext> as str) + "
    unput('+');
    unput('"'); // inputStream = "+(<yourText> as str)+"
}
<stateString>$\{ {
    unput('(');
    unput('+');
    unput('"'); // inputStream = "+(
    PS.onTokenStartOfStrTemplateBrace();
    // don't return '"' here:
    // we must leave '"' with 'unput()' so that the scanner will exit current 'stateString',
    // when it meets '"' by next token.
}
<stateString>\\[\w\$]   {
    append(yylval, std::string(yytext[1], 1).c_str());
}
<stateString>\\\n     { /* A backslash at the end of the line. Do nothing */ }
<stateString>\"       {
    if(!yylval->asStr) yylval->asStr = new std::string();
    yylloc->start.col = yylloc->strCol;
    NM_DI("!!!!!!!! return string = %s !!!!!!!!", *yylval->asStr);
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

## {
    BEGIN(PS.pushState(stateBlockComment));
}

<stateComment>\n {
    BEGIN(PS.popState());
    yylloc->colcnt = yylloc->start.col = yylloc->end.col = 0;
    return PS.onTokenNewLine(NEWLINE);
}
<stateComment>. {}

<stateBlockComment>## { BEGIN(PS.popState()); }
<stateBlockComment>. {}

. {
    return PS.onScanUnexpected(yytext);
}

%%
/*  ============================================================================================
    |                                         EPILOGUE                                         |
    ============================================================================================  */
int yylex(YYSTYPE* yylval_param, YYLTYPE* yylloc_param, yyscan_t yyscanner) {
    nm::parser& ps = PS;
    if(!ps.isInit()) // init: tokenScanable
        ps.setScan<nm::indentScan>();

    int tok;
    do
        tok = ps.onScan(ps, yylval_param, yylloc_param, yyscanner);
    while(tok == SCAN_AGAIN ||
          tok == SCAN_MODE_INDENT ||
          tok == SCAN_MODE_NORMAL);

    return tok;
}

namespace {
    void append(YYSTYPE* val, const char* new1) {
        if(!val->asStr) {
            val->asStr = new std::string(new1);
            return;
        }

        (*val->asStr) += new1;
    }

    char unescape(const std::string& seq) {
        if(seq.size() < 2) return '\0';

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
            case '6': case '7': case '8': case '9': case '0': {
                char ret = (char) std::stoi(seq, nullptr, 0);
                NM_DI("unescape(%s) = %c", seq, ret);
                return ret;
            }

            default:
                return seq[0];
        }
    }
}
#pragma clang diagnostic pop
