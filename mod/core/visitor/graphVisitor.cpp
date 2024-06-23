#include "graphVisitor.hpp"
#include "../ast.hpp"
#include "../frame/frame.hpp"
#include "../loader/worker/worker.inl"
#include <regex>

namespace namu {

    NAMU(DEF_ME(graphVisitor))
    using platformAPI::foreColor;
    using namespace std;

    void me::_onWork() {
        _parentsLast.push_back(true);
        _isStart = false;
        super::_onWork();

        clog << "\n";
    }

    void me::_drawIndent() {
        for(const char* e : _indents)
            clog << platformAPI::foreColor(LIGHTGRAY) << e;
    }

    void me::_onIndent() {
        nbool isParentLast = _parentsLast.back();
        _indents.push_back((isParentLast ? "   " : "┃  "));
    }

    nbool me::onVisit(visitInfo i, nInt& e) { return _onVisitPrimitive<nInt>(i, e); }
    nbool me::onVisit(visitInfo i, nFlt& e) { return _onVisitPrimitive<nFlt>(i, e); }
    nbool me::onVisit(visitInfo i, nStr& e) {
        _drawFrame(i);
        clog << foreColor(LIGHTRED) << i.name << " "
             << foreColor(CYAN) << e.getType().getName()
             << foreColor(LIGHTGRAY) << " = " << foreColor(YELLOW) << _encodeNewLine(e.get());
        return false;
    }
    nbool me::onVisit(visitInfo i, nChar& e) { return _onVisitPrimitive<nChar>(i, e); }
    nbool me::onVisit(visitInfo i, nByte& e) { return _onVisitPrimitive<nByte>(i, e); }
    nbool me::onVisit(visitInfo i, nBool& e) { return _onVisitPrimitive<nBool>(i, e); }

    namespace {
        std::string _to4DigitAddr(void* inst) {
            stringstream ss;
            ss << inst;
            std::string raw = ss.str();
            return raw.substr(raw.length() - 4);
        }
    }

    nbool me::onVisit(visitInfo i, node& visitee) {
        _drawFrame(i);
        clog << foreColor(LIGHTRED) << i.name << " "
             << foreColor(CYAN) << visitee.getType().getName()
             << foreColor(LIGHTGRAY) << "@" << foreColor(RED) << _to4DigitAddr(&visitee);
        return true;
    }

    void me::_drawFrame(visitInfo i) {
        if(_isStart)
            clog << "\n";
        _isStart = true;

        _onIndent();
        _drawIndent();

        nbool isLast = nul(i.parent) ? true : i.index >= i.len - 1;
        clog << foreColor(LIGHTGRAY) << (isLast ? "┗━[" : "┣━[") << foreColor(YELLOW) << i.index << foreColor(LIGHTGRAY) << "] ";
        _parentsLast.push_back(isLast);
    }

    void me::onLeave(visitInfo i, node& visitee) {
        _indents.pop_back();
        _parentsLast.pop_back();
    }

    nbool me::onVisit(visitInfo i, baseFunc& fun) {
        _drawFrame(i);

        clog << foreColor(LIGHTGRAY) << "@" << foreColor(RED) << _to4DigitAddr(&fun) << " "
             << foreColor(LIGHTBLUE) << i.name
             << foreColor(LIGHTGRAY) << "(" << foreColor(CYAN) << fun.getParams().toStr() << foreColor(LIGHTGRAY) << ") "
             << foreColor(CYAN) << fun.getRet()->getType().getName();
        return true;
    }

    nbool me::onVisit(visitInfo i, genericObj& o) {
        onVisit(i, (node&) o);

        int n = 0;
        ncnt len = o._cache.size();
        for(auto e : o._cache)
            e.second->accept(visitInfo { e.first, &o, n++, len, i.depth+1}, *this);
        return true;
    }

