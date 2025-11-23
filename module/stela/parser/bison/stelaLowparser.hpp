/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Skeleton interface for Bison GLR parsers in C

   Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_ZZ_HOME_KNIZ_REPO_BYEOLANG_BYEOL_MODULE_STELA_PARSER_BISON_STELALOWPARSER_HPP_INCLUDED
# define YY_ZZ_HOME_KNIZ_REPO_BYEOLANG_BYEOL_MODULE_STELA_PARSER_BISON_STELALOWPARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef ZZDEBUG
# if defined YYDEBUG
#if YYDEBUG
#   define ZZDEBUG 1
#  else
#   define ZZDEBUG 0
#  endif
# else /* ! defined YYDEBUG */
#  define ZZDEBUG 1
# endif /* ! defined YYDEBUG */
#endif  /* ! defined ZZDEBUG */
#if ZZDEBUG
extern int zzdebug;
#endif
/* "%code requires" blocks.  */

    #include "stela/ast/point.hpp"

    typedef void* zzscan_t;

    namespace by {
        class stela;
    }

    struct lloc : public by::area {
        int colcnt;
        int strCol; // column position when it starts to parse string.

        void rel() {
            colcnt = 0;
            by::area::rel();
        }
    };


/* Token kinds.  */
#ifndef ZZTOKENTYPE
# define ZZTOKENTYPE
  enum zztokentype
  {
    ZZEMPTY = -2,
    ZZEOF = 0,                     /* "end of file"  */
    ZZerror = 256,                 /* error  */
    ZZUNDEF = 257,                 /* "invalid token"  */
    SCAN_AGAIN = 258,              /* SCAN_AGAIN  */
    SCAN_EXIT = 259,               /* SCAN_EXIT  */
    SCAN_MODE_NORMAL = 260,        /* SCAN_MODE_NORMAL  */
    SCAN_MODE_INDENT = 261,        /* SCAN_MODE_INDENT  */
    SCAN_MODE_INDENT_IGNORE = 262, /* SCAN_MODE_INDENT_IGNORE  */
    SCAN_MODE_END = 263,           /* SCAN_MODE_END  */
    DEF = 264,                     /* DEF  */
    _IN_ = 265,                    /* _IN_  */
    _INT_ = 266,                   /* _INT_  */
    _STR_ = 267,                   /* _STR_  */
    _BOOL_ = 268,                  /* _BOOL_  */
    FLT = 269,                     /* FLT  */
    NUL = 270,                     /* NUL  */
    _BYTE_ = 271,                  /* _BYTE_  */
    NEWLINE = 272,                 /* NEWLINE  */
    INDENT = 273,                  /* INDENT  */
    DEDENT = 274,                  /* DEDENT  */
    ENDOFFILE = 275,               /* ENDOFFILE  */
    DOUBLE_DOT = 276,              /* DOUBLE_DOT  */
    TAB = 277,                     /* TAB  */
    DEFASSIGN = 278,               /* DEFASSIGN  */
    OPEN_CLOSE_SQUARE_BRACKET = 279, /* OPEN_CLOSE_SQUARE_BRACKET  */
    INTVAL = 280,                  /* INTVAL  */
    FLTVAL = 281,                  /* FLTVAL  */
    BOOLVAL = 282,                 /* BOOLVAL  */
    NAME = 283,                    /* NAME  */
    STRVAL = 284,                  /* STRVAL  */
    VERVAL = 285                   /* VERVAL  */
  };
  typedef enum zztokentype zztoken_kind_t;
#endif

/* Value type.  */
#if ! defined ZZSTYPE && ! defined ZZSTYPE_IS_DECLARED
union ZZSTYPE
{

    int asInt;
    float asFlt;
    bool asBool;
    char asChar;
    std::string* asStr;
    by::stela* asStela;


};
typedef union ZZSTYPE ZZSTYPE;
# define ZZSTYPE_IS_TRIVIAL 1
# define ZZSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
typedef lloc ZZLTYPE;



int zzparse (zzscan_t scanner);
/* "%code provides" blocks.  */

    extern int zzlineno;
    extern char* yytext;
    namespace by {
        class stelaParser;
    }

    extern "C" {
        int zzlex(ZZSTYPE* val, ZZLTYPE* loc, zzscan_t scanner);
        void zzset_lineno(int linenumber, zzscan_t scanner);
        by::stelaParser* zzget_extra(zzscan_t scanner);
        char* zzget_text(zzscan_t scanner);
        void zzerror(ZZLTYPE* loc, zzscan_t scanner, const char* msg);
    }

    std::string getTokenName(int tok);


#endif /* !YY_ZZ_HOME_KNIZ_REPO_BYEOLANG_BYEOL_MODULE_STELA_PARSER_BISON_STELALOWPARSER_HPP_INCLUDED  */
