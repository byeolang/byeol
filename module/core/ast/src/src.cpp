#include "core/ast/src/src.hpp"

namespace by {
    BY(DEF_ME(src))

    me::src(const srcFile& file, const std::string& name, const point& pos): _file(file), _name(name), _pos(pos) {}

    const srcFile* me::getFile() const { return _file.get(); }

    const point& me::getPos() const { return _pos; }

    const std::string& me::getName() const { return _name; }

    void me::setFile(const srcFile& new1) { _file.bind(new1); }

    void me::setPos(const point& new1) { _pos = new1; }

    void me::setName(const std::string& new1) { _name = new1; }
}
