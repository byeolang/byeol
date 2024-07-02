// nested class of nseq.hpp:
//  this file allows to be refered by 'nseq.hpp' file only.
class nseqIteration : public iteration {
    NM(CLASS(nseqIteration, iteration))
    friend class tnseq;

public:
    nseqIteration(nseq& own, nidx n): _n(n), _own(own) {}

    nbool isEnd() const override {
        return !_own.has(_n);
    }

    /// if iteration reached to the last element to iterate, it can precede to next,
    /// which means to the End of a buffer.
    /// however, this step wasn't regarded to a step even though it proceeds.
    ncnt next(ncnt step) override {
        if(step <= 0) return 0;
        if(isEnd()) return 0;

        int len = _own.len(),
            lastN = len - 1;
        int toLast = lastN - _n;

        _n += step;
        if(_n > lastN) {
            _n = len;
            step = toLast;
        }
        return step;
    }
    using super::get;
    nInt get() override {
        if(isEnd()) return nulOf<nInt>();
        return _own[_n];
    }

    using super::getContainer;
    tucontainable<nInt, nInt>& getContainer() override {
        return _own;
    }

protected:
    nbool _onSame(const typeProvidable& rhs) const override {
        const me& cast = (const me&) rhs;
        return this->isFrom(cast.getContainer()) && _n == cast._n;
    }

private:
    nidx _n;
    nseq& _own;
};
