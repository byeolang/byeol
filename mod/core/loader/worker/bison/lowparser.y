%code top {
/*  ============================================================================================
    |                                          PROLOGUE                                        |
    ============================================================================================  */
    #include <iostream>
    #include <sstream>
    using std::cout;
    #include "../parser.hpp"
    #include "../../../builtin.hpp"
    using namespace namu;

    #define YYLLOC_DEFAULT(Current, Rhs, N) \
        do { \
          if(N) { \
              (Current).start = YYRHSLOC (Rhs, 1).start; \
              (Current).end = YYRHSLOC (Rhs, N).end; \
          } else { \
              (Current).start.row = (Current).end.row = YYRHSLOC (Rhs, 0).end.row; \
              (Current).start.col = (Current).end.col = YYRHSLOC (Rhs, 0).end.col; \
          } \
          PS.onSrcArea(Current); \
        } while(0)

    #define PS (*yyget_extra(scanner))
}

/*  ============================================================================================
    |                                          C++ DECLS                                       |
    ============================================================================================  */

%code requires {
    #include "../../../ast/point.hpp"
    #include "../../../ast/args.hpp"
    #include "../defBlock.hpp"

    typedef void* yyscan_t;

    namespace namu {
        class node;
        class obj;
        class immutableTactic;
        template <typename K, typename V, typename TACTIC> class tnmap;
        typedef tstr<node> str;
        template <typename T, typename WRP> class tnarr;
        typedef tnarr<node> narr;
    }

    struct lloc : public namu::area {
        int colcnt;

        void rel() {
            colcnt = 0;
            namu::area::rel();
        }
    };
}

%code provides {
    extern int yylineno;
    extern char* yytext;
    namespace namu {
        class parser;
    }

    extern "C" {
        int yylex(YYSTYPE* val, YYLTYPE* loc, yyscan_t scanner);
        void yyset_lineno(int linenumber, yyscan_t scanner);
        namu::parser* yyget_extra(yyscan_t scanner);
        char* yyget_text(yyscan_t scanner);
        void yyerror(YYLTYPE* loc, yyscan_t scanner, const char* msg);
    }

    void _onEndParse(yyscan_t scanner);
    std::string getTokenName(int tok);
}

/*  ============================================================================================
    |                                         BISON FLAGS                                      |
    ============================================================================================  */

%union {
    namu::nchar asChar;
    int asInt;
    std::string* asStr;
    bool asBool;
    float asFlt;
    namu::node* asNode;
    namu::obj* asObj;
    namu::narr* asNarr;
    namu::scope* asScope;
    namu::defBlock* asDefBlock;
    namu::args* asArgs;
}

%define api.pure
%define parse.error custom
%glr-parser
%locations

%lex-param {yyscan_t scanner}
%parse-param {yyscan_t scanner}
%define api.location.type {lloc}
%expect 9
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
//      branch:
%token IF _ELSE_ RET NEXT BREAK
//      check:
%token _IN_ IS
//      loop:
%token _WHILE_ FOR
//      define:
%token DEF WITH AS ENUM ONLY END
//  predefined-type:
%token _VOID_ _INT_ _STR_ _BOOL_ FLT NUL _BYTE_ _CHAR_ ME SUPER IT CTOR PACK 
//  valueless-token:
%token NEWLINE INDENT DEDENT ENDOFFILE DOUBLE_MINUS DOUBLE_PLUS DOUBLE_DOT ARROW TAB ASSIGN DEFASSIGN
%token OPEN_CLOSE_SQUARE_BRACKET GE LE EQ NE LOGICAL_AND LOGICAL_OR LSHIFT RSHIFT
%token ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN OR_ASSIGN AND_ASSIGN XOR_ASSIGN
//  value-holding-token:
%token <asChar> CHARVAL
%token <asInt> INTVAL
%token <asFlt> FLTVAL
%token <asBool> BOOLVAL
%token <asStr> NAME STRVAL

