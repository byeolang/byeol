#include "manifest.hpp"

namespace nm {

    NM_DEF_ME(manifest)

    entrypoint::entrypoint(const std::string& newLang, const std::vector<std::string> newPaths):
            lang(newLang), paths(newPaths) {}
    entrypoint::~entrypoint() {}

    nbool entrypoint::isValid() const {
        return lang != "" && paths.size() > 0;
    }

    me::manifest(): name(DEFAULT_NAME), filePath(""), author("unknown"), ver("") {}
    me::manifest(const std::string& newName): name(newName), filePath(""), author("unknown"), ver("") {}
    me::manifest(const std::string& newName, const std::string& newFilePath, const std::string& newAuthor,
            const std::string& newVer, const entrypoints& newPoints):
            name(newName), filePath(newFilePath), author(newAuthor), ver(newVer), points(newPoints) {}

    me::~manifest() {}

    nbool me::isValid() const {
         if(name == "" || filePath == "" || filePath == "" || author == "" || ver == "")
             return false;

         for(entrypoint point : points)
            if(!point.isValid()) return false;

        return true;
    }
}
