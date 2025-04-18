#include "FUOExpr.hpp"

#include "../../builtin/primitive.hpp"
#include "../../worker/visitor/visitor.hpp"

namespace nm {

    NM(DEF_ME(FUOExpr), DEF_VISIT())

    me::FUOExpr(symbol s, const node& it): _symbol(s), _it(it) {}

    str me::run(const args& a) {
        WHEN(!_it).ret(str());
        tstr<arithmeticObj> it = _it->as<node>() OR.ret(str());

        str ret((node*) it->cloneDeep());
        switch(_symbol) {
            case SYMBOL_POSTFIX_DOUBLE_PLUS: {
                tstr<arithmeticObj> rhs = it->add(nInt(1));
                if(rhs) it->mov(*rhs);
                break;
            }

            case SYMBOL_POSTFIX_DOUBLE_MINUS: {
                tstr<arithmeticObj> rhs = it->sub(nInt(1));
                if(rhs) it->mov(*rhs);
                break;
            }

            case SYMBOL_BITWISE_NOT: ret = it->bitwiseNot(); break;

            default:;
        }

        return ret;
    }

    node& me::getOperand() { return *_it; }

    str me::getEval() const { return _it->getEval(); }

    me::symbol me::getSymbol() const { return _symbol; }

    const nchar* me::getSymbolName(symbol s) {
        switch(s) {
            case SYMBOL_POSTFIX_DOUBLE_PLUS: return "++";
            case SYMBOL_POSTFIX_DOUBLE_MINUS: return "--";
            case SYMBOL_BITWISE_NOT: return "~";
            default: return "";
        }
    }
} // namespace nm
