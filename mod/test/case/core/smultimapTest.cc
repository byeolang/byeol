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
