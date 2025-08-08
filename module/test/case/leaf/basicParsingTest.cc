#include "test/byeolTest.hpp"

using namespace nm;
using namespace std;

struct basicParsing: public byeolTest {};

TEST_F(basicParsing, testScript) {
    const std::string script = R"SRC(
        def programmer
           say := "html is not a programming language!"
    )SRC";

    tstr<leaf> root = leafParser().parse(script);
    ASSERT_TRUE(root);

    leaf& programmer = root->sub("programmer");
    ASSERT_TRUE(programmer.isExist());

    leaf& say = programmer["say"];
    ASSERT_TRUE(say.isExist());

    ASSERT_STREQ(say.asStr().c_str(), "html is not a programming language!");
}

TEST_F(basicParsing, testScriptEndsWithEOF) {
    const std::string script = R"SRC(
        def programmer
           say := "html is not a programming language!"
           age := 12)SRC";

    tstr<leaf> root = leafParser().parse(script);
    ASSERT_TRUE(root);

    leaf& programmer = root->sub("programmer");
    ASSERT_TRUE(programmer.isExist());

    leaf& say = programmer["say"];
    ASSERT_TRUE(say.isExist());
    ASSERT_STREQ(say.asStr().c_str(), "html is not a programming language!");

    leaf& age = programmer["age"];
    ASSERT_TRUE(age.isExist());
    ASSERT_EQ(age.asInt(), 12);
}

TEST_F(basicParsing, testScriptWithPackScope) {
    const std::string script = R"SRC(
        say := "html is not a programming language!"
        age := 12
        def ramen
            name := "doshirak"
    )SRC";

    tstr<leaf> file = leafParser().parse(script);
    ASSERT_TRUE(file);

    leaf& say = file->sub("say");
    ASSERT_TRUE(say.isExist());
    ASSERT_STREQ(say.asStr().c_str(), "html is not a programming language!");

    leaf& ramen = file->sub("ramen");
    ASSERT_TRUE(ramen.isExist());
    {
        leaf& name = ramen["name"];
        ASSERT_TRUE(name.isExist());
        ASSERT_STREQ(name.asStr().c_str(), "doshirak");
    }
}

TEST_F(basicParsing, testNullObjNegative) {
    const std::string script = R"SRC(
        say := "html is not a programming language!"
        age := 12
        def ramen
            name := "doshirak"
    )SRC";

    tstr<leaf> file = leafParser().parse(script);
    ASSERT_TRUE(file);

    leaf& notExist = file->sub("dogecoin");
    ASSERT_FALSE(notExist);
    ASSERT_STREQ(notExist.asStr().c_str(), "");
    ASSERT_EQ(notExist.asInt(), 0);
    ASSERT_EQ(notExist.asBool(), false);
    ASSERT_EQ(notExist.asChar(), '\0');
}

TEST_F(basicParsing, testVerObject) {
    const std::string script = R"SRC(
        def man
            dummy := 5
            name := "dark souls"
            ver := 1.0.8
    )SRC";

    tstr<leaf> root = leafParser().parse(script);
    ASSERT_TRUE(root);

    root = leafParser().parse(script);
    ASSERT_TRUE(root);

    leaf& man = root->sub("man");
    leaf& name = man["name"];
    ASSERT_TRUE(name);

    ASSERT_STREQ(name.asStr().c_str(), "dark souls");

    verLeaf& ver = man["ver"].cast<verLeaf>() OR_ASSERT(ver);
    ASSERT_STREQ(ver.asStr().c_str(), "1.0.8");
    ASSERT_EQ(ver.asMajor(), 1);
    ASSERT_EQ(ver.asMinor(), 0);
    ASSERT_EQ(ver.asFix(), 8);

    ASSERT_TRUE(ver >= verLeaf(1, 0, 7));
    ASSERT_TRUE(ver >= verLeaf(1, 0, 8));
    ASSERT_TRUE(ver == verLeaf(1, 0, 8));
    ASSERT_FALSE(ver < verLeaf(1, 0, 8));
    ASSERT_TRUE(ver < verLeaf(1, 1, 8));
    ASSERT_FALSE(ver < verLeaf(0, 2, 8));
    ASSERT_TRUE(ver > verLeaf(0, 2, 8));
}

TEST_F(basicParsing, testNullThisTest) {
    const std::string script = R"SRC(
        def empty
            name := "wow"
    )SRC";

    tstr<leaf> file = leafParser().parse(script);
    ASSERT_TRUE(file);

    leaf& name = file->sub("empty")["name"];
    ASSERT_TRUE(name);
    leaf& shouldNotExist = name["food"]["noodle"]["Jajangmyeon"];
    ASSERT_FALSE(shouldNotExist);
}

TEST_F(basicParsing, testIteration) {
    const std::string script = R"SRC(
        def person
            name := "gorden freeman"
            lookingFor := "crowbar"
            whenWillThereBeAHalflife := 3
    )SRC";

    tstr<leaf> file = leafParser().parse(script);
    ASSERT_TRUE(file);

    map<string, string> expects;
    expects.insert(make_pair("name", "gorden freeman"));
    expects.insert(make_pair("lookingFor", "crowbar"));
    expects.insert(make_pair("whenWillThereBeAHalflife", "3"));

    leaf& person = file->sub("person");
    ASSERT_EQ(expects.size(), person.len());

    for(const auto& pair: person) {
        const string& name = pair.first;

        ASSERT_STREQ(pair.second->asStr().c_str(), expects[name].c_str());
    }
}

TEST_F(basicParsing, testNullThisAccess) {
    const std::string script = R"SRC(
        def empty
            name := "wow"
    )SRC";

    tstr<leaf> file = leafParser().parse(script);
    ASSERT_TRUE(file);

    string shouldExist = file->sub("null obj")["and so null"].asStr();
    ASSERT_STREQ(shouldExist.c_str(), "");
}

TEST_F(basicParsing, testManifestScript) {
    const std::string script = R"SRC(
        def entrypoints
            def cpp
                path := "./libsamplePack.pack"
                name := "samplePack"
        ver := 2.1.0
        author := "kniz"
    )SRC";

    tstr<leaf> root = leafParser().parse(script);
    ASSERT_TRUE(root);

    verLeaf& rootVer = root->sub("ver").cast<verLeaf>() OR_ASSERT(rootVer);
    ASSERT_TRUE(rootVer < verLeaf(2, 1, 1));
    ASSERT_STREQ(root->sub("author").asStr().c_str(), "kniz");

    leaf& entrys = root->sub("entrypoints");
    ASSERT_TRUE(entrys);

    ASSERT_EQ(entrys.len(), 1);
    ASSERT_STREQ(entrys["cpp"]["path"].asStr().c_str(), "./libsamplePack.pack");
}
