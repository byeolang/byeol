#include "test/byeolTest.hpp"

using namespace by;
using namespace std;

struct basicParsing: public byeolTest {};

TEST_F(basicParsing, testScript) {
    const std::string script = R"SRC(
        def programmer
           say := "html is not a programming language!"
    )SRC";

    tstr<stela> root = stelaParser().parse(script);
    ASSERT_TRUE(root);

    stela& programmer = root->sub("programmer");
    ASSERT_TRUE(programmer.isExist());

    stela& say = programmer["say"];
    ASSERT_TRUE(say.isExist());

    ASSERT_STREQ(say.asStr().c_str(), "html is not a programming language!");
}

TEST_F(basicParsing, testScriptEndsWithEOF) {
    const std::string script = R"SRC(
        def programmer
           say := "html is not a programming language!"
           age := 12)SRC";

    tstr<stela> root = stelaParser().parse(script);
    ASSERT_TRUE(root);

    stela& programmer = root->sub("programmer");
    ASSERT_TRUE(programmer.isExist());

    stela& say = programmer["say"];
    ASSERT_TRUE(say.isExist());
    ASSERT_STREQ(say.asStr().c_str(), "html is not a programming language!");

    stela& age = programmer["age"];
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

    tstr<stela> file = stelaParser().parse(script);
    ASSERT_TRUE(file);

    stela& say = file->sub("say");
    ASSERT_TRUE(say.isExist());
    ASSERT_STREQ(say.asStr().c_str(), "html is not a programming language!");

    stela& ramen = file->sub("ramen");
    ASSERT_TRUE(ramen.isExist());
    {
        stela& name = ramen["name"];
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

    tstr<stela> file = stelaParser().parse(script);
    ASSERT_TRUE(file);

    stela& notExist = file->sub("dogecoin");
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

    tstr<stela> root = stelaParser().parse(script);
    ASSERT_TRUE(root);

    root = stelaParser().parse(script);
    ASSERT_TRUE(root);

    stela& man = root->sub("man");
    stela& name = man["name"];
    ASSERT_TRUE(name);

    ASSERT_STREQ(name.asStr().c_str(), "dark souls");

    verStela& ver = man["ver"].cast<verStela>() OR_ASSERT(ver);
    ASSERT_STREQ(ver.asStr().c_str(), "1.0.8");
    ASSERT_EQ(ver.asMajor(), 1);
    ASSERT_EQ(ver.asMinor(), 0);
    ASSERT_EQ(ver.asFix(), 8);

    ASSERT_TRUE(ver >= verStela(1, 0, 7));
    ASSERT_TRUE(ver >= verStela(1, 0, 8));
    ASSERT_TRUE(ver == verStela(1, 0, 8));
    ASSERT_FALSE(ver < verStela(1, 0, 8));
    ASSERT_TRUE(ver < verStela(1, 1, 8));
    ASSERT_FALSE(ver < verStela(0, 2, 8));
    ASSERT_TRUE(ver > verStela(0, 2, 8));
}

TEST_F(basicParsing, testNullThisTest) {
    const std::string script = R"SRC(
        def empty
            name := "wow"
    )SRC";

    tstr<stela> file = stelaParser().parse(script);
    ASSERT_TRUE(file);

    stela& name = file->sub("empty")["name"];
    ASSERT_TRUE(name);
    stela& shouldNotExist = name["food"]["noodle"]["Jajangmyeon"];
    ASSERT_FALSE(shouldNotExist);
}

TEST_F(basicParsing, testIteration) {
    const std::string script = R"SRC(
        def person
            name := "gorden freeman"
            lookingFor := "crowbar"
            whenWillThereBeAHalflife := 3
    )SRC";

    tstr<stela> file = stelaParser().parse(script);
    ASSERT_TRUE(file);

    map<string, string> expects;
    expects.insert(make_pair("name", "gorden freeman"));
    expects.insert(make_pair("lookingFor", "crowbar"));
    expects.insert(make_pair("whenWillThereBeAHalflife", "3"));

    stela& person = file->sub("person");
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

    tstr<stela> file = stelaParser().parse(script);
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

    tstr<stela> root = stelaParser().parse(script);
    ASSERT_TRUE(root);

    verStela& rootVer = root->sub("ver").cast<verStela>() OR_ASSERT(rootVer);
    ASSERT_TRUE(rootVer < verStela(2, 1, 1));
    ASSERT_STREQ(root->sub("author").asStr().c_str(), "kniz");

    stela& entrys = root->sub("entrypoints");
    ASSERT_TRUE(entrys);

    ASSERT_EQ(entrys.len(), 1);
    ASSERT_STREQ(entrys["cpp"]["path"].asStr().c_str(), "./libsamplePack.pack");
}

TEST_F(basicParsing, testValStelaConstructors) {
    valStela boolVal(true, "flag");
    ASSERT_STREQ(boolVal.asStr().c_str(), "true");
    ASSERT_TRUE(boolVal.asBool());

    valStela boolFalse(false, "disabled");
    ASSERT_STREQ(boolFalse.asStr().c_str(), "false");
    ASSERT_FALSE(boolFalse.asBool());

    valStela intVal(42, "number");
    ASSERT_EQ(intVal.asInt(), 42);
    ASSERT_STREQ(intVal.asStr().c_str(), "42");

    valStela floatVal(3.14f, "pi");
    ASSERT_STREQ(floatVal.asStr().substr(0, 4).c_str(), "3.14");
}