    nbool me::onVisit(visitInfo i, getExpr& e) {
        onVisit(i, (node&) e);

        const node& me = e.getMe();
        string from = nul(me) ? "frame" : _getNameFrom(me);
        string args = e.getSubArgs().toStr();

        clog << foreColor(LIGHTGRAY) << " = "
             << foreColor(MAGENTA) << from << foreColor(LIGHTGRAY) << "." << foreColor(YELLOW) << e.getSubName();
        if(!args.empty())
            clog << foreColor(LIGHTGRAY) << "(" << foreColor(CYAN) << args << foreColor(LIGHTGRAY) << ")";
        return true;
    }

    nbool me::onVisit(visitInfo i, runExpr& e) {
        onVisit(i, (node&) e);

        const node& meExpr = e.getMe();
        string me = nul(meExpr) ? "frame" : _getNameFrom(meExpr);

        clog << foreColor(LIGHTGRAY) << " = "
             << foreColor(MAGENTA) << me << foreColor(LIGHTGRAY) << "."
             << foreColor(YELLOW) << _getNameFrom(e.getSubject())
             << foreColor(LIGHTGRAY) << "(" << foreColor(YELLOW) << e.getArgs().toStr() << foreColor(LIGHTGRAY) << ")";
        return true;
    }

    nbool me::onVisit(visitInfo i, FBOExpr& e) {
        onVisit(i, (node&) e);

        clog << foreColor(LIGHTGRAY) << " = "
             << foreColor(CYAN) << e.getLeft().getType().getName();
        str leftVal = e.getLeft();
        const std::string& leftName = leftVal->isSub<getExpr>() ? leftVal->cast<getExpr>().getSubName() : leftVal->getType().getName();
        clog << foreColor(LIGHTGRAY) << "(" << foreColor(YELLOW) << _encodeNewLine(leftName) << foreColor(LIGHTGRAY) << ")";

        clog << " " << foreColor(LIGHTGRAY) << e.getRuleName(e.getRule()) << " "
             << foreColor(CYAN) << e.getRight().getType().getName();
        str rightVal = e.getRight();
        const std::string& rightName = rightVal->isSub<getExpr>() ? rightVal->cast<getExpr>().getSubName() : rightVal->getType().getName();
        clog << foreColor(LIGHTGRAY) << "(" << foreColor(YELLOW) << _encodeNewLine(rightName) << foreColor(LIGHTGRAY) << ")";

        return !e.getLeft().isSub<arithmeticObj>() || !e.getRight().isSub<arithmeticObj>();
    }

    nbool me::onVisit(visitInfo i, FUOExpr& e) {
        onVisit(i, (node&) e);

        const node& op = e.getOperand();
        clog << foreColor(LIGHTGRAY) << " = "
             << foreColor(CYAN) << op.getType().getName() << " "
             << foreColor(LIGHTGRAY) << e.getRuleName(e.getRule());
        return true;
    }

    nbool me::onVisit(visitInfo i, assignExpr& e) {
        onVisit(i, (node&) e);

        clog << foreColor(LIGHTGRAY) << " = "
             << foreColor(MAGENTA) << _getNameFrom(e.getLeft()) << foreColor(LIGHTGRAY) << " = "
             << foreColor(MAGENTA) << _getNameFrom(e.getRight());

        return true;
    }

    nbool me::onVisit(visitInfo i, defAssignExpr& e) {
        onVisit(i, (node&) e);

        clog << foreColor(LIGHTGRAY) << " = "
             << foreColor(MAGENTA) << e.getSubName() << foreColor(LIGHTGRAY) << " := "
             << foreColor(CYAN) << _getNameFrom(e.getRight());
        return true;
    }

    string me::_getNameFrom(const node& it) const {
        string ret = it.getType().getName();
        const getExpr& cast = it.cast<getExpr>();
        if(!nul(cast))
            ret = cast.getSubName();
        return ret;
    }

    string me::_encodeNewLine(const string& msg) const {
        return "\"" + regex_replace(msg, regex("\n"), "\\n") + "\"";
    }
}
