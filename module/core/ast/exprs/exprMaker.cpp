#include "core/ast/exprs/exprMaker.hpp"
#include "core/ast/exprs/defVarExpr.hpp"
#include "core/ast/exprs/assignExpr.hpp"
#include "core/ast/exprs/getExpr.hpp"
#include "core/ast/params.hpp"
#include "core/builtin/primitive/nVoid.hpp"
#include "core/worker/defBlock.hpp"
#include "core/type/typeMaker.hpp"

namespace by {

    BY(DEF_ME(exprMaker))

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
        WHEN(stmts.isEmpty()).ret(nullptr);

        func* ret =
            birth<func>(name, *new modifier(), typeMaker::make<func>(name, params(), new nVoid()));
        ret->getBlock().getStmts().add(stmts);
        return ret;
    }

    func* me::makeExpandFunc(const defBlock& blk) const {
        return _makeFunc(baseObj::EXPAND_NAME, blk.getExpands());
    }

    assignExpr* me::makeAssignExprFrom(const defVarExpr& e) const {
        const auto& right = e.getRight() OR.ret(nullptr);
        return make<assignExpr>(*make<getExpr>(e.getName()), right);
    }

    func* me::makeCommonFunc(const defBlock& blk) const {
        return _makeFunc(baseObj::COMMON_NAME, blk.getCommon());
    }
} // namespace by
