#include "test/namuTest.hpp"

using namespace nm;
using namespace std;

struct smultimapTest: public namuTest {
    smultimap<std::string, str> scope;
};

TEST_F(smultimapTest, primitiveInsertion) {
    smultimap<std::string, int> m;
    ASSERT_EQ(m.size(), 0);

    // basic insertion:
    m.insert("apple", 1);
    m.insert("banana", 2);
    ASSERT_EQ(m.size(), 2);

    ASSERT_EQ(*m.begin("apple").getVal(), 1);
    ASSERT_EQ(*m.begin("banana"), 2);
}

TEST_F(smultimapTest, nodeInsertion) {
    ASSERT_EQ(scope.size(), 0);

    scope.insert("apple", *new nInt(1));
    scope.insert("banana", *new nInt(2));
    ASSERT_EQ(scope.size(), 2);

    ASSERT_EQ(*scope.begin("apple")->get()->cast<nint>(), 1);
    ASSERT_EQ(*scope.begin("banana")->get()->cast<nint>(), 2);
}

TEST_F(smultimapTest, multiInsertion) {
    ASSERT_EQ(scope.size(), 0);

    scope.insert("apple", *new nInt(500));
    scope.insert("banana", *new nInt(2000));
    scope.insert("pizza", *new nInt(25000));
    scope.insert("fried chicken", *new nInt(20000));
    scope.insert("banana", *new nInt(2100));
    ASSERT_EQ(scope.size(), 5);

    ASSERT_EQ(*scope.begin("apple")->get()->cast<nint>(), 500);
    ASSERT_EQ(*scope.begin("fried chicken")->get()->cast<nint>(), 20000);

    auto e = scope.begin("banana");
    nint val1 = *e->get()->cast<nint>();
    ASSERT_TRUE(val1 == 2000 || val1 == 2100);

    ++e;
    nint val2 = *e->get()->cast<nint>();
    nint expectVal2 = val1 == 2000 ? 2100 : 2000;
    ASSERT_EQ(val2, expectVal2);

    ASSERT_EQ(scope.begin("fried chiken"), scope.end()); // look. there is a typo.
}

TEST_F(smultimapTest, insertionOrder) {
    ASSERT_EQ(scope.size(), 0);

    for(nint n = 0; n < 10; ++n) {
        scope.insert("apple", *new nInt(n));
        if(n == 5) scope.insert("banana", *new nInt(-1));
    }
    ASSERT_EQ(scope.size(), 10 + 1); // 1 for banana

    auto e = scope.begin("apple");
    ASSERT_EQ(*e->get()->cast<nint>(), 0);
    for(nint n = 0; n < 10; n++) {
        ASSERT_EQ(*e->get()->cast<nint>(), n);
        ++e;
    }
    ASSERT_EQ(e, scope.end());

    e = scope.begin("banana");
    ASSERT_EQ(*e.getVal()->get()->cast<nint>(), -1);
}

TEST_F(smultimapTest, eraseMultiPairs) {
    for(nint n = 0; n < 7; ++n) {
        scope.insert("apple", *new nInt(n));
        if(n == 3) scope.insert("banana", *new nInt(-1));
    }
    ASSERT_EQ(scope.size(), 8); // = {0, 1, 2, 3, -1, 4, 5, 6}

    auto e = scope.begin();                  // 0
    ASSERT_EQ(*(e++)->get()->cast<nint>(), 0); // e = 1
    auto end = e;
    ASSERT_EQ(*end->get()->cast<nint>(), 1);
    ++end; // 2
    ASSERT_EQ(*end->get()->cast<nint>(), 2);
    ++end; // 3
    ASSERT_EQ(*end->get()->cast<nint>(), 3);
    ++end; // -1
    ASSERT_EQ(*end->get()->cast<nint>(), -1);
    scope.erase(e, end); // [1 ~ -1), erase 3 elements.
    ASSERT_EQ(scope.size(), 5);

    e = scope.begin();
    ASSERT_EQ(*(e++)->get()->cast<nint>(), 0);
    ASSERT_EQ(*(e++)->get()->cast<nint>(), -1);
    ASSERT_EQ(*(e++)->get()->cast<nint>(), 4);
    ASSERT_EQ(*(e++)->get()->cast<nint>(), 5);
    ASSERT_EQ(*(e++)->get()->cast<nint>(), 6);
    ASSERT_EQ(e, scope.end());

    e = scope.begin("banana");
    ASSERT_NE(e, scope.end());
    scope.erase(e);
    ASSERT_EQ(scope.size(), 4);
    ASSERT_EQ(scope.begin("banana"), scope.end());

    e = scope.begin();
    ASSERT_EQ(*(e++)->get()->cast<nint>(), 0);
    ASSERT_EQ(*(e++)->get()->cast<nint>(), 4);
    ASSERT_EQ(*(e++)->get()->cast<nint>(), 5);
    ASSERT_EQ(*(e++)->get()->cast<nint>(), 6);
    ASSERT_EQ(e, scope.end());
}

