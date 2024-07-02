#include "srcFile.hpp"
#include "visitor/visitor.hpp"
#include "../dumScope.hpp"

namespace nm {

    NM(DEF_ME(srcFile), DEF_VISIT())

    me::srcFile(const std::string& fileName, const std::string& contents):
        _fileName(fileName), _contents(contents) {}

    priorType me::prioritize(const args& a) const {
        return NO_MATCH;
    }

    str me::run(const args& a) {
        return str();
    }

    scope& me::subs() {
        static dumScope inner;
        return inner;
    }

    const std::string& me::getFileName() const {
        return _fileName;
    }

    const std::string& me::getContents() const {
        return _contents;
    }
}
