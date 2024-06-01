#include "FUOExpr.hpp"
#include "../../builtin/primitive.hpp"
#include "../../visitor/visitor.hpp"

namespace namu {

    NAMU(DEF_ME(FUOExpr), DEF_VISIT())

    me::FUOExpr(rule rule, const node& it): _rule(rule), _it(it) {}

    str me::run(const args& a) {
        if(!_it) return str();
        tstr<arithmeticObj> it(_it->as<node>());
        if(!it) return str();

        str ret((node*)it->cloneDeep());
        switch(_rule) {
            case POSTFIX_DOUBLE_PLUS: {
                tstr<arithmeticObj> rhs = it->add(nInt(1));
                if(rhs)
                    it->mov(*rhs);
                break;
            }

            case POSTFIX_DOUBLE_MINUS: {
                tstr<arithmeticObj> rhs = it->sub(nInt(1));
                if(rhs)
                    it->mov(*rhs);
                break;
            }

            case BITWISE_NOT:
                ret = it->bitwiseNot();
                break;

            default: ;
        }

        return ret;
    }

    node& me::getOperand() {
        return *_it;
    }

    str me::getEval() const {
        return _it->getEval();
    }

    me::rule me::getRule() const {
        return _rule;
    }

    const nchar* me::getRuleName(rule r) {
        switch(r) {
            case POSTFIX_DOUBLE_PLUS: return "++";
            case POSTFIX_DOUBLE_MINUS: return "--";
            case BITWISE_NOT: return "~";
            default: return "";
        }
    }
}
