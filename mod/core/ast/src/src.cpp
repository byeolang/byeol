#include "src.hpp"

namespace nm {
    NAMU(DEF_ME(src))

    me::src(const srcFile& file, const std::string& name, const point& pos):
        _file(file), _name(name), _pos(pos) {}

    const srcFile& me::getFile() const {
        return *_file;
    }

    const point& me::getPos() const {
        return _pos;
    }

    const std::string& me::getName() const {
        return _name;
    }

    void me::_setFile(const srcFile& new1) {
        _file.bind(new1);
    }

    void me::_setPos(const point& new1) {
        _pos = new1;
    }

    void me::_setName(const std::string& new1) {
        _name = new1;
    }
}
