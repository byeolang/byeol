/// @file
#pragma once

#include <regex>
#include "indep/common.hpp"
#include "indep/macro.hpp"
#include "indep/helper/tmay.inl"
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

    /** @ingroup indep
     *  @brief Cross-platform filesystem utilities for recursive file traversal
     *  @details A simple class for recursively traversing files in a specified folder.
     *  Provides platform-independent API that works on both Windows and POSIX-compliant
     *  operating systems. Core functionality is provided through the iterator class.
     *
     *  @section usage Usage
     *  Basic usage example:
     *  @code
     *      auto e = fsystem::find("../your/path");
     *      while(e.next()) { // Returns false when all files are traversed
     *          const std::string& path = *e; // Path of the found file
     *          if(*e == "../your/path/child/helloWorld.cpp") // Always uses relative paths
     *              doSomething(e->getDir()); // Returns the folder path of the found file
     *      }
     *  @endcode
     *
     *  @remark Always iterates files only
     *  Empty folders are skipped during iteration.
     *
     *
     *  @section this supports partial glob patterns
     *  You can use aterisk (*) and question mark (?) wildcards in the pattern.
     *  @code
     *      auto e = fsystem::find("*.cpp"); // the only difference is here
     *      while(e.next()) {
     *          const std::string& path = *e;
     *          if(*e == "your/path/child/helloWorld.cpp")
     *              doSomething(e->getDir());
     *      }
     *  @endcode
     */
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

        /** @ingroup indep
         *  @brief Iterator for traversing filesystem entries
         *  @details Provides sequential access to filesystem entries with platform-specific
         *  handling for Windows and Unix-like systems.
         */
        class _nout iterator {
            BY(ME(iterator))

        public:
            /**
             * @param path this can be a glob pattern (supports * and ? wildcards)
             */
            iterator(const std::string& path);
            ~iterator();

        public:
            const std::string& operator*();
            me& operator++(int);
            operator nbool() const;
            const std::string* operator->() const;

        public:
            void rel();

            /**
             * @brief Advances iterator to next file entry
             * @return true if next file found, false if no more files exist.
             * @note Automatically skips directories and recursively navigates subdirectories
             */
            nbool next();

            const std::string& get() const;
            std::string getName() const;
            std::string getDir() const;
            nbool isEnd() const;

        private:
            void _addDir(const std::string& dirPath);
            void _popDir();
            std::string _filterPath(const std::string& org);
            nbool _isGlobPattern(const std::string& path);
            std::regex _convertToRegex(const std::string& globPattern);

        private:
            entries _entries;
            std::string _nowPath;
            tmay<std::regex> _pattern;
        };

    public:
        /**
         * @brief Creates iterator for recursive filesystem traversal
         * @return iterator positioned at first file, or at end if path invalid
         * @note Automatically traverses subdirectories recursively, yielding only files
         */
        static iterator find(const std::string& path);

        static std::string getCurrentDir();
        static const std::string& getDelimiter();
    };
} // namespace by
