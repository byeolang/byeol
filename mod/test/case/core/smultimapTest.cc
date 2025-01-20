#include "../../namuTest.hpp"

using namespace nm;
using namespace std;

struct smultimapTest : public namuTest {
    smultimap<std::string, str> scope;
};

TEST_F(smultimapTest, primitiveInsertion) {
    smultimap<std::string, int> m;
    ASSERT_EQ(m.size(), 0);

    // basic insertion:
    m.insert("apple", 1);
    m.insert("banana", 2);
    ASSERT_EQ(m.size(), 2);

    ASSERT_EQ(m.begin("apple").getVal(), 1);
    ASSERT_EQ(*m.begin("banana"), 2);
}

TEST_F(smultimapTest, nodeInsertion) {
    ASSERT_EQ(scope.size(), 0);

    scope.insert("apple", *new nInt(1));
    scope.insert("banana", *new nInt(2));
    ASSERT_EQ(scope.size(), 2);

    ASSERT_EQ(scope.begin("apple")->get().cast<nint>(), 1);
    ASSERT_EQ(scope.begin("banana")->get().cast<nint>(), 2);
}

TEST_F(smultimapTest, multiInsertion) {
    ASSERT_EQ(scope.size(), 0);

    scope.insert("apple", *new nInt(500));
    scope.insert("banana", *new nInt(2000));
    scope.insert("pizza", *new nInt(25000));
    scope.insert("fried chicken", *new nInt(20000));
    scope.insert("banana", *new nInt(2100));
    ASSERT_EQ(scope.size(), 5);

    ASSERT_EQ(scope.begin("apple")->get().cast<nint>(), 500);
    ASSERT_EQ(scope.begin("fried chicken")->get().cast<nint>(), 20000);

    auto e = scope.begin("banana");
    nint val1 = e->get().cast<nint>();
    ASSERT_TRUE(val1 == 2000 || val1 == 2100);

    ++e;
    nint val2 = e->get().cast<nint>();
    nint expectVal2 = val1 == 2000 ? 2100 : 2000;
    ASSERT_EQ(val2, expectVal2);

    ASSERT_EQ(scope.begin("fried chiken"), scope.end()); // look. there is a typo.
}

TEST_F(smultimapTest, insertionOrder) {
    ASSERT_EQ(scope.size(), 0);

    for(nint n = 0; n < 10; ++n) {
        scope.insert("apple", *new nInt(n));
        if(n == 5)
            scope.insert("banana", *new nInt(-1));
    }
    ASSERT_EQ(scope.size(), 10 + 1); // 1 for banana

    auto e = scope.begin("apple");
    ASSERT_EQ(e->get().cast<nint>(), 0);
    for(nint n = 0; n < 10; n++) {
        ASSERT_EQ(e->get().cast<nint>(), n);
        ++e;
    }
    ASSERT_EQ(e, scope.end());

    e = scope.begin("banana");
    ASSERT_EQ(e.getVal().get().cast<nint>(), -1);
}

TEST_F(smultimapTest, complexEraseTest) {
    for(nint n = 0; n < 10; ++n) {
        scope.insert("apple", *new nInt(n));
        if(n == 5)
            scope.insert("banana", *new nInt(-1));
    }
    ASSERT_EQ(scope.size(), 10 + 1); // 1 for banana

    auto e1 = scope.begin("apple");
    scope.erase(e1); // erase 0;
    ++e1; // 1
    ++e1; // 2
    ++e1; // 3
    scope.erase(e1); // erase '3'

    auto e = scope.begin("apple");
    int expects[] = {1, 2, 4, 5, 6, 7, 8, 9};
    int expectLen = 8;
    for(nint n = 0; n < expectLen; n++) {
        ASSERT_EQ(e->get().cast<nint>(), expects[n]);
        ++e;
    }
    ASSERT_EQ(e, scope.end());
    ASSERT_EQ(scope.size(), 9);

    scope.erase("apple"); // erase all apples.
    ASSERT_EQ(scope.begin("apple"), scope.end());
    ASSERT_EQ(scope.size(), 1);

    e = scope.begin("banana");
    ASSERT_EQ(e.getVal().get().cast<nint>(), -1);
}
