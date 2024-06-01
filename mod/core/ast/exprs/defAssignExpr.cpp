#include "defAssignExpr.hpp"
#include "../../frame/thread.hpp"
#include "../../visitor/visitor.hpp"

namespace namu {
    NAMU(DEF_ME(defAssignExpr), DEF_VISIT())

    me::defAssignExpr(const std::string& name, const node& rhs): _rhs(rhs),
            _isOnDefBlock(false), _name(name) {}
    me::defAssignExpr(const node& to, const std::string& name, const node& rhs):
            _to(to), _rhs(rhs), _isOnDefBlock(false), _name(name) {}

    str me::run(const args& a) {
        str new1 = _rhs->as<node>();
        if(!new1)
            return NAMU_E("new1 is null"), str();

        node& to = getTo();
        if(nul(to))
            thread::get()._getNowFrame().pushLocal(_name, *new1);
        else
            to.run()->subs().add(_name, *new1);

        return new1;
    }

    str me::getEval() const {
        return _rhs->getEval();
    }

    const std::string& me::getSubName() const {
        return _name;
    }

    const node& me::getRight() const {
        return *_rhs;
    }

    node& me::getRight() {
        return *_rhs;
    }

    nbool me::isOnDefBlock() const {
        return _isOnDefBlock;
    }

    void me::setOnDefBlock(nbool isOnDefBlock) {
        _isOnDefBlock = isOnDefBlock;
    }

    void me::setTo(const node& new1) {
        _to.bind(new1);
    }

    clonable* me::cloneDeep() const {
        NAMU_DW("defAssignExpr: cloneDeep");

        me* ret = (me*) clone();
        if(_to) ret->_to.bind((node*) _to->cloneDeep());
        if(_rhs) ret->_rhs.bind((node*) _rhs->cloneDeep());

        return ret;
    }

    node& me::getTo() {
        return *_to;
    }
}
