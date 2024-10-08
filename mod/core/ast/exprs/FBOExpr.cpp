#include "FBOExpr.hpp"

#include "../../builtin/primitive.hpp"
#include "../../worker/visitor/visitor.hpp"

namespace nm {

    NM(DEF_ME(FBOExpr), DEF_VISIT())

    me::FBOExpr(rule rule, const node& lhs, const node& rhs):
        _rule(rule), _lhs(str(lhs)), _rhs(str(rhs)) {}

    str me::getEval() const {
        static str inner(new nBool());
        if(isLogicalOp()) return inner;

        if(!_lhs || !_rhs) return str();
        str lhsEval = _lhs->getEval() orRet lhsEval;
        str rhsEval = _rhs->getEval() orRet rhsEval;

        return lhsEval->deduce(*rhsEval);
    }

    nbool me::isLogicalOp() const { return LOGIC_START <= _rule && _rule < LOGIC_END; }

    str me::run(const args& a) {
        tstr<arithmeticObj> lhs(_lhs THEN(template as<arithmeticObj>()));
        tstr<arithmeticObj> rhs(_rhs THEN(template as<arithmeticObj>()));
        if(!lhs || !rhs) return NM_I("lhs or rhs is null"), str();

        switch(_rule) {
            case ADD: return lhs->add(*rhs);
            case SUB: return lhs->sub(*rhs);
            case MUL: return lhs->mul(*rhs);
            case DIV: return lhs->div(*rhs);
            case MOD: return lhs->mod(*rhs);
            case BITWISE_AND: return lhs->bitwiseAnd(*rhs);
            case BITWISE_XOR: return lhs->bitwiseXor(*rhs);
            case BITWISE_OR: return lhs->bitwiseOr(*rhs);
            case LSHIFT: return lhs->lshift(*rhs);
            case RSHIFT: return lhs->rshift(*rhs);
            case EQ: return str(new nBool(lhs->eq(*rhs)));
            case NE: return str(new nBool(lhs->ne(*rhs)));
            case GT: return str(new nBool(lhs->gt(*rhs)));
            case LT: return str(new nBool(lhs->lt(*rhs)));
            case GE: return str(new nBool(lhs->ge(*rhs)));
            case LE: return str(new nBool(lhs->le(*rhs)));
            case AND: return str(new nBool(lhs->logicalAnd(*rhs)));
            case OR: return str(new nBool(lhs->logicalOr(*rhs)));
            default: return str();
        }
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