// nonterminal:
//  basic component:
%type <asNode> compilation-unit unary postfix primary
%type <asStr> visibility
//      expr:
//          normal:
%type <asNode> expr-inline expr-inline1 expr-inline2 expr-inline3 expr-inline4 expr-inline5 expr-inline6 expr-inline7 expr-inline8 expr-inline9
%type <asNode> expr-compound all-expr-compound
//          def:
%type <asNode> def-expr-inline def-expr-compound
//      stmt:
//          normal:
%type <asNode> allstmt stmt stmt-inline stmt-compound
%type <asNode> allstmt-chain allstmt-chain-item
//          def:
%type <asNode> def-stmt
//          decl:
//      block:
%type<asNode> block indentblock
//          normal:
//          def:
%type <asDefBlock> indentDefBlock defblock
%type <asDefBlock> def-stmt-chain
//          decl:
%type <asDefBlock> declBlock indentDeclBlock
//      access:
%type <asNode> access call-access
//      func:
%type <asNode> func-call
//      tuple:
%type <asArgs> tuple tuple-items
%type <asArgs> func-call-tuple func-call-tuple-items
%type <asNode> func-call-tuple-item tuple-item
%type <asNarr> params param-items 
//      type:
%type <asNode> type
%type <asArgs> typenames typeparams
//  keyword:
//      branch:
%type <asNode> if ret-inline ret-compound next break
%type <asNode> matching matchers matcher-item
%type <asNarr> matcher-equal-rhs
//      loop
%type <asNode> while for
//      define:
//          value:
%type <asNode> def-prop-inline def-prop-value def-prop-without-value def-prop-accessor def-prop-accessor-item
%type <asNarr> def-prop-accessor-items
%type <asNode> def-prop-compound
//          func:
%type <asNode> abstract-func def-func end
%type <asNode> lambda lambda-default lambda-deduction
//          obj:
%type <asNode> def-obj def-obj-default def-obj-default-generic
//              container:
%type <asNode> def-array-value
//              with:
%type <asNode> with-inline with-compound
//  predefined-type:
%type <asObj> pack

/*  ============================================================================================
    |                                     OPERATOR PRECEDENCE                                  |
    ============================================================================================  */
%precedence IF
%precedence _ELSE_






/*  ============================================================================================
    |                                           RULES                                          |
    ============================================================================================  */
%%

// basic component:
compilation-unit: pack defblock {
                tstr<obj> pak(*$1);
                tstr<defBlock> lifeBlock($2);
                PS.onCompilationUnit(*pak, *lifeBlock);
                _onEndParse(scanner);
              } | pack {
                tstr<obj> pak(*$1);
                PS.onCompilationUnit(*pak);
                _onEndParse(scanner);
              }

// basic component:
unary: postfix { $$ = $1; }
     | DOUBLE_PLUS unary { $$ = PS.onUnaryDoublePlus(*$2); }
     | DOUBLE_MINUS unary { $$ = PS.onUnaryDoubleMinus(*$2); }
     | '+' unary { $$ = $2; }
     | '-' unary { $$ = PS.onUnaryMinus(*$2); }
     | '!' unary { $$ = PS.onUnaryNot(*$2); }
     | '~' unary { $$ = PS.onUnaryBitwiseNot(*$2); }

postfix: primary { $$ = $1; }
       | postfix DOUBLE_MINUS { $$ = PS.onUnaryPostfixDoubleMinus(*$1); }
       | postfix DOUBLE_PLUS { $$ = PS.onUnaryPostfixDoublePlus(*$1); }
       | postfix '.' access { $$ = PS.onGet(*$1, *$3); }
       | postfix '.' func-call {
        $$ = PS.onFillFromOfFuncCall(*$1, $3->cast<runExpr>());
     } | func-call { $$ = $1; }
       | postfix '[' expr-inline9 ']' { $$ = PS.onGetElem(*$1, *$3); }

primary: INTVAL { $$ = PS.onPrimitive<nInt>($1); }
       | STRVAL {
        $$ = PS.onPrimitive<nStr>(*$1);
        free($1);
     } | FLTVAL { $$ = PS.onPrimitive<nFlt>($1); }
       | BOOLVAL { $$ = PS.onPrimitive<nBool>($1); }
       | CHARVAL { $$ = PS.onPrimitive<nChar>($1); }
       | tuple { $$ = PS.onParanthesisAsTuple(*$1); }
       | NUL {
        // ??
     } | def-array-value { $$ = $1; }
       | access { $$ = $1; }
