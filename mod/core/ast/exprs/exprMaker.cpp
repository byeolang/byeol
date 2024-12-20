#include "exprMaker.hpp"
#include "ast/exprs/defVarExpr.hpp"
#include "ast/exprs/assignExpr.hpp"
#include "ast/exprs/getExpr.hpp"
#include "ast/params.hpp"
#include "builtin/primitive/nVoid.hpp"
#include "worker/defBlock.hpp"

namespace nm {

    NM(DEF_ME(exprMaker))

    me::exprMaker(): _pos{0, 0} {}

    me::exprMaker(const srcFile& s, ncnt row, ncnt col): _pos{row, col} { setSrcFile(s); }

    me& me::addRow() { return addRow(1); }

    me& me::addRow(ncnt step) { return setRow(_pos.row + step); }

    me& me::addCol() { return addCol(1); }

    me& me::addCol(ncnt step) { return setCol(_pos.col + step); }

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

    const point& me::getPos() const { return _pos; }

    const srcFile& me::getSrcFile() const { return *_file; }

    tstr<src> me::makeSrc(const std::string& name) const { return new src(*_file, name, _pos); }

    func* me::_makeFunc(const std::string& name, const narr& stmts) const {
        if(stmts.isEmpty()) return nullptr;

        func* ret =
            birth<func>(name, *new modifier(), mgdType::make<func>(name, params(), *new nVoid()));
        ret->getBlock().getStmts().add(stmts);
        return ret;
    }

    func* me::makeExpandFunc(const defBlock& blk) const {
        return _makeFunc(baseObj::EXPAND_NAME, blk.getExpands());
    }

    assignExpr* me::makeAssignExprFrom(const defVarExpr& e) const {
        return make<assignExpr>(*make<getExpr>(e.getName()), e.getRight());
    }

    func* me::makeCommonFunc(const defBlock& blk) const {
        return _makeFunc(baseObj::COMMON_NAME, blk.getCommon());
    }
} // namespace nm
