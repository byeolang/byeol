%{
#include "../common.hpp"
#include "../interp/sinterpreter.hpp"

namespace nm {
    class obj;
}

using namespace nm;

int yylex();
extern nm::sobj* root;
extern int yylineno;
extern char* yytext;

void yyerror(const char* s);

%}

%glr-parser
%union {
    int intVal;
    float floatVal;
    bool boolVal;
    char charVal;
    const char* strVal;
    nm::sobj* obj;
}

%verbose
%start tfile

%token tdef tprop tnode tin tindent tdedent

%token <intVal> teof tint
%token <floatVal> tfloat
%token <boolVal> tbool
%token <charVal> tokChar
%token <strVal> tokStr
%token <strVal> tid tcontainerName
%token <strVal> tver
%token teol
%type <obj> tfile tarray
%type <obj> trhsexpr trhsIds
%type <obj> tdefOrigin tdefIndentBlock tdefexpr tdefStmt tdefBlock

// 우선순위: 밑으로 갈수록 높음.
//  결합 순서 정의:
//      정해주지 않으면 모호성 오류 나온다.
//      left: 왼쪽 결합.
//      right: 오른쪽 결합.
%right '=' topDefAssign

%%

trhsexpr    : tbool {
                $$ = new termSobj($1);
                NM_DI("trhsexpr(%s) <-- %s", $$, $1 ? "true" : "false");
            }
            | tver {
                $$ = new verSobj($1);
                NM_DI("tver(%s) <-- %s", $$, $1);
            }
            | tfloat {
                $$ = new termSobj($1);
                NM_DI("trhsexpr(%s) <-- %f", $$, $1);
            }
            | tint {
                $$ = new termSobj($1);
                NM_DI("trhsexpr(%s) <-- %d", $$, $1);
            }
            | tokStr {
                $$ = new termSobj($1);
                NM_DI("trhsexpr(%s) <-- '%s'", $$, $1);
            }
            | tokChar {
                $$ = new termSobj($1);
                NM_DI("trhsexpr(%s) <-- tokChar(%c)", $$, $1);
            }
            | tarray {
                $$ = $1;
                NM_DI("trhsexpr(%s) <-- tarray(%s)", $$, $1);
            }
            ;

tdefexpr    : tid topDefAssign trhsexpr {
                $3->setName($1);
                $$ = $3;
                NM_DI("tdefexpr(%s) <-- %s := trhsexpr(%s)", $$, $1, $3);
            }
            | tdefOrigin {
                $$ = $1;
                NM_DI("tdefexpr(%s) <-- tdefOrigin(%s)", $$, $1);
            }
            ;

trhsIds     : trhsexpr ',' trhsexpr {
                $$ = new sobj();
                $$->add(*$1);
                $1->add(*$3);
                NM_DI("trhsIds(%s) <-- trhsexpr(%s) , trhsexpr(%s)", $$, $1, $3);
            }
            | trhsIds ',' trhsexpr {
                $$ = $1;
                $$->add(*$3);
                NM_DI("rhsIds(%s) <-- trhsIds(%s) , trhsexpr(%s)", $$, $1, $3);
            }
            ;

tarray      : '{' trhsIds '}' {
                $$ = $2;
                NM_DI("tarray(%s) <-- { trhsIds(%s) }", $$, $2);
            }
            ;

tdefIndentBlock: teol tindent tdefBlock tdedent {
                $$ = $3;
                NM_DI("tdefIndentBlock(%s) <-- \\n \\t tdefBlock(%s) -\\t", $$, $3);
            }
            | ':' tdefexpr {
                $$ = $2;
                NM_DI("tdefIndentBlock(%s) <-- : tdefexpr(%s)", $$, $2);
            }
            ;

tdefOrigin  : tdef tid tdefIndentBlock {
                $3->setName($2);
                $$ = $3;
                NM_DI("tdefOrigin(%s) <-- def %s tdefIndentBlock(%s)", $$, $2, $3);
            }
            ;

tdefStmt    : tdefexpr teol {
                $$ = $1;
                NM_DI("tdefStmt(%s) <-- tdefexpr(%s) \\n", $$, $1);
            }
            | teol {
                $$ = nullptr;
                NM_DI("tdefStmt(null) <-- \\n");
            }
            ;

tdefBlock   : tdefStmt {
                $$ = new sobj();
                $$->add(*$1);
                NM_DI("tdefBlock(%s) <-- tdefStmt(%s)", $$, $1);
            }
            | tdefBlock tdefStmt {
                $$ = $1;
                if ($2 != nullptr)
                    $$->add(*$2);
                NM_DI("tdefBlock(%s) <-- tdefBlock(%s) tdefStmt(%s)", $$, $1, $2);
            }
            ;

tfile       : tdefBlock {
                $$ = root = $1;
                nm::id id = $1->getId();
                NM_DI("$1 = %x, %d.%d.%d", $1, id.tagN, id.chkN, id.serial);
                NM_DI("tfile(%s) <-- tdefBlock(%s)", $$, $1);
            }
            | teol {
                $$ = root = new sobj();
                NM_DI("tfile(%s) <-- \\n", $$);
            }
            | tfile teol {
                $$ = $1;
                NM_DI("tfile(%s) <-- \\n", $$);
            }
            | tfile teof {
                $$ = $1;
                NM_DI("tfile(%s) <-- EndOfFile", $$);
            }
            ;