visibility: '_' '+' {
            // ??
        } | '+' '_' {
            // ??
        } | '_' {
            // ??
        } | '+' {
            // ??
        } | %empty {
            // ??
        }

//  expr:
//      inline:
expr-inline9: expr-inline8 { $$ = $1; }
          | expr-inline9 LOGICAL_OR expr-inline8 { $$ = PS.onOr(*$1, *$3); }
          | expr-inline9 LOGICAL_AND expr-inline8 { $$ = PS.onAnd(*$1, *$3); }
expr-inline8: expr-inline7 { $$ = $1; }
          | expr-inline8 LSHIFT expr-inline7 { $$ = PS.onLShift(*$1, *$3); }
          | expr-inline8 RSHIFT expr-inline7 { $$ = PS.onRShift(*$1, *$3); }
expr-inline7: expr-inline6 { $$ = $1; }
          | expr-inline6 '>' expr-inline6 { $$ = PS.onGt(*$1, *$3); }
          | expr-inline6 '<' expr-inline6 { $$ = PS.onLt(*$1, *$3); }
          | expr-inline6 GE expr-inline6 { $$ = PS.onGe(*$1, *$3); }
          | expr-inline6 LE expr-inline6 { $$ = PS.onLe(*$1, *$3); }
          | expr-inline6 EQ expr-inline6 { $$ = PS.onEq(*$1, *$3); }
          | expr-inline6 NE expr-inline6 { $$ = PS.onNe(*$1, *$3); }
expr-inline6: expr-inline5 { $$ = $1; }
          | expr-inline6 '|' expr-inline5 { $$ = PS.onBitwiseOr(*$1, *$3); }
          | expr-inline6 IS type {
            // ??
        } | expr-inline6 _IN_ expr-inline5 { $$ = PS.onIn(*$1, *$3); }
expr-inline5: expr-inline4 { $$ = $1; }
          | expr-inline5 '^' expr-inline4 { $$ = PS.onBitwiseXor(*$1, *$3); }
expr-inline4: expr-inline3 { $$ = $1; }
          | expr-inline4 '&' expr-inline3 { $$ = PS.onBitwiseAnd(*$1, *$3); }
expr-inline3: expr-inline2 { $$ = $1; }
          | expr-inline3 '+' expr-inline2 { $$ = PS.onAdd(*$1, *$3); }
          | expr-inline3 '-' expr-inline2 { $$ = PS.onSub(*$1, *$3); }
expr-inline2: expr-inline1 { $$ = $1; }
          | expr-inline2 '*' expr-inline1 { $$ = PS.onMul(*$1, *$3); }
          | expr-inline2 '/' expr-inline1 { $$ = PS.onDiv(*$1, *$3); }
          | expr-inline2 '%' expr-inline1 { $$ = PS.onMod(*$1, *$3); }
expr-inline1: unary { $$ = $1; }
          | expr-inline1 AS type { $$ = PS.onAs(*$1, *$3); }
          | unary DOUBLE_DOT unary { $$ = PS.onDefSeq(*$1, *$3); }
//      compound:
expr-compound: if { $$ = $1; }
             | expr-inline9 ASSIGN expr-compound { $$ = PS.onAssign(*$1, *$3); }
             | for { $$ = $1; }
             | while { $$ = $1; }
             | end { $$ = $1; }

block: allstmt { $$ = PS.onBlock(*$1); }
     | block allstmt { $$ = PS.onBlock($1->cast<blockExpr>(), *$2); }

indentblock: NEWLINE INDENT block DEDENT { $$ = $3; }
           | ':' allstmt-chain NEWLINE { $$ = $2; }
           | ':' all-expr-compound { $$ = PS.onBlock(*$2); }
           | ':' allstmt-chain ';' all-expr-compound {
            $$ = PS.onBlock($2->cast<blockExpr>(), *$4);
         } | ':' ';' NEWLINE { $$ = PS.onBlock(); }

