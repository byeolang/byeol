// common:
PAIRX(UNKNOWN, "unknown")
PAIRX(MSG, "%s")
PAIRX(IS_NUL, "%s is null")
PAIRX(FILE_NOT_OPEN, "can't open %s file.")

// location:
PAIRX(NO_ROW_INFO, "this expr doesn't have row info")
PAIRX(NO_COL_INFO, "this expr doesn't have col info")

// node:
PAIRX(CANT_ACCESS, "there is no '%s' in '%s'")
PAIRX(AMBIGIOUS_ACCESS, "ambigious access found on '%s'")
PAIRX(DUP_VAR, "duplicated variable '%s' found")
PAIRX(ACCESS_TO_INCOMPLETE, "accesses to incomplete object")

// obj:
PAIRX(CANT_CAST_TO_NODE, "obj can't be cast into node class")
PAIRX(OBJ_WRONG_ARGS, "%s can't run with this argument types '%s'. they should be '%s'")
PAIRX(OBJ_ALREADY_EXIST, "\'%s\' object already exists at %s")

// func:
PAIRX(FUNC_DONT_HAVE_ME, "parameters of func not included 'me'")
PAIRX(FUNC_REDIRECTED_OBJ, "all funcs should be redirected by object")
PAIRX(MAIN_FUNC_RET_TYPE, "\'main\' func return type should be int or void")
PAIRX(ALREADY_DEFINED_FUNC,
    "same name and params of the func '%s' was already defined in this scope")
PAIRX(ALREADY_DEFINED_IDENTIFIER, "same identifier of func '%s' was already defined in this scope")
PAIRX(RETURN_VALUE_IS_NUL, "return value of %s() was null.")

// ctor:
PAIRX(CTOR_NOT_IN_DEF_OBJ, "ctor should be inside of 'def' block.")

// parser:
PAIRX(SYNTAX_ERR, "syntax error %s expected, but %s found")
PAIRX(UNEXPECTED_TOK, "unexpected token '%s' found")
PAIRX(WRONG_INDENT_LV,
    "invalid indentation level found. expected %d columns but it was %d. I'm assuming you were "
    "trying to give %d columns")
PAIRX(PARSING_HAS_ERR, "parsing has done. but something went wrong: %d")
PAIRX(NO_SRC, "source code not provided")
PAIRX(SRC_EMPTY, "source code was empty string.")
PAIRX(NO_FUNC_REGISTERED, "there is no func info registering")

// thread:
PAIRX(THERE_IS_NO_FRAMES_IN_THREAD, "there is no frames in current thread.")
PAIRX(FRAME_DOESNT_HAVE_OBJ_SCOPE, "current frame doesn't have any obj scope.")

// pack:
PAIRX(PACK_ONLY_ALLOW_VAR_ACCESS, "variable path only allowed when you define a pack")
PAIRX(NO_PACK_TRAY, "pack tray hasn't been created before")
PAIRX(NO_PACK, "no pack has been specified")
PAIRX(PACK_NOT_MATCH, "pack %s expected, but it was %s")

// expr:
PAIRX(NOT_EXPR, "\'%s\' isn't a expression")
PAIRX(EXPR_EVAL_NUL, "expression evaluated as null type")
//  defVar:
PAIRX(CANT_DEF_VAR, "definining variable '%s %s' wouldn't allowed here")
PAIRX(ALREADY_DEFINED_VAR, "variable '%s %s' was already defined in this scope")
PAIRX(HAS_NO_NAME, "variable should have a name")
PAIRX(NO_ORIGIN, "there is no origin %s object")
PAIRX(TYPE_NOT_EXIST, "when defines variable '%s', couldn't find the type of it.")
PAIRX(DONT_HAVE_CTOR, "%s's type doesn't have a constructor without parameter.")
PAIRX(TYPE_NOT_DEDUCED, "type can't be deduced into meaningful one.")
PAIRX(NO_VOID_CONTAINER, "you can't make a void container")
PAIRX(TYPE_IS_NOT_PRE_EVALUATED,
    "there is something wrong in pre-evaluation to define %s variable.")
