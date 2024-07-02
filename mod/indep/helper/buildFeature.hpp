#pragma once

#include "../macro.hpp"
#include "../common.hpp"

namespace nm {
    using std::string;

    class _nout buildFeature {
        NAMU_ME(buildFeature)

    public:
        struct _nout date {
            NAMU_ME(date)

        public:
            static NAMU_SINGLETON_GETTER(const string& get(), string, NAMU_BUILD_DATE)
            static nint getYear();
            static nint getMonth();
            static nint getDay();
        };

        struct _nout time {
            NAMU_ME(time)

        public:
            static NAMU_SINGLETON_GETTER(const string& get(), string, NAMU_BUILD_TIME)
            static nint getHour();
            static nint getMin();
            static nint getSec();
        };

        struct _nout version {
            NAMU_ME(version)

        public:
            static NAMU_SINGLETON_GETTER(const string& get(), string, NAMU_BUILD_VERSION)
            static NAMU_SINGLETON_GETTER(const string& getValue(), string, NAMU_BUILD_VERSION_VALUE)
            static NAMU_SINGLETON_GETTER(const string& getName(), string, NAMU_BUILD_VERSION_NAME)
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
            NAMU_ME(platform)

        public:
            static platformType get();
            static NAMU_SINGLETON_GETTER(const string& getName(), string, NAMU_BUILD_PLATFORM_NAME)
            static NAMU_SINGLETON_GETTER(const string& getVersion(), string, NAMU_BUILD_PLATFORM_VERSION)
        };

        enum buildType {
            BUILD_TYPE_START = -1,
            DEBUG,
            RELEASE,
            BUILD_TYPE_END,
        };

        struct _nout config {
            NAMU_ME(config)

        public:
            static buildType get();
            static nbool isDbg();
            static NAMU_SINGLETON_GETTER(const string& getName(), string, NAMU_BUILD_TYPENAME)
        };
    };
}
