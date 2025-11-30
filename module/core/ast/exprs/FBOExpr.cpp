#include "core/ast/exprs/FBOExpr.hpp"
#include "core/builtin/primitive.hpp"
#include "core/worker/visitor/visitor.hpp"

namespace by {

    BY(DEF_ME(FBOExpr), DEF_VISIT())

    me::FBOExpr(symbol s, const node& lhs, const node& rhs): _symbol(s), _lhs(str(lhs)), _rhs(str(rhs)) {}

    str me::infer() const {
        static str inner(new nBool());
        WHEN(isLogicalOp()) .ret(inner);

        WHEN(!_lhs || !_rhs) .ret(str());
        str lhsEval = _lhs->infer() OR.ret(lhsEval);
        str rhsEval = _rhs->infer() OR.ret(rhsEval);

        return lhsEval->promote(*rhsEval);
    }

    nbool me::isLogicalOp() const { return SYMBOL_LOGIC_START <= _symbol && _symbol < SYMBOL_LOGIC_END; }

    str me::run(const args& a) {
        tstr<arithmeticObj> lhs(_lhs TO(template as<arithmeticObj>()));
        tstr<arithmeticObj> rhs(_rhs TO(template as<arithmeticObj>()));
        WHEN(!lhs || !rhs) .info("lhs or rhs is null").ret(str());

        str ret;
        switch(_symbol) {
            case SYMBOL_ADD: ret = lhs->add(*rhs); break;
            case SYMBOL_SUB: ret = lhs->sub(*rhs); break;
            case SYMBOL_MUL: ret = lhs->mul(*rhs); break;
            case SYMBOL_DIV: ret = lhs->div(*rhs); break;
            case SYMBOL_MOD: ret = lhs->mod(*rhs); break;
            case SYMBOL_BITWISE_AND: ret = lhs->bitwiseAnd(*rhs); break;
            case SYMBOL_BITWISE_XOR: ret = lhs->bitwiseXor(*rhs); break;
            case SYMBOL_BITWISE_OR: ret = lhs->bitwiseOr(*rhs); break;
            case SYMBOL_LSHIFT: ret = lhs->lshift(*rhs); break;
            case SYMBOL_RSHIFT: ret = lhs->rshift(*rhs); break;
            case SYMBOL_EQ: ret.bind(new nBool(lhs->eq(*rhs))); break;
            case SYMBOL_NE: ret.bind(new nBool(lhs->ne(*rhs))); break;
            case SYMBOL_GT: ret.bind(new nBool(lhs->gt(*rhs))); break;
            case SYMBOL_LT: ret.bind(new nBool(lhs->lt(*rhs))); break;
            case SYMBOL_GE: ret.bind(new nBool(lhs->ge(*rhs))); break;
            case SYMBOL_LE: ret.bind(new nBool(lhs->le(*rhs))); break;
            case SYMBOL_AND: ret.bind(new nBool(lhs->logicalAnd(*rhs))); break;
            case SYMBOL_OR: ret.bind(new nBool(lhs->logicalOr(*rhs))); break;
            default:;
        }

        BY_I("@%s %s --> %s", this, getSymbolName(_symbol), *ret);
        return ret;
    }

    const node& me::getLeft() const { return *_lhs; }

    void me::setLeft(const node& new1) { _lhs.bind(new1); }

    const node& me::getRight() const { return *_rhs; }

    void me::setRight(const node& new1) { _rhs.bind(new1); }

    me::symbol me::getSymbol() const { return _symbol; }

    const nchar* me::getSymbolName(symbol s) {
        switch(s) {
            case SYMBOL_ADD: return "+";
            case SYMBOL_SUB: return "-";
            case SYMBOL_MUL: return "*";
            case SYMBOL_DIV: return "/";
            case SYMBOL_MOD: return "%";
            case SYMBOL_BITWISE_AND: return "&";
            case SYMBOL_BITWISE_XOR: return "^";
            case SYMBOL_BITWISE_OR: return "|";
            case SYMBOL_LSHIFT: return "<<";
            case SYMBOL_RSHIFT: return ">>";
            case SYMBOL_NE: return "!=";
            case SYMBOL_GT: return ">";
            case SYMBOL_LT: return "<";
            case SYMBOL_GE: return ">=";
            case SYMBOL_LE: return "<=";
            case SYMBOL_AND: return "&&";
            case SYMBOL_OR: return "||";
            default: return "";
        }
    }
} // namespace by
