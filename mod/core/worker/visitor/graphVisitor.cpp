#include "graphVisitor.hpp"

#include <regex>

#include "../../ast.hpp"
#include "../../frame/frame.hpp"
#include "../worker.inl"

namespace nm {

    NM(DEF_ME(graphVisitor))
    using platformAPI::foreColor;
    using namespace std;

    void me::_onWork() {
        _parentsLast.push_back(true);
        _isStart = false;
        super::_onWork();

        cout << "\n";
    }

    void me::_drawIndent() {
        for(const char* e: _indents)
            cout << platformAPI::foreColor(LIGHTGRAY) << e;
    }

    void me::_onIndent() {
        nbool isParentLast = _parentsLast.back();
        _indents.push_back((isParentLast ? "   " : "┃  "));
    }

    nbool me::onVisit(const visitInfo& i, nInt& e) { return _onVisitPrimitive<nInt>(i, e); }

    nbool me::onVisit(const visitInfo& i, nFlt& e) { return _onVisitPrimitive<nFlt>(i, e); }

    nbool me::onVisit(const visitInfo& i, nStr& e) {
        _drawFrame(i);
        cout << foreColor(LIGHTRED) << i.name << " " << foreColor(CYAN)
             << e.getType().createNameWithParams() << foreColor(LIGHTGRAY) << " = "
             << foreColor(YELLOW) << _encodeNewLine(e.get());
        return false;
    }

    nbool me::onVisit(const visitInfo& i, nChar& e) { return _onVisitPrimitive<nChar>(i, e); }

    nbool me::onVisit(const visitInfo& i, nByte& e) { return _onVisitPrimitive<nByte>(i, e); }

    nbool me::onVisit(const visitInfo& i, nBool& e) { return _onVisitPrimitive<nBool>(i, e); }

    namespace {
        void _showModifier(const modifier& mod) {
            if(!mod.isPublic()) cout << foreColor(GREEN) << "_";
            if(mod.isExplicitOverride()) cout << foreColor(GREEN) << "+";
        }
    }

    nbool me::onVisit(const visitInfo& i, node& visitee) {
        _drawFrame(i);
        _showModifier(visitee.getModifier());

        cout << foreColor(LIGHTRED) << i.name << " " << foreColor(CYAN)
             << visitee.getType().createNameWithParams() << foreColor(LIGHTGRAY) << "@"
             << foreColor(RED) << platformAPI::toAddrId(&visitee);
        return true;
    }

    void me::_drawFrame(const visitInfo& i) {
        if(_isStart) cout << "\n";
        _isStart = true;

        _onIndent();
        _drawIndent();

        nbool isLast = nul(i.parent) ? true : i.index >= i.len - 1;
        cout << foreColor(LIGHTGRAY) << (isLast ? "┗━[" : "┣━[") << foreColor(YELLOW) << i.index
             << foreColor(LIGHTGRAY) << "] ";
        _parentsLast.push_back(isLast);
    }

    void me::onLeave(const visitInfo& i, node& visitee) {
        _indents.pop_back();
        _parentsLast.pop_back();
    }

    nbool me::onVisit(const visitInfo& i, baseFunc& fun) {
        _drawFrame(i);

        cout << foreColor(LIGHTGRAY) << "@" << foreColor(RED) << platformAPI::toAddrId(&fun) << " ";

        _showModifier(fun.getModifier());

        const node& ret = fun.getRet();
        cout << foreColor(LIGHTBLUE) << i.name << foreColor(LIGHTGRAY) << "(" << foreColor(CYAN)
             << fun.getParams().toStr() << foreColor(LIGHTGRAY) << ") " << foreColor(CYAN)
             << (!nul(ret) ? ret.getType().createNameWithParams() : "null");
        return true;
    }

    nbool me::onVisit(const visitInfo& i, genericOrigin& o) {
        onVisit(i, (node&) o);

        int n = 0;
        ncnt len = o._cache.size();
        for(auto e: o._cache)
            e.second->accept(visitInfo{e.first, &o, n++, len, i.depth + 1}, *this);
        return true;
    }

