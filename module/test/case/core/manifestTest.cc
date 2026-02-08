#include "test/byeolTest.hpp"

using namespace by;
using namespace std;

struct manifestTest: public byeolTest {};

TEST_F(manifestTest, testEntrypointConstructor) {
    vector<string> paths = {"/path/to/file1.byeol", "/path/to/file2.byeol"};
    entrypoint ep("byeol", paths);

    ASSERT_EQ(ep.lang, "byeol");
    ASSERT_EQ(ep.paths.size(), 2);
    ASSERT_EQ(ep.paths[0], "/path/to/file1.byeol");
    ASSERT_EQ(ep.paths[1], "/path/to/file2.byeol");
}

TEST_F(manifestTest, testEntrypointIsValidWithValidData) {
    vector<string> paths = {"/some/path.byeol"};
    entrypoint ep("byeol", paths);

    ASSERT_TRUE(ep.isValid());
}

TEST_F(manifestTest, testEntrypointIsValidWithEmptyLang) {
    vector<string> paths = {"/some/path.byeol"};
    entrypoint ep("", paths);

    ASSERT_FALSE(ep.isValid());
}

TEST_F(manifestTest, testEntrypointIsValidWithEmptyPaths) {
    vector<string> paths;
    entrypoint ep("byeol", paths);

    ASSERT_FALSE(ep.isValid());
}

TEST_F(manifestTest, testEntrypointWithMultiplePaths) {
    vector<string> paths = {"path1.byeol", "path2.byeol", "path3.byeol", "path4.byeol"};
    entrypoint ep("managed", paths);

    ASSERT_TRUE(ep.isValid());
    ASSERT_EQ(ep.paths.size(), 4);
}

TEST_F(manifestTest, testManifestDefaultConstructor) {
    manifest m;

    ASSERT_EQ(m.name, manifest::DEFAULT_NAME);
    ASSERT_EQ(m.filePath, "");
    ASSERT_EQ(m.author, "unknown");
    ASSERT_EQ(m.ver, "");
    ASSERT_EQ(m.points.size(), 0);
}

TEST_F(manifestTest, testManifestConstructorWithName) {
    manifest m("myPack");

    ASSERT_EQ(m.name, "myPack");
    ASSERT_EQ(m.filePath, "");
    ASSERT_EQ(m.author, "unknown");
    ASSERT_EQ(m.ver, "");
}

TEST_F(manifestTest, testManifestFullConstructor) {
    vector<string> paths = {"main.byeol"};
    entrypoint ep("byeol", paths);
    entrypoints points = {ep};

    manifest m("testPack", "/path/to/pack", "John Doe", "1.0.0", points);

    ASSERT_EQ(m.name, "testPack");
    ASSERT_EQ(m.filePath, "/path/to/pack");
    ASSERT_EQ(m.author, "John Doe");
    ASSERT_EQ(m.ver, "1.0.0");
    ASSERT_EQ(m.points.size(), 1);
}

TEST_F(manifestTest, testManifestIsValidWithFullData) {
    vector<string> paths = {"main.byeol"};
    entrypoint ep("byeol", paths);
    entrypoints points = {ep};

    manifest m("pack1", "/path", "author", "1.0", points);

    ASSERT_TRUE(m.isValid());
}

TEST_F(manifestTest, testManifestIsValidWithEmptyName) {
    vector<string> paths = {"main.byeol"};
    entrypoint ep("byeol", paths);
    entrypoints points = {ep};

    manifest m("", "/path", "author", "1.0", points);

    ASSERT_FALSE(m.isValid());
}

TEST_F(manifestTest, testManifestIsValidWithEmptyFilePath) {
    vector<string> paths = {"main.byeol"};
    entrypoint ep("byeol", paths);
    entrypoints points = {ep};

    manifest m("pack1", "", "author", "1.0", points);

    ASSERT_FALSE(m.isValid());
}

TEST_F(manifestTest, testManifestIsValidWithEmptyAuthor) {
    vector<string> paths = {"main.byeol"};
    entrypoint ep("byeol", paths);
    entrypoints points = {ep};

    manifest m("pack1", "/path", "", "1.0", points);

    ASSERT_FALSE(m.isValid());
}

TEST_F(manifestTest, testManifestIsValidWithEmptyVersion) {
    vector<string> paths = {"main.byeol"};
    entrypoint ep("byeol", paths);
    entrypoints points = {ep};

    manifest m("pack1", "/path", "author", "", points);

    ASSERT_FALSE(m.isValid());
}

TEST_F(manifestTest, testManifestIsValidWithInvalidEntrypoint) {
    vector<string> emptyPaths;
    entrypoint invalidEp("byeol", emptyPaths);
    entrypoints points = {invalidEp};

    manifest m("pack1", "/path", "author", "1.0", points);

    ASSERT_FALSE(m.isValid());
}

TEST_F(manifestTest, testManifestWithMultipleEntrypoints) {
    vector<string> paths1 = {"main.byeol"};
    vector<string> paths2 = {"lib.byeol", "utils.byeol"};

    entrypoint ep1("byeol", paths1);
    entrypoint ep2("byeol", paths2);
    entrypoints points = {ep1, ep2};

    manifest m("multiPack", "/multi/path", "Developer", "2.0.0", points);

    ASSERT_TRUE(m.isValid());
    ASSERT_EQ(m.points.size(), 2);
}

TEST_F(manifestTest, testManifestWithMixedValidAndInvalidEntrypoints) {
    vector<string> validPaths = {"valid.byeol"};
    vector<string> emptyPaths;

    entrypoint validEp("byeol", validPaths);
    entrypoint invalidEp("", emptyPaths);
    entrypoints points = {validEp, invalidEp};

    manifest m("pack1", "/path", "author", "1.0", points);

    ASSERT_FALSE(m.isValid());
}

TEST_F(manifestTest, testDefaultManifestIsInvalid) {
    manifest m;

    ASSERT_FALSE(m.isValid());
}

TEST_F(manifestTest, testManifestWithDifferentLanguages) {
    vector<string> byeolPaths = {"main.byeol"};
    vector<string> nativePaths = {"lib.so"};

    entrypoint byeolEp("byeol", byeolPaths);
    entrypoint nativeEp("native", nativePaths);
    entrypoints points = {byeolEp, nativeEp};

    manifest m("hybridPack", "/hybrid", "Dev Team", "1.5.0", points);

    ASSERT_TRUE(m.isValid());
    ASSERT_EQ(m.points[0].lang, "byeol");
    ASSERT_EQ(m.points[1].lang, "native");
}

TEST_F(manifestTest, testEntrypointModifyAfterConstruction) {
    vector<string> paths = {"initial.byeol"};
    entrypoint ep("byeol", paths);

    ep.lang = "native";
    ep.paths.push_back("additional.so");

    ASSERT_EQ(ep.lang, "native");
    ASSERT_EQ(ep.paths.size(), 2);
    ASSERT_TRUE(ep.isValid());
}

TEST_F(manifestTest, testManifestModifyAfterConstruction) {
    manifest m("original");

    m.name = "modified";
    m.filePath = "/new/path";
    m.author = "New Author";
    m.ver = "2.0.0";

    ASSERT_EQ(m.name, "modified");
    ASSERT_EQ(m.filePath, "/new/path");
}
