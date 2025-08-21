/// @file
#pragma once

#include "indep/common.hpp"
#include "indep/macro.hpp"
#ifdef BY_BUILD_PLATFORM_IS_WINDOWS
#    include <direct.h>
#    include <io.h>
#    define getcwd _getcwd
#else
#    include <dirent.h>
#    include <unistd.h>
#endif
#include <vector>

namespace by {

    /// @ingroup indep
    /// @brief Cross-platform filesystem utilities for byeol language system
    /// @details Provides filesystem operations before C++ std::filesystem library
    /// is fully supported across all target platforms. Handles platform-specific
    /// file and directory operations through a unified interface.
    class _nout fsystem {
        BY(ME(fsystem))

        struct entry {
#ifdef BY_BUILD_PLATFORM_IS_WINDOWS
            _finddata_t file;
            intptr_t dir;
#else
            DIR* dir;
#endif
            std::string path;
        };

        typedef std::vector<entry> entries;

        /// @ingroup indep
        /// @brief Iterator for traversing filesystem entries
        /// @details Provides sequential access to filesystem entries with platform-specific
        /// handling for Windows and Unix-like systems.
        class _nout iterator {
            BY(ME(iterator))
            friend class fsystem;

        private:
            iterator(const std::string& path);

        public:
            ~iterator();

        public:
            /// @brief Dereference operator to get current path
            const std::string& operator*();
            /// @brief Post-increment operator to advance iterator
            me& operator++(int);
            /// @brief Conversion to bool for validity checking
            operator nbool() const;

        public:
            /// @brief Release resources held by iterator
            void rel();
            /// @brief Advance to next filesystem entry
            nbool next();
            const std::string& get() const;
            std::string getName() const;
            std::string getDir() const;
            /// @brief Check if iterator has reached end
            nbool isEnd() const;

        private:
            void _addDir(const std::string& dirPath);
            void _popDir();
            std::string _filterPath(const std::string& org);

        private:
            entries _entries;
            std::string _nowPath;
        };

    public:
        /// @brief Create iterator for filesystem traversal
        /// @param path Target directory path to traverse
        static iterator find(const std::string& path);
        /// @brief Get current working directory
        static std::string getCurrentDir();
        /// @brief Get platform-specific path delimiter
        static const std::string& getDelimiter();
    };
} // namespace by
