#pragma once

#include "indep/common.hpp"
#include "indep/macro.hpp"

namespace by {
    class _nout buildFeature {
        BY_ME(buildFeature)

    public:
        struct _nout date {
            BY_ME(date)

        public:
            static const std::string& get();
            static nint getYear();
            static nint getMonth();
            static nint getDay();
        };

        struct _nout time {
            BY_ME(time)

        public:
            static const std::string& get();
            static nint getHour();
            static nint getMin();
            static nint getSec();
        };

        struct _nout version {
            BY_ME(version)

        public:
            static const std::string& get();
            static const std::string& getValue();
            static const std::string& getName();
            static nint getMajor();
            static nint getMinor();
            static nint getFix();
        };

        enum platformType {
            PLATFORM_TYPE_START = -1,
            WINDOW,
            LINUX,
            MACOS,
            OTHERS,
            PLATFORM_TYPE_END,
        };

        struct _nout platform {
            BY_ME(platform)

        public:
            static platformType get();
            static const std::string& getName();
            static const std::string& getVersion();
        };

        enum buildType {
            BUILD_TYPE_START = -1,
            DEBUG,
            RELEASE,
            BUILD_TYPE_END,
        };

        struct _nout config {
            BY_ME(config)

        public:
            static buildType get();
            static nbool isDbg();
            static const std::string& getName();
        };
    };
} // namespace by