indentDefBlock: NEWLINE INDENT defblock DEDENT { $$ = $3; }
              | ':' def-stmt-chain NEWLINE { $$ = $2; }
              | ':' def-expr-compound { $$ = PS.onDefBlock(*$2); }
              | ':' def-stmt-chain ';' def-expr-compound {
                str exprLife($4);
                $$ = PS.onDefBlock(*$2, *exprLife);
            } | ':' ';' NEWLINE { $$ = PS.onDefBlock(); }

defblock: def-stmt { $$ = PS.onDefBlock(*$1); }
        | defblock def-stmt {
        str lifeStmt($2);
        $$ = PS.onDefBlock(*$1, *lifeStmt);
      }

declBlock: access NEWLINE {
            // ??
       } | declBlock access NEWLINE {
            // ??
       }
indentDeclBlock: NEWLINE INDENT declBlock DEDENT {
                // ??
             } | ':' allstmt-chain NEWLINE {
                // TODO: allstmt should be declstmt.
                // for preventing reduce/reduce conflict, I need to declare allstmt on here rule.
                // ??
             }

//  stmt:
//      normal:
expr-inline: expr-inline9 { $$ = $1; }
         | expr-inline9 ASSIGN expr-inline { $$ = PS.onAssign(*$1, *$3); }
         | expr-inline9 ADD_ASSIGN expr-inline9 { $$ = PS.onAddAssign(*$1, *$3); }
         | expr-inline9 SUB_ASSIGN expr-inline9 { $$ = PS.onSubAssign(*$1, *$3); }
         | expr-inline9 MUL_ASSIGN expr-inline9 { $$ = PS.onMulAssign(*$1, *$3); }
         | expr-inline9 DIV_ASSIGN expr-inline9 { $$ = PS.onDivAssign(*$1, *$3); }
         | expr-inline9 MOD_ASSIGN expr-inline9 { $$ = PS.onModAssign(*$1, *$3); }
         | ret-inline { $$ = $1; }
         | break { $$ = $1; }
         | next { $$ = $1; }
stmt: stmt-inline { $$ = $1; }
    | stmt-compound { $$ = $1; }
stmt-inline: expr-inline NEWLINE { $$ = $1; }
stmt-compound: ret-compound { $$ = $1; }
             | expr-compound { $$ = $1; }
             | matching { $$ = $1; }

//      def:
def-expr-inline: with-inline { $$ = $1; }
               | def-prop-inline { $$ = $1; }
               | abstract-func { $$ = $1; }
def-expr-compound: with-compound { $$ = $1; }
                 | def-obj { $$ = $1; }
                 | def-func { $$ = $1; }
                 | def-prop-compound { $$ = $1; }
def-stmt: def-expr-inline NEWLINE { $$ = $1; }
        | def-expr-compound { $$ = $1; }
def-stmt-chain: def-expr-inline { $$ = PS.onDefBlock(*$1); }
              | def-stmt-chain ';' def-expr-inline {
                str lifeItem($3);
                $$ = PS.onDefBlock(*$1, *lifeItem);
            }

//      all:
all-expr-compound: expr-compound { $$ = $1; }
                 | def-expr-compound { $$ = $1; }
allstmt: stmt { $$ = $1; }
       | def-stmt { $$ = $1; }
allstmt-chain: allstmt-chain-item { $$ = PS.onBlock(*$1); }
             | allstmt-chain ';' allstmt-chain-item {
                $$ = PS.onBlock($1->cast<blockExpr>(), *$3);
           }
allstmt-chain-item: expr-inline { $$ = $1; }
                  | def-expr-inline { $$ = $1; }

//  access:
access: call-access { $$ = $1; }
      | type { $$ = $1; }
call-access: type params {
            tstr<narr> paramsLife(*$2);
            $$ = PS.onCallAccess(*$1, *paramsLife);
         }

//  func:
func-call: type func-call-tuple {
        tstr<narr> argsLife($2);
        str typeLife($1);
        $$ = PS.onRunExpr(*typeLife, *argsLife);
      }

//  tuple:
tuple: '(' tuple-items ')' { $$ = $2; }
     | '(' ')' { $$ = PS.onTuple(); }
tuple-item: expr-inline9 { $$ = $1; }
          | expr-compound { $$ = $1; }
