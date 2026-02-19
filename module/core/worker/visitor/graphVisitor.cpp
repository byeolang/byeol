#include "core/worker/visitor/graphVisitor.hpp"

#include <regex>

#include "core/ast.hpp"
#include "core/frame/frame.hpp"
#include "core/worker/tworker.inl"
#include "core/internal/coreInternal.hpp"

namespace by {

#define _VISIT \
    WHEN(alreadyVisited) .ret(false)
#define _LEAVE \
    WHEN(alreadyVisited) .ret()

    BY(DEF_ME(graphVisitor))
    using platformAPI::foreColor;
    using namespace std;

    me::graphVisitor(): super(false) {}

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

    nbool me::onVisit(const visitInfo& i, nInt& e, nbool alreadyVisited) { return _onVisitPrimitive<nInt>(i, e); }

    nbool me::onVisit(const visitInfo& i, nFlt& e, nbool alreadyVisited) { return _onVisitPrimitive<nFlt>(i, e); }

    nbool me::onVisit(const visitInfo& i, nStr& e, nbool alreadyVisited) {
        _drawFrame(i);
        _showModifier(e.getModifier());
        cout << foreColor(LIGHTRED) << i.name << " " << foreColor(CYAN) << e.getType().createNameWithParams()
             << foreColor(LIGHTGRAY) << " = " << foreColor(YELLOW) << _encodeNewLine(e.get());
        return false;
    }

    nbool me::onVisit(const visitInfo& i, nByte& e, nbool alreadyVisited) { return _onVisitPrimitive<nByte>(i, e); }

    nbool me::onVisit(const visitInfo& i, nBool& e, nbool alreadyVisited) { return _onVisitPrimitive<nBool>(i, e); }

    me& me::setShowData(nbool showData) {
        _isShowData = showData;
        return *this;
    }

    nbool me::isShowData() const { return _isShowData; }

    nbool me::onVisit(const visitInfo& i, node& visitee, nbool alreadyVisited) {
        _drawFrame(i);
        _showModifier(visitee.getModifier());

        cout << foreColor(LIGHTRED) << i.name << " " << foreColor(CYAN);
        if(_isShowData) cout << visitee.getType().createNameWithParams();
        else cout << visitee.getType().getName();
        cout << foreColor(LIGHTGRAY) << "@" << foreColor(BROWN) << platformAPI::toAddrId(&visitee);
        return true;
    }

    nbool me::onVisit(const visitInfo& i, obj& o, nbool alreadyVisited) {
        onVisit(i, (obj::super&) o, alreadyVisited);

        if(o.isComplete()) cout << foreColor(GREEN) << " [complete] ";
        return !alreadyVisited; // don't traverse sub nodes again.
    }

    void me::_drawFrame(const visitInfo& i) {
        if(_isStart) cout << "\n";
        _isStart = true;

        _onIndent();
        _drawIndent();

        nbool isLast = i.parent ? i.index >= i.len - 1 : true;
        cout << foreColor(LIGHTGRAY) << (isLast ? "┗━[" : "┣━[") << foreColor(YELLOW) << i.index << foreColor(LIGHTGRAY)
             << "] ";
        _parentsLast.push_back(isLast);
    }

    void me::onLeave(const visitInfo& i, node& visitee, nbool alreadyVisited) {
        _indents.pop_back();
        _parentsLast.pop_back();
    }

    nbool me::onVisit(const visitInfo& i, baseFunc& fun, nbool alreadyVisited) {
        _drawFrame(i);

        cout << foreColor(LIGHTGRAY) << "@" << foreColor(RED) << platformAPI::toAddrId(&fun) << " ";

        _showModifier(fun.getModifier());

        const node* ret = fun.getRet();
        cout << foreColor(LIGHTBLUE) << i.name;
        if(_isShowData)
            cout << foreColor(LIGHTGRAY) << "(" << foreColor(CYAN) << fun.getParams().toStr() << foreColor(LIGHTGRAY)
                 << ") " << foreColor(CYAN) << (ret ? ret->getType().createNameWithParams() : "null");
        else cout << foreColor(LIGHTGRAY) << "(?) ?";
        return !alreadyVisited; // don't traverse subs again.
    }

    nbool me::onVisit(const visitInfo& i, func& fun, nbool alreadyVisited) {
        nbool ret = onVisit(i, (func::super&) fun, alreadyVisited);
        cout << foreColor(YELLOW) << (fun.isAbstract() ? "[abstract]" : "");
        return ret;
    }

    nbool me::onVisit(const visitInfo& i, defNestedFuncExpr& e, nbool alreadyVisited) {
        onVisit(i, (defNestedFuncExpr::super&) e, alreadyVisited);

        const func& fun = e.getOrigin();
        std::string params = fun.getParams().toStr();
        const std::string& name = fun.getSrc().getName();
        cout << " -> " << foreColor(LIGHTGRAY) << "@" << foreColor(RED) << platformAPI::toAddrId(&fun) << " "
             << foreColor(LIGHTBLUE) << (name);
        if(_isShowData) cout << foreColor(LIGHTGRAY) << "(" << foreColor(CYAN) << params << foreColor(LIGHTGRAY) << ")";
        else cout << foreColor(LIGHTGRAY) << "(?)";
        return true;
    }