TEST_F(smultimapTest, complexEraseTest) {
    for(nint n = 0; n < 10; ++n) {
        scope.insert("apple", *new nInt(n));
        if(n == 5) scope.insert("banana", *new nInt(-1));
    }
    ASSERT_EQ(scope.size(), 10 + 1); // 1 for banana

    auto e1 = scope.begin("apple");
    scope.erase(e1++); // erase 0; and step to 1
    ++e1;              // 2
    ++e1;              // 3
    scope.erase(e1);   // erase '3'

    auto e = scope.begin("apple");
    int expects[] = {1, 2, 4, 5, 6, 7, 8, 9};
    int expectLen = 8;
    for(nint n = 0; n < expectLen; n++) {
        ASSERT_EQ(*e->get()->cast<nint>(), expects[n]);
        ++e;
    }
    ASSERT_EQ(e, scope.end());
    ASSERT_EQ(scope.size(), 9);

    scope.erase("apple"); // erase all apples.
    ASSERT_EQ(scope.begin("apple"), scope.end());
    ASSERT_EQ(scope.size(), 1);

    e = scope.begin("banana");
    ASSERT_EQ(*e.getVal()->get()->cast<nint>(), -1);
}

TEST_F(smultimapTest, deleteMiddleOfElementAndIterateSpecificKey) {
    scope.insert("1", *new nInt(1));
    scope.insert("2", *new nInt(2));
    scope.insert("1", *new nInt(3));
    scope.insert("2", *new nInt(4));
    scope.insert("1", *new nInt(5));
    ASSERT_EQ(scope.size(), 5);

    nint expect = 1;
    for(const auto& e: scope)
        ASSERT_EQ(((nInt&) *e).get(), expect++);

    expect = 1;
    for(const auto& e: scope)
        ASSERT_EQ(((nInt&) *e).get(), expect++);

    auto e = scope.begin("2");
    ASSERT_EQ(*e.getKey(), "2");
    ASSERT_EQ(((nInt*) e->get())->get(), 2);

    ++e;
    ASSERT_EQ(*e.getKey(), "2");
    ASSERT_EQ(((nInt*) e->get())->get(), 4);

    ++e;
    ASSERT_EQ(e, scope.end());
}

TEST_F(smultimapTest, setValue) {
    scope.insert("1", *new nInt(1));
    scope.insert("2", *new nInt(2));
    scope.insert("1", *new nInt(3));
    scope.insert("2", *new nInt(4));
    scope.insert("1", *new nInt(5));
    ASSERT_EQ(scope.size(), 5);

    auto e = scope.begin();
    ++e;
    ++e;
    ASSERT_EQ(((nInt*) e->get())->get(), 3);
    e->bind(*new nInt(100));

    int expects[] = {1, 2, 100, 4, 5};
    int n = 0;
    for(auto& val: scope)
        ASSERT_EQ(((nInt&) *val).get(), expects[n++]);
}

TEST_F(smultimapTest, delElementButKeepOrder) {
    scope.insert("1", *new nInt(1));
    scope.insert("2", *new nInt(2));
    scope.insert("1", *new nInt(3));
    scope.insert("2", *new nInt(4));
    scope.insert("1", *new nInt(5));
    ASSERT_EQ(scope.size(), 5);

    auto e = scope.begin(); // 1
    ++e;                    // 2
    ++e;                    // 3
    scope.erase(e++);
    ASSERT_EQ(((nInt*) e->get())->get(), 4);

    int expects[] = {1, 2, 4, 5};
    int n = 0;
    for(auto& val: scope)
        ASSERT_EQ(((nInt&) *val).get(), expects[n++]);
}

TEST_F(smultimapTest, reverseIterator) {
    scope.insert("1", *new nInt(1));
    scope.insert("2", *new nInt(2));
    scope.insert("1", *new nInt(3));
    scope.insert("2", *new nInt(4));
    scope.insert("1", *new nInt(5));

    int expects[] = {5, 4, 3, 2, 1};
    int n = 0;
    for(auto re = scope.rbegin(); re != scope.rend(); ++re)
        ASSERT_EQ(*re->cast<nint>(), expects[n++]);
}

TEST_F(smultimapTest, delWithIterWithoutKey) {
    scope.insert("1", *new nInt(1));
    scope.insert("2", *new nInt(2));
    scope.insert("1", *new nInt(3));
    scope.insert("2", *new nInt(4));
    scope.insert("1", *new nInt(5));

    auto e = scope.begin() + 3; // `4`, without key.
    scope.erase(e);
    ASSERT_EQ(scope.size(), 4);

    int expects[] = {1, 2, 3, 5};
    int n = 0;
    for(auto e = scope.begin(); e != scope.end(); ++e)
        ASSERT_EQ(*e->cast<nint>(), expects[n++]);
}
