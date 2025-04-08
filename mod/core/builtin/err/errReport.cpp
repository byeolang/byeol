#include "errReport.hpp"

#include "../../ast/node.hpp"
#include "nerr.hpp"

namespace nm {

    NM_DEF_ME(errReport)

    me::errReport(): me(buildFeature::config::isDbg()) {}

    me::errReport(nbool isNoisy): super(), _isNoisy(isNoisy) {}

    nbool me::operator==(const me& rhs) const {
        if(len() != rhs.len()) return false;

        for(nint n = 0; n < len(); n++)
            if(get(n) != rhs[n]) return false;
        return true;
    }

    nbool me::operator!=(const me& rhs) const { return !operator==(rhs); }

    const baseErr& me::operator[](nidx n) const { return get(n); }

    me::operator nbool() const { return inErr(); }

    nbool me::inErr() const { return in(logLv::ERR); }

    nbool me::inErr(nidx since) const { return in(logLv::ERR, since); }

    nbool me::inWarn() const { return in(logLv::WARN); }

    nbool me::inWarn(nidx since) const { return in(logLv::WARN, since); }

    nbool me::in(logLv::level type, nidx since) const {
        if(since < 0) since = 0;
        for(nidx n = since; n < _errs.size(); n++)
            if(_errs[n]->getLv() == type) return true;
        return false;
    }

    nbool me::in(logLv::level type) const { return in(type, 0); }

    const baseErr& me::get(nidx n) const { return *_errs[n]; }

    ncnt me::len() const { return _errs.size(); }

    const baseErr& me::add(const baseErr& new1) {
        _noise(new1);
        _errs.push_back(new1);
        return new1;
    }

    const baseErr& me::add(const baseErr* new1) { return add(*new1); }

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
        if(!_isNoisy) return;
        new1.log();
    }

    const baseErr& dummyErrReport::add(const baseErr& new1) {
        static ndummyErr dum;
        return dum;
    }

    dummyErrReport dummyErrReport::singletone;
} // namespace nm