    nbool me::onVisit(const visitInfo& i, genericOrigin& o, nbool alreadyVisited) {
        onVisit(i, (node&) o, alreadyVisited);

        int n = 0;
        auto& cache = coreInternal::getCache(o);
        ncnt len = cache.size();
        for(auto e: cache)
            e.second->accept(visitInfo{e.first, &o, n++, len, i.depth + 1}, *this);
        return true;
    }

    nbool me::onVisit(const visitInfo& i, getExpr& e, nbool alreadyVisited) {
        onVisit(i, (node&) e, alreadyVisited);


        cout << foreColor(LIGHTGRAY) << " = " << foreColor(MAGENTA);
        if(_isShowData) {
            cout << _getNameFrom(e.getMe()) << foreColor(LIGHTGRAY) << "." << foreColor(YELLOW) << e.getName();
            string args = e.getArgs().toStr();
            if(!args.empty())
                cout << foreColor(LIGHTGRAY) << "(" << foreColor(CYAN) << args << foreColor(LIGHTGRAY) << ")";
        } else
            cout << "?" << foreColor(LIGHTGRAY) << "." << foreColor(YELLOW) << e.getName() << foreColor(LIGHTGRAY)
                 << "(?)";

        return true;
    }

    nbool me::onVisit(const visitInfo& i, evalExpr& e, nbool alreadyVisited) {
        onVisit(i, (node&) e, alreadyVisited);

        if(_isShowData)
            cout << foreColor(LIGHTGRAY) << " = " << foreColor(MAGENTA) << _getNameFrom(e.getMe())
                 << foreColor(LIGHTGRAY) << "." << foreColor(YELLOW) << _getNameFrom(e.getSubj())
                 << foreColor(LIGHTGRAY) << "(" << foreColor(YELLOW) << e.getArgs().toStr() << foreColor(LIGHTGRAY)
                 << ")";
        return true;
    }

    nbool me::onVisit(const visitInfo& i, FBOExpr& e, nbool alreadyVisited) {
        onVisit(i, (node&) e, alreadyVisited);

        if(_isShowData) {
            cout << foreColor(LIGHTGRAY) << " = " << foreColor(CYAN) << e.getLeft().getType().createNameWithParams();
            str leftVal = e.getLeft();
            const std::string& leftName = leftVal->isSub<getExpr>() ? leftVal->cast<getExpr>()->getName() :
                                                                      leftVal->getType().createNameWithParams();
            cout << foreColor(LIGHTGRAY) << "(" << foreColor(YELLOW) << _encodeNewLine(leftName) << foreColor(LIGHTGRAY)
                 << ")";

            cout << " " << foreColor(LIGHTGRAY) << e.getSymbolName(e.getSymbol()) << " " << foreColor(CYAN)
                 << e.getRight().getType().createNameWithParams();
            str rightVal = e.getRight();
            const std::string& rightName = rightVal->isSub<getExpr>() ? rightVal->cast<getExpr>()->getName() :
                                                                        rightVal->getType().createNameWithParams();
            cout << foreColor(LIGHTGRAY) << "(" << foreColor(YELLOW) << _encodeNewLine(rightName)
                 << foreColor(LIGHTGRAY) << ")";
        }

        return !e.getLeft().isSub<scalar>() || !e.getRight().isSub<scalar>();
    }

    nbool me::onVisit(const visitInfo& i, FUOExpr& e, nbool alreadyVisited) {
        onVisit(i, (node&) e, alreadyVisited);
        WHEN(!_isShowData) .ret(true);

        const node& op = e.getOperand();
        cout << foreColor(LIGHTGRAY) << " = " << foreColor(CYAN) << op.getType().createNameWithParams() << " "
             << foreColor(LIGHTGRAY) << e.getSymbolName(e.getSymbol());
        return true;
    }

    nbool me::onVisit(const visitInfo& i, assignExpr& e, nbool alreadyVisited) {
        onVisit(i, (node&) e, alreadyVisited);
        WHEN(!_isShowData) .ret(true);

        cout << foreColor(LIGHTGRAY) << " = " << foreColor(MAGENTA) << _getNameFrom(e.getLeft()) << foreColor(LIGHTGRAY)
             << " = " << foreColor(MAGENTA) << _getNameFrom(e.getRight());

        return true;
    }

    nbool me::onVisit(const visitInfo& i, defVarExpr& e, nbool alreadyVisited) {
        onVisit(i, (node&) e, alreadyVisited);
        WHEN(!_isShowData) .ret(true);

        cout << foreColor(LIGHTGRAY) << " = " << foreColor(MAGENTA) << e.getName() << foreColor(LIGHTGRAY)
             << " := " << foreColor(CYAN) << _getNameFrom(e.getRight());
        return true;
    }

    string me::_getNameFrom(const node& it) const {
        string ret = it.getType().createNameWithParams();
        const auto* name = it.cast<getExpr>() TO(getName());
        if(name) ret = *name;
        return ret;
    }

    void me::_showModifier(const modifier& mod) {
        if(!mod.isPublic()) cout << foreColor(GREEN) << "_";
        if(mod.isExplicitOverride()) cout << foreColor(GREEN) << "+";
    }

    string me::_encodeNewLine(const string& msg) const { return "\"" + regex_replace(msg, regex("\n"), "\\n") + "\""; }
} // namespace by
