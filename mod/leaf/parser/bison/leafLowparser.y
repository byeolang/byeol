%code top {
/*  ============================================================================================
    |                                          PROLOGUE                                        |
    ============================================================================================  */
    #include <iostream>
    #include <sstream>
    #include "../leafParser.hpp"
    using namespace nm;

    #define YYLLOC_DEFAULT(Current, Rhs, N) \
        do { \
          if(N) { \
              (Current).start = YYRHSLOC (Rhs, 1).start; \
              (Current).end = YYRHSLOC (Rhs, N).end; \
          } else { \
              (Current).start.row = (Current).end.row = YYRHSLOC (Rhs, 0).end.row; \
              (Current).start.col = (Current).end.col = YYRHSLOC (Rhs, 0).end.col; \
          } \
        } while(0)

    #define PS (*zzget_extra(scanner))
}

/*  ============================================================================================
    |                                          C++ DECLS                                       |
    ============================================================================================  */

%code requires {
    #include "../../ast/point.hpp"

    typedef void* zzscan_t;

    namespace nm {
        class leaf;
    }

    struct lloc : public nm::area {
        int colcnt;
        int strCol; // column position when it starts to parse string.

        void rel() {
            colcnt = 0;
            nm::area::rel();
        }
    };
}

%code provides {
    extern int zzlineno;
    extern char* yytext;
    namespace nm {
        class leafParser;
    }

    extern "C" {
        int zzlex(ZZSTYPE* val, ZZLTYPE* loc, zzscan_t scanner);
        void zzset_lineno(int linenumber, zzscan_t scanner);
        nm::leafParser* zzget_extra(zzscan_t scanner);
        char* zzget_text(zzscan_t scanner);
        void zzerror(ZZLTYPE* loc, zzscan_t scanner, const char* msg);
    }

    void _onEndParse(zzscan_t scanner);
    std::string getTokenName(int tok);
}

/*  ============================================================================================
    |                                         BISON FLAGS                                      |
    ============================================================================================  */

%union {
    int asInt;
    float asFlt;
    bool asBool;
    char asChar;
    std::string* asStr;
    nm::leaf* asLeaf;
}

%define api.pure
%define parse.error custom
%glr-parser
%locations
%define api.prefix {zz}

%lex-param {zzscan_t scanner}
%parse-param {zzscan_t scanner}
%define api.location.type {lloc}
%expect 0
%require "3.8.1"

/*  ============================================================================================
    |                                        BISON SYMBOLS                                     |
    ============================================================================================  */
%start compilation-unit

// mode:
%token SCAN_AGAIN SCAN_EXIT SCAN_MODE_NORMAL SCAN_MODE_INDENT SCAN_MODE_INDENT_IGNORE SCAN_MODE_END

// terminal:
//  reserved-keyword:
//      use prefix '_' on some tokens for windows compatibility.
%token DEF _IN_
//  predefined-type:
%token _INT_ _STR_ _BOOL_ FLT NUL _BYTE_
//  valueless-token:
%token NEWLINE INDENT DEDENT ENDOFFILE DOUBLE_DOT TAB DEFASSIGN
%token OPEN_CLOSE_SQUARE_BRACKET
//  value-holding-token:
%token <asInt> INTVAL
%token <asFlt> FLTVAL
%token <asBool> BOOLVAL
%token <asStr> NAME STRVAL VERVAL

// nonterminal:
//  basic component:
%type <asLeaf> compilation-unit
//      expr:
//          normal:
%type <asLeaf> expr-inline expr-inline5 unary
//          def:
%type <asLeaf> def-expr-inline def-expr-compound
//      stmt:
%type <asLeaf> def-stmt
//      block:
%type <asLeaf> indentDefBlock defblock
%type <asLeaf> def-stmt-chain
//      tuple:
%type <asLeaf> tuple-items tuple-item
//      type:
%type <asLeaf> access type
//  keyword:
//      define:
//          value:
%type <asLeaf> def-prop-inline def-prop-value
//          obj:
%type <asLeaf> def-obj def-obj-default
//              container:
%type <asLeaf> def-array-value

/*  ============================================================================================
    |                                     OPERATOR PRECEDENCE                                  |
    ============================================================================================  */


/*  ============================================================================================
    |                                           RULES                                          |
    ============================================================================================  */
%%

// basic component:
unary: INTVAL { $$ = PS.onPrimitive<nint>($1); }
       | STRVAL {
        $$ = PS.onPrimitive<std::string>(*$1);
        delete $1;
     } | VERVAL {
        $$ = PS.onVer(*$1);
        delete $1;
     } | FLTVAL { $$ = PS.onPrimitive<nflt>($1); }
       | BOOLVAL { $$ = PS.onPrimitive<nbool>($1); }
       | NUL {
        // ??
     } | def-array-value { $$ = $1; }
       | access { $$ = $1; }

