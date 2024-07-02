#include "buildFeature.hpp"

namespace nm {

    using namespace std;

    NAMU_DEF_ME(buildFeature)

    nint me::date::getYear() {
        return NAMU_BUILD_DATE_YEAR;
    }

    nint me::date::getMonth() {
        return NAMU_BUILD_DATE_MONTH;
    }

    nint me::date::getDay() {
        return NAMU_BUILD_DATE_DAY;
    }

    nint me::time::getHour() {
        return NAMU_BUILD_TIME_HOUR;
    }

    nint me::time::getMin() {
        return NAMU_BUILD_TIME_MINUTE;
    }

    nint me::time::getSec() {
        return NAMU_BUILD_TIME_SECOND;
    }

    nint me::version::getMajor() {
        return NAMU_BUILD_VERSION_MAJOR;
    }

    nint me::version::getMinor() {
        return NAMU_BUILD_VERSION_MINOR;
    }

    nint me::version::getFix() {
        return NAMU_BUILD_VERSION_FIX;
    }

    me::platformType me::platform::get() {
        static platformType inner = PLATFORM_TYPE_START;
        if(inner == PLATFORM_TYPE_START)
        {
            const string& name = getName();
            const char* map[] = {"Windows", "Linux", "Darwin", "Others", nullptr};
            int n=-1;
            while(map[++n])
                if(name == map[n])
                {
                    inner = platformType(n);
                    break;
                }
        }

        return inner;
    }

    me::buildType me::config::get() {
        static buildType inner = BUILD_TYPE_START;
        if(inner == BUILD_TYPE_START)
        {
            const string& name = getName();
            const char* map[] = {"Debug", "Release", nullptr};
            int n=-1;
            while(map[++n])
                if(name == map[n])
                {
                    inner = buildType(n);
                    break;
                }
        }

        return inner;
    }

    nbool me::config::isDbg() {
#if NAMU_IS_DBG
        return true;
#else
        return false;
#endif
    }
}
