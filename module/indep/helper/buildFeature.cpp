#include "indep/helper/buildFeature.hpp"

namespace by {
    BY_DEF_ME(buildFeature)

    const std::string& me::date::get() {
        static std::string inner(BY_BUILD_DATE);
        return inner;
    }

    const std::string& me::time::get() {
        static std::string inner(BY_BUILD_TIME);
        return inner;
    }

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

    const std::string& me::config::getName() {
        static std::string inner(BY_BUILD_TYPENAME);
        return inner;
    }
} // namespace by
