#include "test/byeolTest.hpp"

using namespace by;
using namespace std;

struct funcTypeTest: public byeolTest {};

TEST_F(funcTypeTest, testDefaultConstructor) {
    // Create empty params for a simple function
    params ps;
    nInt retType(0);

    funcType ft("testFunc", ttype<baseFunc>::get(), ps, false, &retType);

    ASSERT_EQ(ft.getName(), "testFunc");
    ASSERT_EQ(ft.getParams().len(), 0);
}

TEST_F(funcTypeTest, testConstructorWithParams) {
    // Create a function type with parameters: func(int, str) -> int
    nInt intOrigin(0);
    nStr strOrigin("");
    nInt retType(0);

    params ps;
    ps.add(new param("arg1", intOrigin));
    ps.add(new param("arg2", strOrigin));

    funcType ft("testFunc", ttype<baseFunc>::get(), ps, false, &retType);

    ASSERT_EQ(ft.getName(), "testFunc");
    ASSERT_EQ(ft.getParams().len(), 2);
}

TEST_F(funcTypeTest, testEqualityOperatorWithSameSignature) {
    // Two funcTypes with the same signature should be equal
    nInt intOrigin(0);
    nInt retType(0);

    params ps1;
    ps1.add(new param("arg", intOrigin));

    params ps2;
    ps2.add(new param("arg", intOrigin));

    funcType ft1("func1", ttype<baseFunc>::get(), ps1, false, &retType);
    funcType ft2("func2", ttype<baseFunc>::get(), ps2, false, &retType);

    // operator== only compares parameter and return type, not the name
    ASSERT_TRUE(ft1 == ft2);
}

TEST_F(funcTypeTest, testEqualityOperatorWithDifferentParams) {
    // Two funcTypes with different parameters should not be equal
    nInt intOrigin(0);
    nStr strOrigin("");
    nInt retType(0);

    params ps1;
    ps1.add(new param("arg", intOrigin));

    params ps2;
    ps2.add(new param("arg", strOrigin));

    funcType ft1("func1", ttype<baseFunc>::get(), ps1, false, &retType);
    funcType ft2("func2", ttype<baseFunc>::get(), ps2, false, &retType);

    ASSERT_FALSE(ft1 == ft2);
}

TEST_F(funcTypeTest, testEqualityOperatorWithDifferentReturnTypes) {
    // Two funcTypes with different return types should not be equal
    nInt intOrigin(0);
    nInt intRet(0);
    nStr strRet("");

    params ps1;
    ps1.add(new param("arg", intOrigin));

    params ps2;
    ps2.add(new param("arg", intOrigin));

    funcType ft1("func", ttype<baseFunc>::get(), ps1, false, &intRet);
    funcType ft2("func", ttype<baseFunc>::get(), ps2, false, &strRet);

    ASSERT_FALSE(ft1 == ft2);
}

TEST_F(funcTypeTest, testEqualityOperatorWithSameReference) {
    // A funcType should be equal to itself
    nInt intOrigin(0);
    nInt retType(0);

    params ps;
    ps.add(new param("arg", intOrigin));

    funcType ft("func", ttype<baseFunc>::get(), ps, false, &retType);

    ASSERT_TRUE(ft == ft);
}

TEST_F(funcTypeTest, testConstructorWithEmptyParams) {
    // Test function with no parameters: func() -> int
    params ps;
    nInt retType(0);

    funcType ft("noArgFunc", ttype<baseFunc>::get(), ps, false, &retType);

    ASSERT_EQ(ft.getName(), "noArgFunc");
    ASSERT_EQ(ft.getParams().len(), 0);
}

TEST_F(funcTypeTest, testConstructorWithMultipleParams) {
    // Test function with multiple parameters: func(int, str, flt) -> void
    nInt intOrigin(0);
    nStr strOrigin("");
    nFlt fltOrigin(0.0f);
    nVoid retType;

    params ps;
    ps.add(new param("arg1", intOrigin));
    ps.add(new param("arg2", strOrigin));
    ps.add(new param("arg3", fltOrigin));

    funcType ft("multiArgFunc", ttype<baseFunc>::get(), ps, false, &retType);

    ASSERT_EQ(ft.getName(), "multiArgFunc");
    ASSERT_EQ(ft.getParams().len(), 3);
}

TEST_F(funcTypeTest, testGetParamsAndRet) {
    // Test getParams() and getRet() methods
    nInt intOrigin(0);
    nInt retType(42);

    params ps;
    ps.add(new param("arg", intOrigin));

    funcType ft("func", ttype<baseFunc>::get(), ps, false, &retType);

    ASSERT_EQ(ft.getParams().len(), 1);
    ASSERT_EQ(ft.getParams()[0].getName(), "arg");
    ASSERT_TRUE(ft.getRet() != nullptr);
}
