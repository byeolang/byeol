#include "test/byeolIntegTest.hpp"

using namespace by;
using namespace std;

struct seqIntegTest: public byeolIntegTest {};

TEST_F(seqIntegTest, basicBehaviour) {
    nseq s(1, 10); // [1, 10)
    ASSERT_EQ(*s[0].cast<nint>(), 1);
    ASSERT_EQ(*s[8].cast<nint>(), 9);

    ASSERT_EQ(s.len(), 9);
}

TEST_F(seqIntegTest, basicBehaviourNegative) {
    nseq s(1, 10);
    errReport rpt(false);
    threadUse thr(rpt);
    ASSERT_EQ(s[10], s[8]); // returns value but this is negative test.
    ASSERT_EQ(s[-1], s[0]);
    ASSERT_TRUE(rpt);
}

TEST_F(seqIntegTest, basicBehaviourOfSeq) {
    seq s(1, 10); // [1, 10)
    ASSERT_EQ(*s[0].cast<nint>(), 1);
    ASSERT_EQ(*s[8].cast<nint>(), 9);

    ASSERT_EQ(s.len(), 9);
}

TEST_F(seqIntegTest, basicBehaviourOfSeqNegative) {
    seq s(1, 10);
    errReport rpt(false);
    threadUse thr(rpt);
    ASSERT_EQ(s[10], s[8]); // returns value but this is negative test.
    ASSERT_EQ(s[-1], s[0]);
    ASSERT_TRUE(rpt);
}

TEST_F(seqIntegTest, basicManagedBehaviorOfSeq) {
    errReport rpt(false); // silent report
    threadUse thr(rpt);

    seq s(1, 10);
    str ret = s.eval("len");
    ASSERT_TRUE(ret);
    ASSERT_EQ(*ret->cast<nint>(), s.len());

    ret = s.eval("get", args(narr(*new nInt(0))));
    ASSERT_TRUE(ret);
    ASSERT_EQ(*ret->cast<nInt>(), s[0]);

    ASSERT_FALSE(rpt);
    ret = s.eval("get", args(narr(*new nInt(-1))));
    ASSERT_TRUE(ret);
    ASSERT_EQ(*ret->cast<nInt>(), s[-1]);
    ASSERT_TRUE(rpt);
    rpt.rel();

    ret = s.eval("get", args(narr(*new nInt(8))));
    ASSERT_TRUE(ret);
    ASSERT_EQ(*ret->cast<nInt>(), s[8]);

    ASSERT_FALSE(rpt);
    ret = s.eval("get", args(narr(*new nInt(10))));
    ASSERT_TRUE(ret);
    ASSERT_EQ(*ret->cast<nInt>(), s[10]);
    ASSERT_TRUE(rpt);
}

TEST_F(seqIntegTest, eachReturnedValueShouldBeDifferent) {
    seq s(1, 10); // [1, 10)

    str ret = s.eval("get", args(narr(*new nInt(0))));
    ASSERT_TRUE(ret);
    ASSERT_EQ(*ret->cast<nInt>(), s[0]);
    str ret2 = s.eval("get", args(narr(*new nInt(1))));
    ASSERT_TRUE(ret2);
    ASSERT_EQ(*ret2->cast<nInt>(), s[1]);

    ASSERT_EQ(*ret->cast<nInt>(), s[0]);
}

TEST_F(seqIntegTest, basicSyntaxNegative) {
    make()
        .negative() // the code below doesn't have compile error, but runtime one.
        .parse(R"SRC(
        print(msg str) void: 1
        main() int
            seq := 2..4 # [2, 4)
            print("seq.len=" + seq.len() as str + "\n")
            print("seq[1]=" + seq[1] as str + "\n")
            ret seq[2] # this may occurs exception!
    )SRC")
        .shouldVerified(true);

    str res = run();
    ASSERT_TRUE(res);
    ASSERT_TRUE(res->isSub<baseErr>());
}

TEST_F(seqIntegTest, addSeqElement) {
    make()
        .parse(R"SRC(
    main() int
        seq := 2..4
        ret seq[0] + seq[1]
    )SRC")
        .shouldVerified(true);
    str res = run();
    ASSERT_TRUE(res);
    ASSERT_EQ(*res->cast<nint>(), 5);
}

TEST_F(seqIntegTest, verifyStartEndNegative) {
    make()
        .negative()
        .parse(R"SRC(
        main() void
            out := "kniz awesome"
            for n in 0..out
                print(out[n])
    )SRC")
        .shouldVerified(false);
}
