#pragma once

#include "../common.hpp"
#include "../macro.hpp"
#ifdef NM_BUILD_PLATFORM_IS_WINDOWS
#    include <direct.h>
#    include <io.h>
#    define getcwd _getcwd
#else
#    include <dirent.h>
#    include <unistd.h>
#endif
#include <vector>

namespace nm {

    /// fsystem is going to be used before filesystem library fully supported.
    /// to apply fsystem on clang++ is quite hard now.
    class _nout fsystem {
        NM(ME(fsystem))

        struct entry {
#ifdef NM_BUILD_PLATFORM_IS_WINDOWS
            _finddata_t file;
            intptr_t dir;
#else
            DIR* dir;
#endif
            std::string path;
        };

        typedef std::vector<entry> entries;

        class _nout iterator {
            NM(ME(iterator))
            friend class fsystem;

        private:
            iterator(const std::string& path);

        public:
            ~iterator();

        public:
            const std::string& operator*();
            me& operator++(int);
            operator nbool() const;

        public:
            void rel();
            nbool next();
            const std::string& get() const;
            std::string getName() const;
            std::string getDir() const;
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
        static iterator find(const std::string& path);
        static std::string getCurrentDir();
        static const std::string& getDelimiter();
    };
} // namespace nm