//  expr:
//      inline:
expr-inline5: unary { $$ = $1; }
            | unary DOUBLE_DOT unary { /*TODO:*/ }
expr-inline: expr-inline5 { $$ = $1; }

indentDefBlock: NEWLINE INDENT defblock DEDENT { $$ = $3; }
              | ':' def-stmt-chain NEWLINE { $$ = $2; }
              | ':' def-expr-compound { $$ = PS.onDefBlock(*$2); }
              | ':' def-stmt-chain ';' def-expr-compound {
                tstr<leaf> exprLife($4);
                $$ = PS.onDefBlock(*$2, *exprLife);
            } | ':' ';' NEWLINE { $$ = PS.onDefBlock(); }

defblock: def-stmt { $$ = PS.onDefBlock(*$1); }
        | defblock def-stmt {
        tstr<leaf> lifeStmt($2);
        $$ = PS.onDefBlock(*$1, *lifeStmt);
      }

//      def:
def-expr-inline: def-prop-inline { $$ = $1; }
def-expr-compound: def-obj { $$ = $1; }
def-stmt: def-expr-inline NEWLINE { $$ = $1; }
        | def-expr-compound { $$ = $1; }
//      access:
def-stmt-chain: def-expr-inline { $$ = PS.onDefBlock(*$1); }
              | def-stmt-chain ';' def-expr-inline {
                tstr<leaf> lifeItem($3);
                $$ = PS.onDefBlock(*$1, *lifeItem);
            }

//  type:
//      access:
type: _INT_ { $$ = PS.onPrimitive<nint>(0); }
    | _BYTE_ { $$ = PS.onPrimitive<nuchar>(0); }
    | _STR_ { $$ = PS.onPrimitive<std::string>(""); }
    | _BOOL_ { $$ = PS.onPrimitive<nbool>(false); }
    | FLT { $$ = PS.onPrimitive<nflt>(0.0f); }

//  keyword:
//      define:
//          value:
def-prop-inline: def-prop-value { $$ = $1; }
def-prop-value: NAME DEFASSIGN expr-inline {
                $$ = PS.onDefAssign(*$1, *$3);
                delete $1;
            }

//          obj:
def-obj: def-obj-default { $$ = $1; }
def-obj-default: DEF NAME indentDefBlock {
                $$ = PS.onDefOrigin(std::string(*$2), *$3);
                delete $2;
             }

//  access:
access: type { $$ = $1; }

//              container:
tuple-item: expr-inline { $$ = $1; }
tuple-items: tuple-item { $$ = PS.onDefArray(*$1); }
           | tuple-items ',' tuple-item { $$ = PS.onDefArray(*$1, *$3); }
def-array-value: '{' tuple-items '}' { $$ = $2; }

compilation-unit: defblock { $$ = PS.onCompilationUnit(*$1); }

%%

/*  ============================================================================================
    |                                         EPILOGUE                                         |
    ============================================================================================  */

static std::string traceErr(const yypcontext_t* ctx, zzscan_t scanner) {
    constexpr nint TOKEN_MAX = 5;
    yysymbol_kind_t tokens[TOKEN_MAX];
    ncnt expected = yypcontext_expected_tokens(ctx, tokens, TOKEN_MAX);
    if(expected <= 0)
        return "nothing";

    std::stringstream ss;
    ss << "'" << yysymbol_name(tokens[0]) << "'";
    for(int n=1; n < expected ;n++)
        ss << ", '" << yysymbol_name(tokens[n]) << "'";
    return ss.str();

    // TODO: high quality error trace..
}

// when bison claims that it can't parse any further, this func will be called.
// it means that error recovery has been failed already.
static int yyreport_syntax_error(const yypcontext_t* ctx, zzscan_t scanner) {
    yysymbol_kind_t symbol = yypcontext_token(ctx);
    PS.onParseErr(traceErr(ctx, scanner), yysymbol_name(symbol));

    return _onEndParse(scanner), 0;
}

void _onEndParse(zzscan_t scanner) {
    zzset_lineno(0, scanner);
}

std::string getLeafTokenName(int tok) {
    if(tok <= 127) return std::string(1, tok);

    auto token = YYTRANSLATE(tok);
    return std::string(yysymbol_name(token));
}

// errors except syntax will come here. for instance, when available memory doesn't exist.
void zzerror(ZZLTYPE* loc, zzscan_t scanner, const char* msg) {
    PS.report(msg);
}
