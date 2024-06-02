#include "defPropExpr.hpp"
#include "../../frame/thread.hpp"
#include "../../visitor/visitor.hpp"

namespace namu {

    NAMU(DEF_ME(defPropExpr), DEF_VISIT())

    me::defPropExpr(const std::string& name, const node& org): _org(org), _name(name) {}
    me::defPropExpr(const std::string& name, const node& org, const scope& where): _org(org),
            _name(name), _where(where) {}

    str me::run(const args& a) {
        str org = _org->as<node>();
        if(!org)
            return NAMU_E("getting origin by %s returns null", _name.c_str()), org;

        nbool res = _where ? _where->add(_name, *org->run()) :
            thread::get()._getNowFrame().addLocal(_name, *org->run());
        if(!res)
            NAMU_E("define variable %s is failed.", _name.c_str());

        return org;
    }

    void me::setOrigin(const node& newOrg) {
        _org.bind(newOrg);
    }

    const std::string& me::getName() const {
        return _name;
    }

    const node& me::getOrigin() const {
        return *_org;
    }

    /// @return null of scope if this variable will be defined to local scope.
    const scope& me::getWhere() const {
        return *_where;
    }

    void me::setWhere(const scope& new1) {
        _where.bind(new1);
    }

    str me::getEval() const {
        return _org->getEval();
    }
}
