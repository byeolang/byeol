#include "exprMaker.hpp"

namespace nm {

    NM(DEF_ME(exprMaker))

    me::exprMaker(): _pos {0, 0} {}

    me::exprMaker(const srcFile& s, ncnt row, ncnt col): _pos {row, col} {
        setSrcFile(s);
    }

    me& me::addRow() {
        return addRow(1);
    }

    me& me::addRow(ncnt step) {
        return setRow(_pos.row + step);
    }

    me& me::addCol() {
        return addCol(1);
    }

    me& me::addCol(ncnt step) {
        return setCol(_pos.col + step);
    }

    me& me::setRow(ncnt row) {
        _pos.row = row;
        return *this;
    }

    me& me::setCol(ncnt col) {
        _pos.col = col;
        return *this;
    }

    me& me::setSrcFile(const srcFile& s) {
        _file.bind(s);
        return *this;
    }

    void me::rel() {
        _file.rel();
        _pos.rel();
    }

    const point& me::getPos() const {
        return _pos;
    }

    const srcFile& me::getSrcFile() const {
        return *_file;
    }
}