TEST_F(basicParsing, testValStelaAsChar) {
    const std::string script = R"SRC(
        letter := "A"
        empty := ""
    )SRC";

    tstr<stela> root = stelaParser().parse(script);
    ASSERT_TRUE(root);

    stela& letter = root->sub("letter");
    ASSERT_EQ(letter.asChar(), 'A');

    stela& empty = root->sub("empty");
    ASSERT_EQ(empty.asChar(), '\0');
}

TEST_F(basicParsing, testValStelaAsBool) {
    const std::string script = R"SRC(
        t1 := "true"
        f1 := "false"
        t2 := "TRUE"
        f2 := "FALSE"
        nz := "5"
        z := "0"
    )SRC";

    tstr<stela> root = stelaParser().parse(script);
    ASSERT_TRUE(root);

    ASSERT_TRUE(root->sub("t1").asBool());
    ASSERT_FALSE(root->sub("f1").asBool());
    ASSERT_TRUE(root->sub("t2").asBool());
    ASSERT_FALSE(root->sub("f2").asBool());
    ASSERT_TRUE(root->sub("nz").asBool());
    ASSERT_FALSE(root->sub("z").asBool());
}

TEST_F(basicParsing, testStelaDelMethod) {
    const std::string script = R"SRC(
        name := "test"
        age := 25
        city := "seoul"
    )SRC";

    tstr<stela> root = stelaParser().parse(script);
    ASSERT_TRUE(root);
    ASSERT_EQ(root->len(), 3);

    root->del("age");
    ASSERT_EQ(root->len(), 2);
    ASSERT_FALSE(root->has("age"));
    ASSERT_TRUE(root->has("name"));

    stela& cityRef = root->sub("city");
    root->del(cityRef);
    ASSERT_EQ(root->len(), 1);
    ASSERT_FALSE(root->has("city"));

    root->del((const nchar*) "name");
    ASSERT_EQ(root->len(), 0);
}

TEST_F(basicParsing, testStelaSetName) {
    stela node("original");
    ASSERT_STREQ(node.getName().c_str(), "original");

    node.setName("renamed");
    ASSERT_STREQ(node.getName().c_str(), "renamed");

    node.setName((const nchar*) "cstring");
    ASSERT_STREQ(node.getName().c_str(), "cstring");

    node.setName((const nchar*) nullptr);
    ASSERT_STREQ(node.getName().c_str(), "cstring");
}

TEST_F(basicParsing, testStelaIndexAccess) {
    const std::string script = R"SRC(
        first := "a"
        second := "b"
        third := "c"
    )SRC";

    tstr<stela> root = stelaParser().parse(script);
    ASSERT_TRUE(root);

    stela& elem0 = root->sub(0);
    ASSERT_TRUE(elem0);

    stela& elem1 = root->sub(1);
    ASSERT_TRUE(elem1);

    stela& elem2 = root->sub(2);
    ASSERT_TRUE(elem2);

    stela& outOfBounds = root->sub(10);
    ASSERT_FALSE(outOfBounds);

    stela& negative = root->sub(-1);
    ASSERT_FALSE(negative);
}

TEST_F(basicParsing, testStelaCStringOperator) {
    const std::string script = R"SRC(
        def config
            host := "localhost"
            port := 8080
    )SRC";

    tstr<stela> root = stelaParser().parse(script);
    ASSERT_TRUE(root);

    stela& config = (*root)[(const nchar*) "config"];
    ASSERT_TRUE(config);

    stela& host = config[(const nchar*) "host"];
    ASSERT_STREQ(host.asStr().c_str(), "localhost");

    stela& notExist = config[(const nchar*) nullptr];
    ASSERT_FALSE(notExist);
}

TEST_F(basicParsing, testStelaDefaultValues) {
    stela emptyNode("empty");

    ASSERT_EQ(emptyNode.asInt(), 0);
    ASSERT_EQ(emptyNode.asChar(), '\0');
    ASSERT_STREQ(emptyNode.asStr().c_str(), "");
    ASSERT_FALSE(emptyNode.asBool());
    ASSERT_TRUE(emptyNode.isExist());
}

TEST_F(basicParsing, testComplexNestedScript) {
    const std::string script = R"SRC(
        def database
            def connection
                host := "localhost"
                port := 5432
                timeout := 30
            def credentials
                user := "admin"
                password := "secret"
        def logging
            level := "info"
            enabled := true
    )SRC";

    tstr<stela> root = stelaParser().parse(script);
    ASSERT_TRUE(root);

    stela& db = root->sub("database");
    ASSERT_TRUE(db);

    stela& conn = db["connection"];
    ASSERT_EQ(conn["port"].asInt(), 5432);
    ASSERT_EQ(conn["timeout"].asInt(), 30);

    stela& creds = db["credentials"];
    ASSERT_STREQ(creds["user"].asStr().c_str(), "admin");

    stela& log = root->sub("logging");
    ASSERT_TRUE(log["enabled"].asBool());
}
