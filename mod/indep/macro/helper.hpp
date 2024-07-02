#pragma once

//        helpers:
#define NM_VOID()
#define NM_SPACE NM_VOID() NM_VOID()
#define NM_CONSUME_ARGS(...)
#define NM_UNWRAP(...) __VA_ARGS__

#define NM_UNCONST_0()             NM_UNCONST_1(unconst)
#define NM_UNCONST_1(value)        NM_UNCONST_2(me, value)
#define NM_UNCONST_2(type, value)  NM_UNWRAP(type)& value = *(const_cast<NM_UNWRAP(type)*>(this));
#define NM_UNCONST(...)            NM_OVERLOAD(NM_UNCONST, __VA_ARGS__)

#define NM_DELAY(x) x NM_VOID()

#define _STR(x)            #x
#define NM_STR(x)    _STR(x)

#define _CONCAT(x, y)    x##y
#define NM_CONCAT(x, y)    _CONCAT(x, y)

//    why delaying comma expression?:
//        it can causes delimiter issue on expanding macros for template parameters.
#define _COMMA() ,
#define NM_COMMA() NM_DELAY(_COMMA)()
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