    nbool me::onVisit(const visitInfo& i, getExpr& e) {
        onVisit(i, (node&) e);

        string args = e.getArgs().toStr();

        cout << foreColor(LIGHTGRAY) << " = " << foreColor(MAGENTA) << _getNameFrom(e.getMe())
             << foreColor(LIGHTGRAY) << "." << foreColor(YELLOW) << e.getName();
        if(!args.empty())
            cout << foreColor(LIGHTGRAY) << "(" << foreColor(CYAN) << args << foreColor(LIGHTGRAY)
                 << ")";
        return true;
    }

    nbool me::onVisit(const visitInfo& i, runExpr& e) {
        onVisit(i, (node&) e);

        cout << foreColor(LIGHTGRAY) << " = " << foreColor(MAGENTA) << _getNameFrom(e.getMe())
             << foreColor(LIGHTGRAY) << "." << foreColor(YELLOW) << _getNameFrom(e.getSubj())
             << foreColor(LIGHTGRAY) << "(" << foreColor(YELLOW) << e.getArgs().toStr()
             << foreColor(LIGHTGRAY) << ")";
        return true;
    }

    nbool me::onVisit(const visitInfo& i, FBOExpr& e) {
        onVisit(i, (node&) e);

        cout << foreColor(LIGHTGRAY) << " = " << foreColor(CYAN)
             << e.getLeft().getType().createNameWithParams();
        str leftVal = e.getLeft();
        const std::string& leftName = leftVal->isSub<getExpr>() ?
            leftVal->cast<getExpr>().getName() :
            leftVal->getType().createNameWithParams();
        cout << foreColor(LIGHTGRAY) << "(" << foreColor(YELLOW) << _encodeNewLine(leftName)
             << foreColor(LIGHTGRAY) << ")";

        cout << " " << foreColor(LIGHTGRAY) << e.getRuleName(e.getRule()) << " " << foreColor(CYAN)
             << e.getRight().getType().createNameWithParams();
        str rightVal = e.getRight();
        const std::string& rightName = rightVal->isSub<getExpr>() ?
            rightVal->cast<getExpr>().getName() :
            rightVal->getType().createNameWithParams();
        cout << foreColor(LIGHTGRAY) << "(" << foreColor(YELLOW) << _encodeNewLine(rightName)
             << foreColor(LIGHTGRAY) << ")";

        return !e.getLeft().isSub<arithmeticObj>() || !e.getRight().isSub<arithmeticObj>();
    }

    nbool me::onVisit(const visitInfo& i, FUOExpr& e) {
        onVisit(i, (node&) e);

        const node& op = e.getOperand();
        cout << foreColor(LIGHTGRAY) << " = " << foreColor(CYAN)
             << op.getType().createNameWithParams() << " " << foreColor(LIGHTGRAY)
             << e.getRuleName(e.getRule());
        return true;
    }

    nbool me::onVisit(const visitInfo& i, assignExpr& e) {
        onVisit(i, (node&) e);

        cout << foreColor(LIGHTGRAY) << " = " << foreColor(MAGENTA) << _getNameFrom(e.getLeft())
             << foreColor(LIGHTGRAY) << " = " << foreColor(MAGENTA) << _getNameFrom(e.getRight());

        return true;
    }

    nbool me::onVisit(const visitInfo& i, defVarExpr& e) {
        onVisit(i, (node&) e);

        cout << foreColor(LIGHTGRAY) << " = " << foreColor(MAGENTA) << e.getName()
             << foreColor(LIGHTGRAY) << " := " << foreColor(CYAN) << _getNameFrom(e.getRight());
        return true;
    }

    string me::_getNameFrom(const node& it) const {
        if(nul(it)) return "frame";

        string ret = it.getType().createNameWithParams();
        const auto& name = it.cast<getExpr>() THEN(getName());
        if(!nul(name)) ret = name;
        return ret;
    }

    string me::_encodeNewLine(const string& msg) const {
        return "\"" + regex_replace(msg, regex("\n"), "\\n") + "\"";
    }
} // namespace nm
