#include "core/ast/exprs/defSeqExpr.hpp"

#include "core/builtin/container/mgd/seq.hpp"
#include "core/worker/visitor/visitor.hpp"

namespace nm {

    NM(DEF_ME(defSeqExpr), DEF_VISIT())

    me::defSeqExpr(const node& start, const node& end): _start(start), _end(end) {}

    str me::run(const args& a) {
        WHEN(!_start || !_end).err("_start or _end is null").ret(str());

        str start(_start->as<node>());
        str end(_end->as<node>());
        WHEN(!start || !end).err("ased start or end is null").ret(str());

        return str(new seq(start->cast<nInt>(), end->cast<nInt>()));
    }

    str me::getEval() const {
        static str inner(new seq(1, 10));
        return inner;
    }

    const node& me::getStart() const { return *_start; }

    const node& me::getEnd() const { return *_end; }
}
