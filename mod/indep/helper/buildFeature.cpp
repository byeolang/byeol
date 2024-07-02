#include "buildFeature.hpp"

namespace nm {

    using namespace std;

    NM_DEF_ME(buildFeature)

    nint me::date::getYear() {
        return NM_BUILD_DATE_YEAR;
    }

    nint me::date::getMonth() {
        return NM_BUILD_DATE_MONTH;
    }

    nint me::date::getDay() {
        return NM_BUILD_DATE_DAY;
    }

    nint me::time::getHour() {
        return NM_BUILD_TIME_HOUR;
    }

    nint me::time::getMin() {
        return NM_BUILD_TIME_MINUTE;
    }

    nint me::time::getSec() {
        return NM_BUILD_TIME_SECOND;
    }

    nint me::version::getMajor() {
        return NM_BUILD_VERSION_MAJOR;
    }

    nint me::version::getMinor() {
        return NM_BUILD_VERSION_MINOR;
    }

    nint me::version::getFix() {
        return NM_BUILD_VERSION_FIX;
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
#if NM_IS_DBG
        return true;
#else
        return false;
#endif
    }
}
