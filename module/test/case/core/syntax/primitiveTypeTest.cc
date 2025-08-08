#include "test/byeolSyntaxTest.hpp"

using namespace nm;
using namespace std;

namespace {
    struct primitiveTypeTest: public byeolSyntaxTest {};
}

TEST_F(primitiveTypeTest, strFuncLen) {
    make()
        .parse(R"SRC(
        main() int
            word := "hello"
            ret word.len()
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), nStr("hello").len());
}

TEST_F(primitiveTypeTest, strFuncGet) {
    make()
        .parse(R"SRC(
        main() int
            "hello"[2] == "l"
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 1);
}

TEST_F(primitiveTypeTest, concatCharAndStr) {
    make()
        .parse(R"SRC(
        main() int
            ("hello" + "1" + "wow" + "2") == "hello1wow2"
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 1);
}

TEST_F(primitiveTypeTest, charConversion) {
    make()
        .parse(R"SRC(
        main() int
            a := "h"
            a as byte == 104
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 1);
}

TEST_F(primitiveTypeTest, byteConversion) {
    make()
        .parse(R"SRC(
        main() int
            b byte := 104
            b as str == "h"
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 1);
}

TEST_F(primitiveTypeTest, strGetSeq) {
    make()
        .parse(R"SRC(
        main() int
            "hello world"[2..4] == "ll"
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 1);
}

TEST_F(primitiveTypeTest, strIter) {
    make()
        .parse(R"SRC(
        main() int
            ans := ""
            for c in "hello"
                ans += c
            ret ans == "hello"
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 1);
}

TEST_F(primitiveTypeTest, deduceAndImplicitCastNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            a := "a" + 1 # str + int is not allowed.
            print(a)
    )SRC")
        .shouldVerified(false);
}

TEST_F(primitiveTypeTest, codepointBasedLen) {
    make()
        .parse(R"SRC(
        main() int
            if "அம்மா".len() != 5: 0
            else: 1
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 1);
}

TEST_F(primitiveTypeTest, codepointBasedLen2) {
    make()
        .parse(R"SRC(
        main() int
            if "🏁🎌☃".len() != 3: 0
            else: 1
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 1);
}

TEST_F(primitiveTypeTest, codepointBasedIteration) {
    make()
        .parse(R"SRC(
        main() int
            res str
            for ch in "அம்மா"
                res += ch
            res == "அம்மா"
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 1);
}

TEST_F(primitiveTypeTest, codepointBasedIteration2) {
    make()
        .parse(R"SRC(
        main() int
            res str
            n int
            for ch in "안1녕2하3세4요."
                if n++ % 2 == 1: next
                res += ch
            res == "안녕하세요"
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res.cast<nint>(), 1);
}
