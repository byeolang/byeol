#include "core/ast/exprs/FUOExpr.hpp"

#include "core/builtin/scalar.hpp"
#include "core/worker/visitor/visitor.hpp"

namespace by {

    BY(DEF_ME(FUOExpr), DEF_VISIT())

    me::FUOExpr(symbol s, const node& it): _symbol(s), _it(it) {}

    str me::eval(const args& a) {
        WHEN(!_it) .ret(str());
        tstr<scalar> it = _it->as<scalar>() OR.ret(str());

        str ret((node*) it->cloneDeep());
        switch(_symbol) {
            case SYMBOL_POSTFIX_DOUBLE_PLUS: {
                tstr<scalar> rhs = it->add(nInt(1));
                if(rhs) it->mov(*rhs);
                break;
            }

            case SYMBOL_POSTFIX_DOUBLE_MINUS: {
                tstr<scalar> rhs = it->sub(nInt(1));
                if(rhs) it->mov(*rhs);
                break;
            }

            case SYMBOL_BITWISE_NOT: ret = it->bitwiseNot(); break;

            default:;
        }

        return ret;
    }

    node& me::getOperand() { return *_it; }

    str me::infer() const { return _it->infer(); }

    me::symbol me::getSymbol() const { return _symbol; }

    const nchar* me::getSymbolName(symbol s) {
        switch(s) {
            case SYMBOL_POSTFIX_DOUBLE_PLUS: return "++";
            case SYMBOL_POSTFIX_DOUBLE_MINUS: return "--";
            case SYMBOL_BITWISE_NOT: return "~";
            default: return "";
        }
    }
} // namespace by
