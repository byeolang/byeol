#include "FBOExpr.hpp"

#include "../../builtin/primitive.hpp"
#include "../../worker/visitor/visitor.hpp"

namespace nm {

    NM(DEF_ME(FBOExpr), DEF_VISIT())

    me::FBOExpr(rule rule, const node& lhs, const node& rhs):
        _rule(rule), _lhs(str(lhs)), _rhs(str(rhs)) {}

    str me::getEval() const {
        static str inner(new nBool());
        WHEN(isLogicalOp()).ret(inner);

        WHEN(!_lhs || !_rhs).ret(str());
        str lhsEval = _lhs->getEval() orRet lhsEval;
        str rhsEval = _rhs->getEval() orRet rhsEval;

        return lhsEval->deduce(*rhsEval);
    }

    nbool me::isLogicalOp() const { return LOGIC_START <= _rule && _rule < LOGIC_END; }

    str me::run(const args& a) {
        tstr<arithmeticObj> lhs(_lhs TO(template as<arithmeticObj>()));
        tstr<arithmeticObj> rhs(_rhs TO(template as<arithmeticObj>()));
        WHEN(!lhs || !rhs).info("lhs or rhs is null").ret(str());

        str ret;
        switch(_rule) {
            case ADD: ret = lhs->add(*rhs); break;
            case SUB: ret = lhs->sub(*rhs); break;
            case MUL: ret = lhs->mul(*rhs); break;
            case DIV: ret = lhs->div(*rhs); break;
            case MOD: ret = lhs->mod(*rhs); break;
            case BITWISE_AND: ret = lhs->bitwiseAnd(*rhs); break;
            case BITWISE_XOR: ret = lhs->bitwiseXor(*rhs); break;
            case BITWISE_OR: ret = lhs->bitwiseOr(*rhs); break;
            case LSHIFT: ret = lhs->lshift(*rhs); break;
            case RSHIFT: ret = lhs->rshift(*rhs); break;
            case EQ: ret.bind(new nBool(lhs->eq(*rhs))); break;
            case NE: ret.bind(new nBool(lhs->ne(*rhs))); break;
            case GT: ret.bind(new nBool(lhs->gt(*rhs))); break;
            case LT: ret.bind(new nBool(lhs->lt(*rhs))); break;
            case GE: ret.bind(new nBool(lhs->ge(*rhs))); break;
            case LE: ret.bind(new nBool(lhs->le(*rhs))); break;
            case AND: ret.bind(new nBool(lhs->logicalAnd(*rhs))); break;
            case OR: ret.bind(new nBool(lhs->logicalOr(*rhs))); break;
            default:;
        }

        NM_I("@%s %s --> %s", this, getRuleName(_rule), *ret);
        return ret;
    }

    const node& me::getLeft() const { return *_lhs; }

    void me::setLeft(const node& new1) { _lhs.bind(new1); }

    const node& me::getRight() const { return *_rhs; }

    void me::setRight(const node& new1) { _rhs.bind(new1); }

    me::rule me::getRule() const { return _rule; }

    const nchar* me::getRuleName(rule r) {
        switch(r) {
            case ADD: return "+";
            case SUB: return "-";
            case MUL: return "*";
            case DIV: return "/";
            case MOD: return "%";
            case BITWISE_AND: return "&";
            case BITWISE_XOR: return "^";
            case BITWISE_OR: return "|";
            case LSHIFT: return "<<";
            case RSHIFT: return ">>";
            case NE: return "!=";
            case GT: return ">";
            case LT: return "<";
            case GE: return ">=";
            case LE: return "<=";
            case AND: return "&&";
            case OR: return "||";
            default: return "";
        }
    }
} // namespace nm
