#include <stdio.h>

#include "../../common/dep.hpp"

using namespace std;
using namespace nm;

namespace {
    typedef struct consoleStreamTest: public ::testing::Test {
        void SetUp() override {
            logger& log = logger::get();
            _isVerbose = log.isEnable();
            log.setEnable(true);
            delLogFile();
            ASSERT_FALSE(consoleStreamTest::hasLogFile());
        }

        void TearDown() override {
            delLogFile();
            logger::get().setEnable(_isVerbose);
        }

        static void delLogFile() {
            NM_I("delLogFile");
            std::remove(getLogFilePath());
        }

        static const char* getLogFilePath() {
            static const char* name = NULL;

            if(name == NULL) {
                logger& log = logger::get();
                for(int n = 0; n < log.getStreamCount(); n++) {
                    fileLogStream* as = dynamic_cast<fileLogStream*>(&log[n]);
                    if(as) {
                        name = as->getPath().c_str();
                        break;
                    }
                }
            }
            return name;
        }

        static nbool hasLogFile() {
            struct stat buffer;
            return stat(getLogFilePath(), &buffer) == 0;
        }

    private:
        nbool _isVerbose;

    } thisTest;
} // namespace

TEST_F(consoleStreamTest, logFormat) {
    logger::get().logBypass("hello");
    logger::get().logFormatBypass(
        "%s %s <%s::%s#%d> "
        "hello",
        nm::platformAPI::createNowTime("%b %d %Y  %X").c_str(), "I", __FILENAME__, __func__,
        __LINE__);

    ASSERT_TRUE(thisTest::hasLogFile());
}

TEST_F(consoleStreamTest, macro) {
    ASSERT_FALSE(thisTest::hasLogFile());

    NM_W("world!");
    NM_E("this is not an error.");

    ASSERT_TRUE(thisTest::hasLogFile());
}

TEST_F(consoleStreamTest, debugLogFormat) {
    nbool isDbg = buildFeature::config::get() == buildFeature::DEBUG;

    NM_DW("if it's debug mode, this msg should be shown.");

    ASSERT_EQ(isDbg, thisTest::hasLogFile());
}

TEST_F(consoleStreamTest, richLogTest) {
    richLog(errLv::INFO, "filename", "func", 1, "%s.%s\n", nInt(1), obj());
}
