#include "test/common/dep.hpp"

using namespace by;

TEST(fsystemTest, recursiveDirSearchAtBin) {
    // expect that workdirectory of this executable is at 'bin'.
    auto root = fsystem::find("..");
    nbool buildFound = false;

    while(root.next()) {
        if(*root == "../build/CMakeLists.txt") {
            buildFound = true;
            ASSERT_EQ(root.getName(), "CMakeLists.txt");
            ASSERT_EQ(root.getDir(), "../build");
            break;
        }
    }
    root.rel();
    ASSERT_TRUE(buildFound);
}

TEST(fsystemTest, dirSearchWithPathEndsWithSlash) {
    auto root = fsystem::find("../");
    nbool buildFound = false;

    while(root.next()) {
        if(*root == "../build/CMakeLists.txt") {
            buildFound = true;
            ASSERT_EQ(root.getName(), "CMakeLists.txt");
            ASSERT_EQ(root.getDir(), "../build");
            break;
        }
    }
    root.rel();
    ASSERT_TRUE(buildFound);
}
