#pragma once

//        helpers:
#define BY_VOID()
#define BY_SPACE BY_VOID() BY_VOID()
#define BY_CONSUME_ARGS(...)
#define BY_UNWRAP(...) __VA_ARGS__

#define BY_UNCONST_0() BY_UNCONST_1(unconst)
#define BY_UNCONST_1(value) BY_UNCONST_2(me, value)
#define BY_UNCONST_2(type, value) BY_UNWRAP(type)& value = *(const_cast<BY_UNWRAP(type)*>(this));
#define BY_UNCONST(...) BY_OVERLOAD(BY_UNCONST, __VA_ARGS__)

#define BY_DELAY(x) x BY_VOID()

#define _STR(x) #x
#define BY_STR(x) _STR(x)

#define _CONCAT(x, y) x##y
#define BY_CONCAT(x, y) _CONCAT(x, y)

//    why delaying comma expression?:
//        it can causes delimiter issue on expanding macros for template parameters.
#define _COMMA() ,
#define BY_COMMA() BY_DELAY(_COMMA)()
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
