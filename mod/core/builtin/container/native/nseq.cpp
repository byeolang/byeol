#include "nseq.hpp"

#include "core/builtin/container/tucontainable.inl"
#include "core/frame/thread.hpp"
#include "core/builtin/err/nerr.hpp"

namespace nm {

    NM(DEF_ME(nseq))

    me::nseq(const nInt& start, const nInt& end): _start(start), _end(end) { _updateStep(); }

    me::nseq(const nInt& start, const nInt& end, const nInt& step):
        _start(start), _end(end), _step(step) {}

    nInt me::operator[](nidx n) { return get(n); }

    const nInt& me::getStart() const { return _start; }

    const nInt& me::getEnd() const { return _end; }

    const nInt& me::getStep() const { return _step; }

    void me::rel() {}

    nbool me::add(const iter& at, const nInt& new1) { return false; }

    void me::add(const iter& at, const iter& from, const iter& to) {}

    nbool me::add(nidx n, const nInt& new1) { return false; }

    nbool me::set(const iter& at, const nInt& new1) { return false; }

    nbool me::set(nidx n, const nInt& new1) { return false; }

    nbool me::del(const iter& it) { return false; }

    nbool me::del(const iter& from, const iter& end) { return false; }

    nbool me::del(nidx n) { return false; }

    void me::_updateStep() { _step.get() = _end.get() >= _start.get() ? 1 : -1; }

    ncnt me::len() const { return abs(_end.get() - _start.get()); }

    me::iteration* me::_onMakeIteration(ncnt step, nbool isReversed) const {
        me* unconst = (me*) this;
        return new nseqIteration(*unconst, step, isReversed);
    }

    nInt me::get(nidx n) {
        if(n >= len()) {
            thread::get().getEx().add(nerr::newErr(OUT_OF_RANGE, n, len()));
            n = len() - 1;
        }
        if(n < 0) {
            thread::get().getEx().add(nerr::newErr(OUT_OF_RANGE, n, len()));
            n = 0;
        }

        return nInt(_start.get() + _step.get() * n);
    }

    nbool me::in(nidx n) const { return len() > n && n >= 0; }
} // namespace nm
