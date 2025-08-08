#include "indep/helper/buildFeature.hpp"

namespace by {
    BY_DEF_ME(buildFeature)

    const std::string& me::date::get() {
        static std::string inner(BY_BUILD_DATE);
        return inner;
    }

    nint me::date::getYear() { return BY_BUILD_DATE_YEAR; }

    nint me::date::getMonth() { return BY_BUILD_DATE_MONTH; }

    nint me::date::getDay() { return BY_BUILD_DATE_DAY; }

    const std::string& me::time::get() {
        static std::string inner(BY_BUILD_TIME);
        return inner;
    }

    nint me::time::getHour() { return BY_BUILD_TIME_HOUR; }

    nint me::time::getMin() { return BY_BUILD_TIME_MINUTE; }

    nint me::time::getSec() { return BY_BUILD_TIME_SECOND; }

    const std::string& me::version::get() {
        static std::string inner(BY_BUILD_VERSION);
        return inner;
    }

    const std::string& me::version::getValue() {
        static std::string inner(BY_BUILD_VERSION_VALUE);
        return inner;
    }

    const std::string& me::version::getName() {
        static std::string inner(BY_BUILD_VERSION_NAME);
        return inner;
    }

    nint me::version::getMajor() { return BY_BUILD_VERSION_MAJOR; }

    nint me::version::getMinor() { return BY_BUILD_VERSION_MINOR; }

    nint me::version::getFix() { return BY_BUILD_VERSION_FIX; }

    me::platformType me::platform::get() {
        static platformType inner = PLATFORM_TYPE_START;
        if(inner == PLATFORM_TYPE_START) {
            const std::string& name = getName();
            const char* map[] = {"Windows", "Linux", "Darwin", "Others", nullptr};
            int n = -1;
            while(map[++n])
                if(name == map[n]) {
                    inner = platformType(n);
                    break;
                }
        }

        return inner;
    }

    const std::string& me::platform::getName() {
        static std::string inner(BY_BUILD_PLATFORM_NAME);
        return inner;
    }

    const std::string& me::platform::getVersion() {
        static std::string inner(BY_BUILD_PLATFORM_VERSION);
        return inner;
    }

    me::buildType me::config::get() {
        static buildType inner = BUILD_TYPE_START;
        if(inner == BUILD_TYPE_START) {
            const std::string& name = getName();
            const char* map[] = {"Debug", "Release", nullptr};
            int n = -1;
            while(map[++n])
                if(name == map[n]) {
                    inner = buildType(n);
                    break;
                }
        }

        return inner;
    }

    nbool me::config::isDbg() {
#if BY_IS_DBG
        return true;
#else
        return false;
#endif
    }

    const std::string& me::config::getName() {
        static std::string inner(BY_BUILD_TYPENAME);
        return inner;
    }
} // namespace by
