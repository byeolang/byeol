#include "core/ast/src/srcFile.hpp"

#include "core/ast/dumScope.hpp"
#include "core/worker/visitor/visitor.hpp"

namespace nm {

    NM(DEF_ME(srcFile), DEF_VISIT())

    me::srcFile(): me("", "") {}

    me::srcFile(const std::string& fileName, const std::string& contents):
        _fileName(fileName), _contents(contents) {}

    priorType me::prioritize(const args& a) const { return NO_MATCH; }

    str me::run(const args& a) { return str(); }

    scope& me::subs() {
        return dumScope::singleton();
    }

    const std::string& me::getFileName() const { return _fileName; }

    const std::string& me::getContents() const { return _contents; }
}
