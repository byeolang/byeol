// nested class of tnarr.hpp:
//  this file allows to be refered by 'tnarr.hpp' file only.
class narrIteration: public iteration {
    NM(CLASS(narrIteration, iteration))
    friend class tnarr;

public:
    narrIteration(tnarr& own, nidx n, nbool isReversed): super(isReversed), _n(n), _own(own) {}

    nbool isEnd() const override { return !_own.in(_n); }

    /// if iteration reached to the last element to iterate, it can precede to next,
    /// which means to the End of a buffer.
    /// however, this step wasn't regarded to a step even though it proceeds.
    ncnt stepForward(ncnt step) override {
        if(step <= 0) return 0;
        if(isEnd()) return 0;
        int len = _own.len(), lastN = len - 1;
        int toLast = lastN - _n;

        _n += step;
        if(_n >= lastN) {
            _n = len;
            step = toLast;
        }
        return step;
    }

    ncnt stepBackward(ncnt step) override {
        if(step <= 0) return 0;
        if(isEnd()) return 0;
        int toLast = _n;

        _n -= step;
        if(_n < 0) {
            _n = -1;
            step = toLast;
        }
        return step;
    }

    using super::get;

    T& get() override {
        if(isEnd()) return nulOf<T>();
        return _own.get(_n);
    }

    using super::getContainer;

    tucontainable<T>& getContainer() override { return _own; }

protected:
    nbool _onSame(const typeProvidable& rhs) const override {
        const me& cast = (const me&) rhs;
        return this->isFrom(cast.getContainer()) && _n == cast._n;
    }

private:
    nidx _n;
    tnarr& _own;
};
