#include "../../common/dep.hpp"

using namespace nm;

TEST(buildFeatureTest, dateGetter) {
    buildFeature::date date;
    ASSERT_GT(date.getYear(), 0);
    ASSERT_GT(date.getMonth(), 0);
    ASSERT_GT(date.getDay(), 0);
    ASSERT_STRNE(date.get().c_str(), "");
}

TEST(buildFeatureTest, timeGetter) {
    ASSERT_GE(buildFeature::time::getHour(), 0);
    ASSERT_GE(buildFeature::time::getMin(), 0);
    ASSERT_GE(buildFeature::time::getSec(), 0);
    ASSERT_STRNE(buildFeature::time::get().c_str(), "");
}

TEST(buildFeatureTest, versionGetter) {
    ASSERT_GE(buildFeature::version::getMajor(), 0);
    ASSERT_GE(buildFeature::version::getMinor(), 0);
    ASSERT_GE(buildFeature::version::getFix(), 0);
    ASSERT_STRNE(buildFeature::version::get().c_str(), "");
    ASSERT_STRNE(buildFeature::version::getValue().c_str(), "");
    ASSERT_STRNE(buildFeature::version::getName().c_str(), "");
}

TEST(buildFeatureTest, platformGetter) {
    buildFeature::platformType type = buildFeature::platform::get();
    ASSERT_GT(type, buildFeature::PLATFORM_TYPE_START);
    ASSERT_LE(type, buildFeature::PLATFORM_TYPE_END);
    ASSERT_STRNE(buildFeature::platform::getVersion().c_str(), "");
    ASSERT_STRNE(buildFeature::platform::getName().c_str(), "");
}