PAIRX(PROTECTED_NOT_ALLOWED_FOR_LOCAL,
    "you can't declare 'protected' modifier for a local variable, %s")
PAIRX(OVERRIDE_NOT_ALLOWED_FOR_LOCAL,
    "you can't decalre 'explicit-override' for a local varaible, %s")
PAIRX(MODIFIER_NOT_FOUND, "parser didn't make a modifier for this symbol, %s")
PAIRX(CANT_ACCESS_TO_PROTECTED_VARIABLE, "you can't access this %s variable. it's protected.")
PAIRX(EXPLICIT_TYPE_SHOULDNT_BE_NULL, "explicit type shouldn't be null when you define it.")
PAIRX(NO_VOID_VARIABLE, "you can't define a varaible as void type")
PAIRX(SHORT_DEF_VAR_ONLY_ALLOWED_TO_CUSTOM_TYPE,
    "short definition isn't allowed to array or primitive types")
PAIRX(COULDNT_MAKE_INSTANCE_UNEXPECTEDLY, "couldn't make an instance unexpectedly")
PAIRX(YOU_CANT_DEFINE_PROPERTY_WITH_ABSTRACT_FUNC, "you can't define property with abstract func")
//  defSeq:
PAIRX(SEQ_SHOULD_INT_COMPATIBLE, "start and end of a sequence should be sort of an int.")
//  assign:
PAIRX(TYPE_NOT_COMPATIBLE, "rhs type '%s' is not compatible to lhs '%s' of this assigment")
PAIRX(ASSIGN_TO_RVALUE, "you can't assign %s to this unamed %s. please define it first")
PAIRX(ASSIGN_TO_CONST, "to assign value to '%s' const object is not allowed")
//  type:
PAIRX(CAST_NOT_AVAILABLE, "couldn't cast %s type as %s")
PAIRX(LHS_IS_NUL, "lhs of the expression is null")
PAIRX(RHS_IS_NUL, "rhs of the expression is null")
PAIRX(EXPR_SHOULDNT_BE_HERE, "expression shouldn't be here")
PAIRX(IMPLICIT_CAST_NOT_AVAILABLE, "couldn't implicit cast %s type as %s")
PAIRX(VOID_NOT_CAST, "casting to the void type isn't allowed")
PAIRX(CAST_TO_UNKNOWN, "cast to unknown type")
PAIRX(ARR_DOESNT_HAVE_TYPE_PARAM, "array doesn't have type parameter.")
PAIRX(ARR_RETURNING_SOMETHING, "block returning something can't be evaluated as an array.")
PAIRX(CONVERGE_INFO_IS_NULL, "couldn't converge types. some type info was null")
PAIRX(CONVERGE_INFO_NOT_ACCESSIBLE, "couldn't converge type of %s")
//  arithmetic operation:
PAIRX(LHS_IS_NOT_ARITH, "lhs '%s' is not arithmetic type")
PAIRX(RHS_IS_NOT_ARITH, "rhs '%s' is not arithmetic type")
//  defObj:
PAIRX(MAKE_GENERIC_FAIL, "construction '%s' for generic obj has failed.")
PAIRX(ORIGIN_OBJ_CANT_BE_CONST, "origin object can't be defined with 'const'")
PAIRX(COMPLETE_OBJ_BUT_NO_CTOR, "complete object should have a ctor without any parameters")
PAIRX(CANT_CALL_COMPLETE_FOR_INCOMPLETE, "incomplete object couldn't have any call complete.")
PAIRX(UNEXPECTED_ATTR, "unexpected attribute found")
//  generics:
PAIRX(SHOULD_TYPE_PARAM_NAME, "'%s' type parameter wasn't be defined as new name.")
//  func-call:
PAIRX(FUNC_NOT_EXIST, "this func doesn't exist.")
PAIRX(DONT_KNOW_ME, "I don't know what the 'me' object is to call this func.")
//  retState:
PAIRX(BREAK_OUTSIDE_OF_LOOP, "use 'break' outside of a loop")
PAIRX(NEXT_OUTSIDE_OF_LOOP, "use 'next' outside of a loop")
PAIRX(LOOP_NO_RET_ARR, "loop expression didn't return an array")
//  if:
PAIRX(CONDITION_IS_EMPTY, "condition expression is empty")
PAIRX(CONDITION_CANT_CAST_TO_BOOL, "condition expression can't cast to bool type.")
//  getExpr:
PAIRX(WHAT_IS_THIS_IDENTIFIER, "I don't know what '%s' is.")
PAIRX(IDENTIFIER_ONLY, "You can't use type or generic for '%s'")
//  defAssign:
PAIRX(RHS_NOT_EVALUATED,
    "I can't define a new variable here because there is a problem with the expression on the "
    "right.")
