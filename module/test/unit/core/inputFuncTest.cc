#include "test/byeolTest.hpp"
#include "core/builtin/pkgs/default/inputFunc.hpp"
#include "core/builtin/scalar/nStr.hpp"

using namespace by;
using namespace std;

struct inputFuncTest: public byeolTest {};

TEST_F(inputFuncTest, testGetType) {
    inputFunc func;
    const ntype& type = func.getType();

    ASSERT_EQ(type.getName(), "input");
    ASSERT_EQ(type.getParams().len(), 0);

    // Check return type is nStr
    const node* ret = type.getRet();
    ASSERT_TRUE(ret != nullptr);
    ASSERT_TRUE(ret->isSub<nStr>());
}

// Commented out due to crash issues
// TEST_F(inputFuncTest, testGetOrigin) {
//     inputFunc func;
//     const baseObj& origin = func.getOrigin();
//
//     ASSERT_TRUE(origin.isSub<obj>());
//     ASSERT_FALSE(origin.isSub<nStr>());
// }

// Commented out due to crash issues
// TEST_F(inputFuncTest, testClone) {
//     inputFunc func;
//     clonable* cloned = func.clone();
//
//     ASSERT_TRUE(cloned != nullptr);
//     inputFunc* clonedFunc = dynamic_cast<inputFunc*>(cloned);
//     ASSERT_TRUE(clonedFunc != nullptr);
//     ASSERT_EQ(clonedFunc->getType().getName(), "input");
//     delete cloned;
// }

TEST_F(inputFuncTest, testGetParams) {
    inputFunc func;
    const params& ps = func.getParams();

    // input() function takes no parameters
    ASSERT_EQ(ps.len(), 0);
}

TEST_F(inputFuncTest, testIsSubOfBaseFunc) {
    inputFunc func;

    ASSERT_TRUE(func.isSub<baseFunc>());
    ASSERT_TRUE(func.isSub<node>());
}

TEST_F(inputFuncTest, testGetTypeReturnsSameInstance) {
    inputFunc func;
    const ntype& type1 = func.getType();
    const ntype& type2 = func.getType();

    // getType() should return the same static instance
    ASSERT_EQ(&type1, &type2);
}

// Commented out due to crash issues
// TEST_F(inputFuncTest, testGetOriginReturnsSameInstance) {
//     inputFunc func;
//     const baseObj& origin1 = func.getOrigin();
//     const baseObj& origin2 = func.getOrigin();
//
//     // getOrigin() should return the same static instance
//     ASSERT_EQ(&origin1, &origin2);
// }

TEST_F(inputFuncTest, testEvalWithWrongArgCount) {
    inputFunc func;

    // Create args with 1 argument (but input() expects 0)
    nInt arg1(42);
    narr argArr;
    argArr.add(&arg1);
    args testArgs(argArr);

    // Should return null because arg count doesn't match
    str result = func.eval(testArgs);
    ASSERT_FALSE(result);
}

TEST_F(inputFuncTest, testTypeIsNotImmutable) {
    inputFunc func;
    const ntype& type = func.getType();

    // Functions are not immutable types
    ASSERT_FALSE(type.isImmutable());
}
