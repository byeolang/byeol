#include "coreInternal.hpp"
#include "core/type/mgdType.hpp"
#include "core/ast/modifier//modifier.hpp"
#include "core/frame/thread.hpp"
#include "core/worker/parser.hpp"
#include "core/ast/exprs/exprMaker.hpp"
#include "core/ast/func.hpp"
#include "core/ast/genericOrigin.hpp"
#include "core/ast/exprs/forExpr.hpp"

namespace by {
    BY(DEF_ME(coreInternal))

    void me::setSrc(node& me, const src& s) { me._setSrc(s); }

    void me::setType(baseObj& me, const mgdType& new1) { me._setType(new1); }

    void me::setModifier(baseObj& me, const modifier& mod) { me._setModifier(mod); }

    void me::setOrigin(baseObj& me, const baseObj& newOrg) { me._org.bind(newOrg); }

    void me::setOrigin(func& me, const baseObj& newOrg) { me._org.bind(newOrg); }

    void me::setOrigin(baseCtor& me, const baseObj& newOrg) { me._type.setRet(newOrg); }

    void me::setSubPack(origin& me, const node& subpack) { me._subpack.bind(subpack); }

    ntype& me::getType(node& me) { return (ntype&) me.getType(); }

    str me::onEvalSub(node& me, node& sub, const args& a) { return me._onEvalSub(sub, a); }

    frames* me::getFrames() { return thread::get() TO(_getFrames()); }

    frame* me::getNowFrame() { return thread::get() TO(_getNowFrame()); }

    exprMaker& me::getMaker(parser& me) { return me._maker; }

    orgCache& me::getCache(genericOrigin& me) { return me._cache; }

    str me::getContainer(forExpr& me) { return me._container; }
}