PAIRX(VOID_CANT_DEFINED, "you can't define a variable to void type")
PAIRX(CANT_ASSIGN_RET, "rhs has 'ret' which is not appropriate to assign.")

// end:
PAIRX(END_ONLY_BE_IN_A_FUNC, "you've to use 'end' inside of a func.")

// func:("verify: arithmeticExpr: lhs & rhs should bind something.");
//
// if(nul(it.getLeft) || nul(it.getRight()))
//
PAIRX(PARAM_HAS_VAL, "parameter must take a form of '<name> <type>'")
PAIRX(FUNC_HAS_NO_NAME, "a function should have a name")
PAIRX(WRONG_RET_TYPE, "%s is not a valid return type")
PAIRX(NO_RET_TYPE, "return type is not specified")
PAIRX(NO_FUNC_INFO, "function info doesn't exist")
PAIRX(MAIN_SHOULD_HAVE_STMTS, "'main' function need to contain more than 1 statement inside")
PAIRX(RET_TYPE_NOT_MATCH, "return type is %s. and it's not valid to return type of func, %s")
PAIRX(PARAM_NOT_VOID, "parameter '%s' shouldn't be a void type")
PAIRX(FUNC_SHOULD_RETURN_SOMETHING, "function should return something")
PAIRX(RET_ERR_ON_CTOR, "returning err on function not allowed")

// closure:
PAIRX(NOT_CAPTURED, "closure don't capture any scopes.")
PAIRX(NOT_HAS_FUNC, "closure don't refer a func")
PAIRX(CANT_RETURN_A_CLOSURE,
    "it seems that you tried to return a closure of a func, but I couldn't")

// block:
PAIRX(RET_AT_MIDDLE_OF_BLOCK, "use 'ret' at last of block stmt.")
PAIRX(BLK_CANT_MAKE_FRAME, "block statement failed to construct frame structure")

// container:
PAIRX(ELEM_TYPE_IS_NUL, "there is no elem type")
PAIRX(ELEM_TYPE_DEDUCED_NUL, "elem type has been deduced to null")
PAIRX(ELEM_TYPE_DEDUCED_WRONG, "elem type has been deduced to %s type")
PAIRX(ELEM_TYPE_NOT_VOID, "elem type shouldn't be the void type")
PAIRX(CONTAINER_IS_NUL, "there is no container")
PAIRX(ITERATOR_IS_NUL, "null iterator has been used")
PAIRX(VALUE_IS_NUL, "tried to add/delete null value")
PAIRX(ITERATOR_NOT_BELONG_TO_CONTAINER, "this iterator is not belonged to this container.")
PAIRX(OUT_OF_RANGE, "given index '%d' is out of range. container's length is %d")

// operator:
PAIRX(STRING_IS_NOT_PROPER_TO_OP, "string type isn't suitable to this operators.")
PAIRX(PARANTHESIS_WAS_TUPLE, "paranthesis was tuple only contains 1 expression")

// signal:
PAIRX(SIG_SEGV, "segmentation fault")
PAIRX(SIG_INT, "external interrupt")
PAIRX(SIG_ILL, "invalid program image")
PAIRX(SIG_ABORT, "abonormal termination condition")
PAIRX(SIG_FPE, "erroneous arithmetic operation")
PAIRX(SIG_TERM, "termination request")
