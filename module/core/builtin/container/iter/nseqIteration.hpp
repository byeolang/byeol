// nested class of nseq.hpp:
//  this file allows to be refered by 'nseq.hpp' file only.
class nseqIteration: public iteration {
    BY(CLASS(nseqIteration, iteration))
    friend class tnseq;

public:
    nseqIteration(nseq& own, nidx n, nbool isReversed):
        super(isReversed), _n(isReversed ? own.len() - 1 - n : n), _own(own) {}

    using super::operator*;

    nInt operator*() override { return get(); }

public:
    nbool isEnd() const override { return !_own.in(_n); }

    void rel() override { _n = this->isReversed() ? -1 : _own.len(); }

    /// if iteration reached to the last element to iterate, it can precede to next,
    /// which means to the End of a buffer.
    /// however, this step wasn't regarded to a step even though it proceeds.
    ncnt stepForward(ncnt step) override {
        WHEN(step <= 0) .ret(0);
        WHEN(isEnd()) .ret(0);
        int len = _own.len();
        int toLast = len - 1 - _n;

        _n += step;
        if(_n >= len) {
            _n = len;
            step = toLast;
        }
        return step;
    }

    ncnt stepBackward(ncnt step) override {
        WHEN(step <= 0) .ret(0);
        WHEN(isEnd()) .ret(0);
        int toLast = _n;

        _n -= step;
        if(_n < 0) {
            _n = -1;
            step = toLast;
        }
        return step;
    }

    using super::get;

    nInt get() override {
        WHEN(isEnd()) .ret(nInt(0));
        return _own[_n];
    }

    using super::getContainer;

    tucontainable<nInt, nInt, nInt>* getContainer() override { return &_own; }

protected:
    nbool _onSame(const typeProvidable& rhs) const override {
        const me& cast = (const me&) rhs;
        return this->isFrom(cast.getContainer()) && _n == cast._n;
    }

private:
    nidx _n;
    nseq& _own;
};
