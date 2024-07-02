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
                NAMU_DI("trhsexpr(%x) <-- %s", $$, $1 ? "true" : "false");
            }
            | tver {
                $$ = new verSobj($1);
                NAMU_DI("tver(%x) <-- %s", $$, $1);
            }
            | tfloat {
                $$ = new termSobj($1);
                NAMU_DI("trhsexpr(%x) <-- %f", $$, $1);
            }
            | tint {
                $$ = new termSobj($1);
                NAMU_DI("trhsexpr(%x) <-- %d", $$, $1);
            }
            | tokStr {
                $$ = new termSobj($1);
                NAMU_DI("trhsexpr(%x) <-- '%s'", $$, $1);
            }
            | tokChar {
                $$ = new termSobj($1);
                NAMU_DI("trhsexpr(%x) <-- tokChar(%c)", $$, $1);
            }
            | tarray {
                $$ = $1;
                NAMU_DI("trhsexpr(%x) <-- tarray(%x)", $$, $1);
            }
            ;

tdefexpr    : tid topDefAssign trhsexpr {
                $3->setName($1);
                $$ = $3;
                NAMU_DI("tdefexpr(%x) <-- %s := trhsexpr(%x)", $$, $1, $3);
            }
            | tdefOrigin {
                $$ = $1;
                NAMU_DI("tdefexpr(%x) <-- tdefOrigin(%x)", $$, $1);
            }
            ;

trhsIds     : trhsexpr ',' trhsexpr {
                $$ = new sobj();
                $$->add(*$1);
                $1->add(*$3);
                NAMU_DI("trhsIds(%x) <-- trhsexpr(%x) , trhsexpr(%x)", $$, $1, $3);
            }
            | trhsIds ',' trhsexpr {
                $$ = $1;
                $$->add(*$3);
                NAMU_DI("rhsIds(%x) <-- trhsIds(%x) , trhsexpr(%x)", $$, $1, $3);
            }
            ;

tarray      : '{' trhsIds '}' {
                $$ = $2;
                NAMU_DI("tarray(%x) <-- { trhsIds(%x) }", $$, $2);
            }
            ;

tdefIndentBlock: teol tindent tdefBlock tdedent {
                $$ = $3;
                NAMU_DI("tdefIndentBlock(%x) <-- \\n \\t tdefBlock(%x) -\\t", $$, $3);
            }
            | ':' tdefexpr {
                $$ = $2;
                NAMU_DI("tdefIndentBlock(%x) <-- : tdefexpr(%x)", $$, $2);
            }
            ;

tdefOrigin  : tdef tid tdefIndentBlock {
                $3->setName($2);
                $$ = $3;
                NAMU_DI("tdefOrigin(%x) <-- def %s tdefIndentBlock(%x)", $$, $2, $3);
            }
            ;

tdefStmt    : tdefexpr teol {
                $$ = $1;
                NAMU_DI("tdefStmt(%x) <-- tdefexpr(%x) \\n", $$, $1);
            }
            | teol {
                $$ = nullptr;
                NAMU_DI("tdefStmt(null) <-- \\n");
            }
            ;

tdefBlock   : tdefStmt {
                $$ = new sobj();
                $$->add(*$1);
                NAMU_DI("tdefBlock(%x) <-- tdefStmt(%x)", $$, $1);
            }
            | tdefBlock tdefStmt {
                $$ = $1;
                if ($2 != nullptr)
                    $$->add(*$2);
                NAMU_DI("tdefBlock(%x) <-- tdefBlock(%x) tdefStmt(%x)", $$, $1, $2);
            }
            ;

tfile       : tdefBlock {
                $$ = root = $1;
                nm::id id = $1->getId();
                NAMU_DI("$1 = %x, %d.%d.%d", $1, id.tagN, id.chkN, id.serial);
                NAMU_DI("tfile(%x) <-- tdefBlock(%x)", $$, $1);
            }
            | teol {
                $$ = root = new sobj();
                NAMU_DI("tfile(%x) <-- \\n", $$);
            }
            | tfile teol {
                $$ = $1;
                NAMU_DI("tfile(%x) <-- \\n", $$);
            }
            | tfile teof {
                $$ = $1;
                NAMU_DI("tfile(%x) <-- EndOfFile", $$);
            }
            ;