tuple-items: tuple-item { $$ = PS.onTuple(*$1); }
           | tuple-items ',' tuple-item { $$ = PS.onTuple(*$1, *$3); }
func-call-tuple: '(' func-call-tuple-items ')' { $$ = $2; }
               | '(' ')' { $$ = PS.onFuncCallTuple(); }
func-call-tuple-item: expr-inline9 { $$ = $1; }
                    | expr-compound { $$ = $1; }
                    | lambda {
                        // ??
                  }
func-call-tuple-items: func-call-tuple-item {
                        $$ = PS.onFuncCallTuple(*$1);
                   } | func-call-tuple-items ',' func-call-tuple-item {
                        $$ = PS.onFuncCallTuple(*$1, *$3);
                   }
params: '(' _VOID_ ')' { $$ = PS.onParams(); }
      | '(' param-items ')' { $$ = $2; }
param-items: def-prop-without-value {
            $$ = PS.onParams($1->cast<defPropExpr>());
         } | param-items ',' def-prop-without-value {
            $$ = PS.onParams(*$1, $3->cast<defPropExpr>());
         }

//  type:
type: _VOID_ { $$ = PS.onPrimitive<nVoid>(); }
    | _INT_ { $$ = PS.onPrimitive<nInt>(); }
    | _BYTE_ { $$ = PS.onPrimitive<nByte>(); }
    | _CHAR_ { $$ = PS.onPrimitive<nChar>(); }
    | _STR_ { $$ = PS.onPrimitive<nStr>(); }
    | _BOOL_ { $$ = PS.onPrimitive<nBool>(); }
    | FLT { $$ = PS.onPrimitive<nFlt>(); }
    | NAME {
        $$ = PS.onGet(*$1);
        free($1);
  } | type OPEN_CLOSE_SQUARE_BRACKET { $$ = PS.onGetArray(*$1); }
    | NAME typeparams {
        tstr<args> argsLife($2);
        $$ = PS.onGetGeneric(*$1, *argsLife);
        free($1);
  }

typeparams: '<' typenames '>' { $$ = $2; }
typenames: type { $$ = PS.onTypeNames(*$1); }
         | typenames ',' type { $$ = PS.onTypeNames(*$1, *$3); }

//  keyword:
//      branch:
if: IF expr-inline9 indentblock {
    $$ = PS.onIf(*$2, $3->cast<blockExpr>());
} | IF expr-inline9 indentblock _ELSE_ indentblock {
    $$ = PS.onIf(*$2, $3->cast<blockExpr>(), $5->cast<blockExpr>());
} | IF expr-inline9 indentblock _ELSE_ if {
    $$ = PS.onIf(*$2, $3->cast<blockExpr>(), $5->cast<ifExpr>());
}

ret-inline: RET { $$ = PS.onRet(); }
          | RET expr-inline9 { $$ = PS.onRet(*$2); }
ret-compound: RET expr-compound { $$ = PS.onRet(*$2); }

next: NEXT { $$ = PS.onNext(); }

break: BREAK { $$ = PS.onBreak(); }

matching: expr-inline9 NEWLINE INDENT matchers DEDENT {
            // ??
      }
matchers: matcher-item {
            // ??
      } | matchers matcher-item {
            // ??
      }
matcher-item: _IN_ expr-inline5 indentblock {
                // ??
          } | IS type indentblock {
                // ??
          } | matcher-equal-rhs indentblock {
                // ??
          } | '>' expr-inline6 indentblock {
                // ??
          } | '<' expr-inline6 indentblock {
                // ??
          } | GE expr-inline6 indentblock {
                // ??
          } | LE expr-inline6 indentblock {
                // ??
          } | EQ expr-inline6 indentblock {
                // ??
          } | NE expr-inline6 indentblock {
                // ??
          }
matcher-equal-rhs: expr-inline9 {
                    // ??
               } | matcher-equal-rhs ',' expr-inline9 {
                    // ??
               }

//      loop:
while: _WHILE_ expr-inline9 indentblock {
     $$ = PS.onWhile(*$2, $3->cast<blockExpr>());
   }

