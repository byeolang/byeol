#include "../../namuTest.hpp"

using namespace nm;
using namespace std;

struct helloProgrammerTest: public namuTest {};
/*
TEST_F(helloProgrammerTest, testScript) {
    const std::string script = R"SRC(def programmer
           say := 'html is not a programming language!'
        )SRC";

    tstr<sobj> file = sinterpreter().interp(script);
    ASSERT_TRUE(file);

    sobj& programmer = file->sub("programmer");
    ASSERT_FALSE(nul(programmer));

    sobj& say = programmer["say"];
    ASSERT_FALSE(nul(say));

    ASSERT_STREQ(say.asStr().c_str(), "html is not a programming language!");
}
*/
/*
TEST_F(helloProgrammerTest, testScriptEndsWithEOF) {
    const std::string script = R"SRC(def programmer
           say := 'html is not a programming language!'
           age := 12
    )SRC";

    tstr<sobj> file = sinterpreter().interp(script);
    ASSERT_TRUE(file);

    sobj& programmer = file->sub("programmer");
    ASSERT_FALSE(nul(programmer));

    sobj& say = programmer["say"];
    ASSERT_FALSE(nul(say));
    ASSERT_STREQ(say.asStr().c_str(), "html is not a programming language!");

    sobj& age = programmer["age"];
    ASSERT_FALSE(nul(age));
    ASSERT_EQ(age.asInt(), 12);
}

TEST_F(helloProgrammerTest, testScriptWithPackScope) {
    const std::string script =
R"SRC(say := 'html is not a programming language!'
age := 12
def ramen
  name := 'doshirak'
    )SRC";

    tstr<sobj> file = sinterpreter().interp(script);
    ASSERT_TRUE(file);

    sobj& say = file->sub("say");
    ASSERT_FALSE(nul(say));
    ASSERT_STREQ(say.asStr().c_str(), "html is not a programming language!");

    sobj& ramen = file->sub("ramen");
    ASSERT_FALSE(nul(ramen));

    {
        sobj& name = ramen["name"];
        ASSERT_FALSE(nul(name));
        ASSERT_STREQ(name.asStr().c_str(), "doshirak");
    }
}

TEST_F(helloProgrammerTest, testNullObjNegative) {

    const std::string script =
R"SRC(say := 'html is not a programming language!'
age := 12
def ramen
 name := 'doshirak'
    )SRC";

    tstr<sobj> file = sinterpreter().interp(script);
    ASSERT_TRUE(file);

    sobj& notExist = file->sub("dogecoin");
    ASSERT_FALSE(notExist);
    ASSERT_STREQ(notExist.asStr().c_str(), "");
    ASSERT_EQ(notExist.asInt(), 0);
    ASSERT_EQ(notExist.asBool(), false);
    ASSERT_EQ(notExist.asChar(), '\0');
}

TEST_F(helloProgrammerTest, testVerObject) {
    const std::string script =
R"SRC(def man
   dummy := 5
   name := 'dark souls'
   ver := 1.0.8
)SRC";

    tstr<sobj> file = sinterpreter().interp(script);
    ASSERT_TRUE(file);
    file = sinterpreter().interp(script);
    ASSERT_TRUE(file);

    sobj& man = file->sub("man");
    sobj& name = man["name"];
    ASSERT_FALSE(nul(name));

    ASSERT_STREQ(name.asStr().c_str(), "dark souls");

    verSobj& ver = man["ver"].cast<verSobj>();
    ASSERT_FALSE(nul(ver));
    ASSERT_STREQ(ver.asStr().c_str(), "1.0.8");
    ASSERT_EQ(ver.asMajor(), 1);
    ASSERT_EQ(ver.asMinor(), 0);
    ASSERT_EQ(ver.asFix(), 8);

    ASSERT_TRUE(ver >= verSobj(1, 0, 7));
    ASSERT_TRUE(ver >= verSobj(1, 0, 8));
    ASSERT_TRUE(ver == verSobj(1, 0, 8));
    ASSERT_FALSE(ver < verSobj(1, 0, 8));
    ASSERT_TRUE(ver < verSobj(1, 1, 8));
    ASSERT_FALSE(ver < verSobj(0, 2, 8));
    ASSERT_TRUE(ver > verSobj(0, 2, 8));
}

TEST_F(helloProgrammerTest, testNullThisTest) {
    const std::string script =
R"SRC(def empty
    name := 'wow'
    )SRC";

    tstr<sobj> file = sinterpreter().interp(script);
    ASSERT_TRUE(file);

    sobj& name = file->sub("empty")["name"];
    ASSERT_TRUE(name);
    sobj& shouldNotExist = name["food"]["noodle"]["Jajangmyeon"];
    ASSERT_FALSE(shouldNotExist);
}

TEST_F(helloProgrammerTest, testIteration) {
    const std::string script =
R"SRC(def person
   name := 'gorden freeman'
   lookingFor := 'crowbar'
   whenWillThereBeAHalflife := 3
    )SRC";

    tstr<sobj> file = sinterpreter().interp(script);
    ASSERT_TRUE(file);

    map<string, string> expects;
    expects.insert(make_pair("name", "gorden freeman"));
    expects.insert(make_pair("lookingFor", "crowbar"));
    expects.insert(make_pair("whenWillThereBeAHalflife", "3"));

    sobj& person = file->sub("person");
    ASSERT_EQ(expects.size(), person.len());

    for(const auto& pair : person) {
        const string& name = pair.first;

        ASSERT_STREQ(pair.second->asStr().c_str(), expects[name].c_str());
    }
}

TEST_F(helloProgrammerTest, testNullThisAccess) {
    const std::string script =
R"SRC(def empty
    name := 'wow'
    )SRC";

    tstr<sobj> file = sinterpreter().interp(script);
    ASSERT_TRUE(file);

    string shouldExist = file->sub("null obj")["and so null"].asStr();
    ASSERT_FALSE(nul(shouldExist));
    ASSERT_STREQ(shouldExist.c_str(), "");
}

TEST_F(helloProgrammerTest, testManifestScript) {
    const std::string script =
R"SRC(def entrypoints
   def cpp
       path := './libsamplePack.pack'
name := 'samplePack'
ver := 2.1.0
author := 'kniz'
    )SRC";

    tstr<sobj> file = sinterpreter().interp(script);
    ASSERT_TRUE(file);

    ASSERT_TRUE(file->sub("ver").cast<verSobj>() < verSobj(2, 1, 1));
    ASSERT_STREQ(file->sub("author").asStr().c_str(), "kniz");

    sobj& entrys = file->sub("entrypoints");
    ASSERT_FALSE(nul(entrys));

    ASSERT_EQ(entrys.len(), 1);
    ASSERT_STREQ(entrys["cpp"]["path"].asStr().c_str(), "./libsamplePack.pack");
}*/
