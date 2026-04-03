#include "core/ast/pack.hpp"

#include "core/type/typeMaker.hpp"
#include "core/worker/visitor/visitor.hpp"
#include "core/ast/exprs/getExpr.hpp"
#include "core/ast/exprs/evalExpr.hpp"
#include "core/ast/baseFunc.hpp"
#include "core/ast/origin.hpp"
#include "core/internal/coreInternal.hpp"

namespace by {

    BY(DEF_ME(pack), DEF_VISIT())

    me::pack(const manifest& mani): super(), _manifest(mani) {
        origin* org = new origin(typeMaker::make<obj>(_manifest.name));
        org->setCallComplete(*new mockNode());
        coreInternal::setOrigin(*this, *org);
    }

    void me::_rel() {
        _dependents.rel();
    }

    manifest& me::getManifest() { return _manifest; }

    const manifest& me::getManifest() const { return _manifest; }

    nbool me::isValid() const { return true; }

    void me::rel() { _rel(); }

    void me::addDependent(pack& dependent) { _dependents.add(dependent); }

    const tnarr<pack>& me::getDependents() const { return _dependents; }

    void me::_invalidate() {
        // propagate result only if it's not valid.
        for(auto& e: _dependents)
            e._invalidate();
    }
} // namespace by
