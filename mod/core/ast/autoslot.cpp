#include "autoslot.hpp"
#include "baseFunc.hpp"
#include "../visitor/visitor.hpp"
#include "../type/mgdType.hpp"
#include "origin.hpp"

namespace nm {

    NM(DEF_ME(autoslot), DEF_VISIT())

    me::autoslot(const manifest& manifest, const packLoadings& loadings):
        super(manifest), _loadings(loadings), _state(RELEASED), _rpt(dummyErrReport::singletone) {}

    me::~autoslot() {
        // release all instance first:
        //  I must release allocated shared object first,
        //  before release the handle of it by releasing packLoading instance.
        me::rel();

        for(auto* e : _loadings)
            delete e;
        _loadings.clear();
    }

    void me::_rel() {
        _state = RELEASED;
        _rpt.bind(dummyErrReport::singletone);
    }

    obj& me::getPack() {
        if(_state == RELEASED) {
            const std::string& name = getManifest().name;
            _pak.bind(new origin(mgdType::make(name)));
            NM_I("%s pack is about to interpret lazy.", name);
            // TODO: check _rpt error count increased or not.
            //       if increased, then parse() function has been failed.
            parse(*_rpt, _pak->getShares()); // recursive call wasn't allowed.
            verify(*_rpt, *_pak);
            link();
        }

        return super::getPack();
    }

    me::state me::getState() const {
        return _state;
    }

    void me::setReport(errReport& rpt) {
        _rpt.bind(rpt);
    }

    void me::rel() {
        _rel();
        super::rel();
    }

    tstr<srcs> me::parse(errReport& rpt, bicontainable& tray) {
        // You shouldn't release instances which _subs is holding:
        //  there is a scenario which _subs containing parsed instance when
        //  this function called.
        //  Only you can do here is adding new parsed instances into _subs.
        for(packLoading* load : _loadings) {
            auto res = load->parse(rpt, tray);
            _srcs.add(*res);
        }
        _state = PARSED;
        return _srcs;
    }

    nbool me::verify(errReport& rpt, obj& pak) {
        for(packLoading* load : _loadings)
            load->verify(rpt, pak);

        _state = VERIFIED;
        _setValid(!_rpt); // if has an error, setValid(false);
        return true;
    }

    nbool me::link() {
        _state = LINKED;
        return !isValid() ? _invalidate() : true;
    }

    nbool me::_invalidate() {
        _setValid(false);
        if(_state != LINKED) return false;

        return super::_invalidate();
    }
}
