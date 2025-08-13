#include "core/ast/slot.hpp"

#include "core/type/typeMaker.hpp"
#include "core/worker/visitor/visitor.hpp"
#include "core/ast/exprs/getExpr.hpp"
#include "core/ast/exprs/runExpr.hpp"
#include "core/ast/baseFunc.hpp"
#include "core/ast/origin.hpp"

namespace by {

    BY(DEF_ME(slot), DEF_VISIT())

    me::slot(const manifest& manifest): _manifest(manifest), _isValid(true) {
        origin& org = *new origin(typeMaker::make<obj>(_manifest.name));
        org.setCallComplete(*new mockNode());
        _pak.bind(org);
    }

    me::slot(const manifest& manifest, const obj& pack): _manifest(manifest), _isValid(true) { _pak.bind(pack); }

    scope& me::subs() { return getPack().subs(); }

    priorType me::prioritize(const args& a) const { return getPack().prioritize(a); }

    str me::run(const args& a) { return getPack().run(a); }

    str me::getEval() const { return getPack().getEval(); }

    void me::_rel() {
        _dependents.rel();
        _pak.rel();
        _isValid = true;
    }

    obj& me::getPack() { return *_pak; }

    manifest& me::getManifest() { return _manifest; }

    const manifest& me::getManifest() const { return _manifest; }

    nbool me::isValid() const { return _isValid; }

    void me::rel() { _rel(); }

    void me::addDependent(slot& dependent) { _dependents.add(dependent); }

    const tnarr<slot>& me::getDependents() const { return _dependents; }

    void me::_setValid(nbool valid) { _isValid = valid; }

    nbool me::_invalidate() {
        _setValid(false);

        // propagate result only if it's not valid.
        for(auto& e: _dependents)
            e._invalidate();
        return true;
    }

    str me::_onRunSub(node& sub, const args& a) { return getPack()._onRunSub(sub, a); }
} // namespace by
