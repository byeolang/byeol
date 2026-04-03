#include "core/ast/autopack.hpp"

#include "core/type/typeMaker.hpp"
#include "core/worker/visitor/visitor.hpp"
#include "core/ast/exprs/getExpr.hpp"
#include "core/ast/exprs/evalExpr.hpp"
#include "core/ast/baseFunc.hpp"
#include "core/ast/origin.hpp"
#include "core/frame/thread.hpp"

namespace by {

    BY(DEF_ME(autopack), DEF_VISIT())

    me::autopack(const manifest& manifest, const packLoadings& loadings):
        super(manifest), _loadings(loadings), _state(RELEASED) {
        coreInternal::setOrigin(*this, *this);
    }

    me::~autopack() {
        // release all instance first:
        //  I must release allocated shared object first,
        //  before release the handle of it by releasing packLoading instance.
        me::rel();

        for(auto* e: _loadings)
            delete e;
        _loadings.clear();
    }

    void me::_rel() { _state = RELEASED; }

    namespace {
        errReport& _getReport() { return thread::get().getEx(); }
    }

    scope& me::subs() {
        if(_state == RELEASED) {
            BY_I("%s pack is about to load in lazy.", getManifest().name);

            auto& exRpt = _getReport();
            errReport rpt(exRpt.isNoisy());
            // TODO: check rpt error count increased or not.
            //       if increased, then parse() function has been failed.
            parse(rpt, *this); // recursive call wasn't allowed.
            expand(rpt, *this);
            verify(rpt, *this);
            exRpt.add(rpt); // add errors if they occurs during loading.

            link();
        }

        return super::subs();
    }

    state me::getState() const { return _state; }

    void me::setState(state new1) { _state = new1; }

    nbool me::isValid() const { return _state == LINKED; }

    void me::rel() {
        _rel();
        super::rel();
    }

    void me::_invalidate() {
        _state = INVALID;
        super::_invalidate();
    }

    nbool me::parse(errReport& rpt, pack& pak) {
        // change state first:
        //  during parsing, another nodes can call subs() of this instance
        //  which makes the chain of recursive call.
        _state = PARSING;

        // You shouldn't release instances which _subs is holding:
        //  there is a scenario which _subs containing parsed instance when
        //  this function called.
        //  Only you can do here is adding new parsed instances into _subs.
        for(packLoading* load: _loadings) {
            nbool res = load->parse(rpt, pak);
            WHEN(!res).exErr(PACK_NOT_LOADED, rpt, getManifest().name)
                .run([&] { _invalidate(); }).ret(false);
        }

        _state = PARSED;
        return true;
    }

    nbool me::expand(errReport& rpt, pack& pak) {
        for(packLoading* load : _loadings) {
            nbool res = load->expand(rpt, pak);
            WHEN(!res).exErr(PACK_NOT_LOADED, rpt, getManifest().name)
                .run([&] { _invalidate(); }).ret(false);
        }

        _state = EXPANDED;
        return true;
    }

    nbool me::verify(errReport& rpt, pack& pak) {
        for(packLoading* load: _loadings) {
            nbool res = load->verify(rpt, pak);
            WHEN(!res).exErr(PACK_NOT_LOADED, rpt, getManifest().name)
                .run([&] { _invalidate(); }).ret(false);
        }

        _state = VERIFIED;
        return true;
    }

    nbool me::link() {
        _state = LINKED;
        return true;
    }
} // namespace by
