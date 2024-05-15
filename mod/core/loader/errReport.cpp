#include "../ast/node.hpp"
#include "errReport.hpp"

namespace namu {

    NAMU_DEF_ME(errReport)

    nbool me::operator==(const me& rhs) const {
        if(len() != rhs.len()) return false;

        for(nint n=0; n < len() ;n++)
            if(get(n) != rhs[n])
                return false;
        return true;
    }

    nbool me::operator!=(const me& rhs) const { return !operator==(rhs); }
    const err& me::operator[](nidx n) const { return get(n); }
    me::operator nbool() const { return hasErr(); }

    nbool me::hasErr() const { return has(logLv::ERR); }
    nbool me::hasErr(nidx since) const { return has(logLv::ERR, since); }

    nbool me::hasWarn() const { return has(logLv::WARN); }
    nbool me::hasWarn(nidx since) const { return has(logLv::WARN, since); }

    nbool me::has(logLv::level type, nidx since) const {
        if (since < 0) since = 0;
        for(nidx n=since; n < _errs.size() ;n++)
            if(_errs[n]->fType == type)
                return true;
        return false;
    }
    nbool me::has(logLv::level type) const { return has(type, 0); }

    const err& me::get(nidx n) const {
        return *_errs[n];
    }

    ncnt me::len() const {
        return _errs.size();
    }

    const err& me::add(const err* new1) {
        _errs.push_back(new1);
        return *new1;
    }

    const err& me::add(const err& new1) {
        return add(&new1);
    }

    void me::add(const me& rhs) {
        for(const auto& e : rhs)
            add(*e);
    }

    std::vector<tstr<err>>::const_iterator me::begin() const {
        return _errs.begin();
    }

    std::vector<tstr<err>>::const_iterator me::last() const {
        return end() - 1;
    }

    std::vector<tstr<err>>::const_iterator me::end() const {
        return _errs.end();
    }

    void me::log() const {
        for(auto& elem : _errs)
            elem->log();
    }

    void me::dump() const {
        for(auto& elem : _errs)
            elem->dump();
    }

    void me::rel() {
        _errs.clear();
    }

    const err& dummyErrReport::add(const err* new1) {
        static dummyErr dum;
        return dum;
    }

    dummyErrReport dummyErrReport::singletone;
}
