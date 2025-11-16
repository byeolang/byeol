/// @file
#pragma once

#include "indep/common.hpp"
#include "indep/macro.hpp"

namespace by {
    /// @ingroup indep
    /// @brief Provides build-time information and feature detection
    /// @details Contains nested structs for accessing build date, time, version,
    /// platform information, and configuration details at runtime.
    class _nout buildFeature {
        BY_ME(buildFeature)

    public:
        /// @ingroup indep
        /// @brief Build date information
        /// @details Provides access to the date when the application was built.
        struct _nout date {
            BY_ME(date)

        public:
            static const std::string& get();
            static constexpr nint getYear() { return BY_BUILD_DATE_YEAR; }
            static constexpr nint getMonth() { return BY_BUILD_DATE_MONTH; }
            static constexpr nint getDay() { return BY_BUILD_DATE_DAY; }
        };

        /// @ingroup indep
        /// @brief Build time information
        /// @details Provides access to the time when the application was built.
        struct _nout time {
            BY_ME(time)

        public:
            static const std::string& get();
            static constexpr nint getHour() { return BY_BUILD_TIME_HOUR; }
            static constexpr nint getMin() { return BY_BUILD_TIME_MINUTE; }
            static constexpr nint getSec() { return BY_BUILD_TIME_SECOND; }
        };

        /// @ingroup indep
        /// @brief Version information
        /// @details Provides access to version numbers and names.
        struct _nout version {
            BY_ME(version)

        public:
            static const std::string& get();
            static const std::string& getValue();
            static const std::string& getName();
            static constexpr nint getMajor() { return BY_BUILD_VERSION_MAJOR; }
            static constexpr nint getMinor() { return BY_BUILD_VERSION_MINOR; }
            static constexpr nint getFix() { return BY_BUILD_VERSION_FIX; }
        };

        enum platformType {
            PLATFORM_TYPE_START = -1,
            WINDOW,
            LINUX,
            MACOS,
            OTHERS,
            PLATFORM_TYPE_END,
        };

        /// @ingroup indep
        /// @brief Platform information
        /// @details Provides platform type, name, and version detection.
        struct _nout platform {
            BY_ME(platform)

        public:
            static platformType get();
            static const std::string& getName();
            static const std::string& getVersion();
            static constexpr ncnt getArchBits() { return sizeof(void*); }
        };

        enum buildType {
            BUILD_TYPE_START = -1,
            DEBUG,
            RELEASE,
            BUILD_TYPE_END,
        };

        /// @ingroup indep
        /// @brief Build configuration information
        /// @details Provides information about debug/release build type.
        struct _nout config {
            BY_ME(config)

        public:
            static buildType get();
            constexpr static nbool isDbg() {
#if BY_IS_DBG
                return true;
#else
                return false;
#endif
            }
            static const std::string& getName();
        };
    };
} // namespace by