for: FOR NAME _IN_ expr-inline9 indentblock {
    $$ = PS.onFor(std::string(*$2), *$4, $5->cast<blockExpr>());
    free($2);
 }

//      define:
//          value:
def-prop-inline: def-prop-without-value { $$ = $1; }
              | def-prop-value { $$ = $1; }
def-prop-without-value: NAME type { // exp means 'explicitly'
                        $$ = PS.onDefProp(*$1, *$2);
                        free($1);
                    }
def-prop-value: NAME DEFASSIGN expr-inline9 {
              $$ = PS.onDefProp(*$1, *$3);
              free($1);
          }
def-prop-accessor: NEWLINE INDENT def-prop-accessor-items DEDENT {
                   // ??
               }
def-prop-accessor-item: visibility NAME indentblock {
                        // ??
                        // TODO: NAME should 'get' or 'set'
                    }
def-prop-accessor-items: def-prop-accessor-item {
                        // ??
                     } | def-prop-accessor-items def-prop-accessor-item {
                        // ??
                     }

def-prop-compound: NAME DEFASSIGN expr-compound {
                    $$ = PS.onDefProp(*$1, *$3);
                    free($1);
               } | def-prop-inline def-prop-accessor {
                    // ??
               }

//          func:
abstract-func: call-access type {
                str accessLife(*$1);
                $$ = PS.onAbstractFunc(accessLife->cast<getExpr>(), *$2);
           } | type '(' ')' type {
                $$ = PS.onAbstractFunc(*$1, *$4);
           }

def-func: abstract-func indentblock {
        $$ = PS.onFunc($1->cast<func>(), $2->cast<blockExpr>());
     }

end: END indentblock {
    // ??
 }

lambda: lambda-default {
        // ??
    } | lambda-deduction {
        // ??
    }
lambda-default: tuple type indentblock {
                // TODO: tuple should be checked whether it's defPropExpr or getExpr.
                // ??
            } | params type indentblock {
                // ??
            }
lambda-deduction: tuple indentblock {
                // TODO: tuple should be checked whether it's defPropExpr or getExpr.
                // ??
              } | params indentblock {
                // ??
              }


//          obj:
def-obj: def-obj-default { $$ = $1; }
       | def-obj-default-generic { $$ = $1; }
def-obj-default: DEF NAME indentDefBlock {
                $$ = PS.onDefObj(std::string(*$2), *$3);
                free($2);
             }
def-obj-default-generic: DEF NAME typeparams indentDefBlock {
                        tstr<args> argsLife($3);
                        $$ = PS.onDefObjGeneric(*$2, *argsLife, *$4);
                        free($2);
                     }

//              container:
def-array-value: '{' tuple-items '}' { $$ = PS.onDefArray(*$2); }

//              with:
with-inline: WITH expr-inline9 {
            // ??
         }

with-compound: with-inline indentblock {
                // ??
           } | with-inline ONLY indentDeclBlock {
                // ??
           }

//  predefined-type:
pack: PACK postfix NEWLINE { $$ = PS.onPack(*$2); }
    | %empty { $$ = PS.onPack(); }


%%

/*  ============================================================================================
    |                                         EPILOGUE                                         |
    ============================================================================================  */

static std::string traceErr(const yypcontext_t* ctx, yyscan_t scanner) {
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
static int yyreport_syntax_error(const yypcontext_t* ctx, yyscan_t scanner) {
    yysymbol_kind_t symbol = yypcontext_token(ctx);
    PS.onSrcArea(*yypcontext_location(ctx));
    PS.onParseErr(traceErr(ctx, scanner), yysymbol_name(symbol));

    return _onEndParse(scanner), 0;
}

void _onEndParse(yyscan_t scanner) {
    yyset_lineno(0, scanner);
}

std::string getTokenName(int tok) {
    if(tok <= 127) return std::string(1, tok);

    auto token = YYTRANSLATE(tok);
    return std::string(yysymbol_name(token));
}

// errors except syntax will come here. for instance, when available memory doesn't exist.
void yyerror(YYLTYPE* loc, yyscan_t scanner, const char* msg) {
    PS.error(errCode::MSG, msg);
}
