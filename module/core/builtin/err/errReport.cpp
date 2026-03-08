#include "core/builtin/err/errReport.hpp"

#include "core/ast/node.hpp"
#include "core/builtin/err/nerr.hpp"

namespace by {

    BY_DEF_ME(errReport)

    me::errReport(): me(buildFeature::config::isDbg()) {}

    me::errReport(nbool isNoisy): super(), _isNoisy(isNoisy) {}

    nbool me::operator==(const me& rhs) const {
        WHEN(len() != rhs.len()) .ret(false);

        for(nint n = 0; n < len(); n++) {
            const baseErr& elem = get(n) OR_CONTINUE;
            WHEN(elem != rhs[n]) .ret(false);
        }
        return true;
    }

    nbool me::operator!=(const me& rhs) const { return !operator==(rhs); }

    const baseErr& me::operator[](nidx n) const { return *get(n); }

    me::operator nbool() const { return inErr(); }

    nbool me::inErr() const { return in(errLv::ERR); }

    nbool me::inErr(errCode code) const { return in(errLv::ERR, code); }

    nbool me::inErr(errCode code, nidx since) const { return in(errLv::ERR, code, since); }

    nbool me::inWarn() const { return in(errLv::WARN); }

    nbool me::inWarn(errCode code) const { return in(errLv::WARN, code); }

    nbool me::inWarn(errCode code, nidx since) const { return in(errLv::WARN, code, since); }

    nbool me::in(errLv::level type, errCode code, nidx since) const {
        if(since < 0) since = 0;
        for(nidx n = since; n < _errs.size(); n++) {
            const baseErr& e = _errs[n].get() OR_CONTINUE;
            if(e.getLv() != type) continue;
            if(code == NOT_SPECIFIED || e.getErrCode() == code) return true;
        }
        return false;
    }

    nbool me::in(errLv::level type) const { return in(type, NOT_SPECIFIED, 0); }

    nbool me::in(errLv::level type, errCode code) const { return in(type, code, 0); }

    const baseErr* me::get(nidx n) const { return _errs[n].get(); }

    ncnt me::len() const { return _errs.size(); }

    const baseErr& me::add(const baseErr& new1) {
        _noise(new1);
        _errs.push_back(new1);
        return new1;
    }

    void me::add(const me& rhs) {
        for(const auto& e: rhs)
            add(*e);
    }

    std::vector<tstr<baseErr>>::const_iterator me::begin() const { return _errs.begin(); }

    std::vector<tstr<baseErr>>::const_iterator me::last() const { return end() - 1; }

    std::vector<tstr<baseErr>>::const_iterator me::end() const { return _errs.end(); }

    void me::log(nidx since) const {
        for(nidx n = since; n < _errs.size(); n++)
            _errs[n]->log();
    }

    void me::log() const { return log(0); }

    void me::dump(nidx since) const {
        for(nidx n = since; n < _errs.size(); n++)
            _errs[n]->dump();
    }

    void me::dump() const { return dump(0); }

    void me::rel() { _errs.clear(); }

    nbool me::isNoisy() const { return _isNoisy; }

    me& me::setNoisy(nbool isNoisy) {
        _isNoisy = isNoisy;
        return *this;
    }

    void me::_noise(const baseErr& new1) {
        WHEN(!_isNoisy) .ret();
        enablesZone zone(true);
        new1.log();
    }

    dummyErrReport::dummyErrReport(): super(false) {}

    const baseErr& dummyErrReport::add(const baseErr& new1) {
        static ndummyErr dum;
        return dum;
    }

    me& dummyErrReport::setNoisy(nbool isNoisy) { return *this; }

    dummyErrReport dummyErrReport::singleton;
} // namespace by
