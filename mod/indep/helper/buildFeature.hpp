#pragma once

#include "../macro.hpp"
#include "../common.hpp"

namespace nm {
    using std::string;

    class _nout buildFeature {
        NM_ME(buildFeature)

    public:
        struct _nout date {
            NM_ME(date)

        public:
            static NM_SINGLETON_GETTER(const string& get(), string, NM_BUILD_DATE)
            static nint getYear();
            static nint getMonth();
            static nint getDay();
        };

        struct _nout time {
            NM_ME(time)

        public:
            static NM_SINGLETON_GETTER(const string& get(), string, NM_BUILD_TIME)
            static nint getHour();
            static nint getMin();
            static nint getSec();
        };

        struct _nout version {
            NM_ME(version)

        public:
            static NM_SINGLETON_GETTER(const string& get(), string, NM_BUILD_VERSION)
            static NM_SINGLETON_GETTER(const string& getValue(), string, NM_BUILD_VERSION_VALUE)
            static NM_SINGLETON_GETTER(const string& getName(), string, NM_BUILD_VERSION_NAME)
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
            NM_ME(platform)

        public:
            static platformType get();
            static NM_SINGLETON_GETTER(const string& getName(), string, NM_BUILD_PLATFORM_NAME)
            static NM_SINGLETON_GETTER(const string& getVersion(), string, NM_BUILD_PLATFORM_VERSION)
        };

        enum buildType {
            BUILD_TYPE_START = -1,
            DEBUG,
            RELEASE,
            BUILD_TYPE_END,
        };

        struct _nout config {
            NM_ME(config)

        public:
            static buildType get();
            static nbool isDbg();
            static NM_SINGLETON_GETTER(const string& getName(), string, NM_BUILD_TYPENAME)
        };
    };
}
