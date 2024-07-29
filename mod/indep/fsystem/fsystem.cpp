#include "fsystem.hpp"

namespace nm {

    NM_DEF_ME(fsystem)

    const std::string& me::iterator::get() const {
        return _nowPath;
    }

    const std::string& me::getDelimiter() {    
#if NM_BUILD_PLATFORM == NM_TYPE_WINDOWS
        static std::string inner = "\\";
#else
        static std::string inner = "/";
#endif
        return inner;
    }

    nbool me::iterator::next() {
        // assume that all of data to _entries was valid:
        //  which means, if returning value from readdir() was null,
        //  then the most top element of entries should be removed.
        while(!isEnd()) {
            entry& e = _entries[_entries.size()-1];
#ifdef NM_BUILD_PLATFORM_IS_WINDOWS
            int res = _findnext(e.dir, &e.file);
            if(res == -1) {
#else
            struct dirent* file = readdir(e.dir);
            if(!file) {
#endif
                _popDir();
                continue;
            }
#ifdef NM_BUILD_PLATFORM_IS_WINDOWS
            std::string name = e.file.name;
#else
            std::string name = file->d_name;
#endif
            if(name == ".." || name == ".")
                continue;
            std::string path = e.path + getDelimiter() + name;
#ifdef NM_BUILD_PLATFORM_IS_WINDOWS
            if(e.file.attrib & _A_SUBDIR) {
#else
            if(file->d_type == DT_DIR) {
#endif
                _addDir(path);
                continue;
            }

            _nowPath = path;
            return true;
        }

        _nowPath = "";
        return false;
    }

    me::iterator::iterator(const std::string& path) {
        _addDir(path);
    }

    me::iterator::~iterator() {
        rel();
    }

    const std::string& me::iterator::operator*() {
        return get();
    }

    me::iterator& me::iterator::operator++(int) {
        next();
        return *this;
    }

    me::iterator::operator nbool() const {
        return !isEnd();
    }

    void me::iterator::rel() {
        while(!isEnd())
            _popDir();
    }

    std::string me::iterator::getName() const {
        const std::string& path = get();
#if NM_BUILD_PLATFORM == NM_TYPE_WINDOWS
        auto slash = path.rfind('\\');
#else
        auto slash = path.rfind('/');
#endif
        return path.substr(slash + 1);
    }

    std::string me::iterator::getDir() const {
        const std::string& path = get();
#if NM_BUILD_PLATFORM == NM_TYPE_WINDOWS
        auto slash = path.rfind('\\');
#else
        auto slash = path.rfind('/');
#endif
        return path.substr(0, slash);
    }

    nbool me::iterator::isEnd() const {
        return _entries.size() == 0;
    }

    void me::iterator::_addDir(const std::string& dirPath) {
        if(dirPath.empty()) return;
        std::string path = _filterPath(dirPath);
#ifdef NM_BUILD_PLATFORM_IS_WINDOWS
        _finddata_t file;
        intptr_t newDir = _findfirst((path + "\\*.*").c_str(), &file);
        if (newDir == -1) return;
        _entries.push_back(entry {file, newDir, path});
#else
        DIR* newDir = opendir(path.c_str());
        if(!newDir) return;
        _entries.push_back(entry {newDir, path});
#endif
    }

    void me::iterator::_popDir() {
        entry& e = _entries[_entries.size()-1];
#ifdef NM_BUILD_PLATFORM_IS_WINDOWS
        _findclose(e.dir);
#else
        closedir(e.dir);
#endif
        _entries.pop_back();
    }

    std::string me::iterator::_filterPath(const std::string& org) {
        auto idx = org.length() - 1;
        char last = org[idx];
#ifdef NM_BUILD_PLATFORM_IS_WINDOWS
        if (last == '\\' || last == '/')
#else
        if (last == '/')
#endif
            return org.substr(0, idx);

        return org;
    }

    me::iterator me::find(const std::string& path) {
        return iterator(path);
    }

    std::string me::getCurrentDir() {
        constexpr ncnt BUF_LEN = 256;
        char buf[BUF_LEN] = {};
        return getcwd(buf, BUF_LEN);
    }
}
