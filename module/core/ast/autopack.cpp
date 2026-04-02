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
            parse(rpt, getShares()); // recursive call wasn't allowed.
            verify(rpt, *this);
            exRpt.add(rpt); // add errors if they occurs during loading.

            link();
        }

        return super::subs();
    }

    state me::getState() const { return _state; }

    void me::setState(state new1) { _state = new1; }

    void me::rel() {
        _rel();
        super::rel();
    }

    tstr<srcs> me::parse(errReport& rpt, bicontainable& tray) {
        // You shouldn't release instances which _subs is holding:
        //  there is a scenario which _subs containing parsed instance when
        //  this function called.
        //  Only you can do here is adding new parsed instances into _subs.
        for(packLoading* load: _loadings) {
            auto res = load->parse(rpt, tray);
            _srcs.add(*res);
        }
        _state = PARSED; // don't need to expand.
        return _srcs;
    }

    nbool me::verify(errReport& rpt, obj& pak) {
        for(packLoading* load: _loadings)
            load->verify(rpt, pak);

        _state = VERIFIED;
        _setValid(!rpt); // if has an error, setValid(false);
        return true;
    }

    nbool me::link() {
        _state = LINKED;
        return !isValid() ? _invalidate() : true;
    }

    nbool me::_invalidate() {
        _setValid(false);
        WHEN(_state != LINKED) .ret(false);

        return super::_invalidate();
    }
} // namespace by
