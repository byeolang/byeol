// nested class of tnmap.hpp:
//  this file allows to be refered by 'tnmap.hpp' file only.
class nmapIteration: public iteration {
    BY(CLASS(nmapIteration, iteration))
    friend class tnmap;

public:
    nmapIteration(tnmap& own, const K* key, nbool isReversed):
        super(isReversed),
        _own(own),
        _citer(isReversed ? own._map.rbegin(key) : own._map.begin(key)),
        _end(isReversed ? own._map.rend() : own._map.end()) {}

    nbool isEnd() const override { return _citer == _end; }

    void rel() override { _citer = _end; }

    /// if iteration reached to the last element to iterate, it can precede to next,
    /// which means to the End of a buffer.
    /// however, this step wasn't regarded to a step even though it proceeds.
    ncnt stepForward(ncnt step) override { return _step(false, step); }

    ncnt stepBackward(ncnt step) override { return _step(true, step); }

    const K* getKey() const override {
        WHEN(isEnd()) .ret(nullptr);
        return _citer.getKey();
    }

    using super::getVal;

    V* getVal() override {
        WHEN(isEnd()) .ret(nullptr);
        return _citer.getVal() TO(get());
    }

    using super::setVal;

    void setVal(const V& new1) override {
        WHEN(isEnd()) .ret();
        _citer->bind(new1);
    }

    using super::getContainer;

    tbicontainable<K, V>* getContainer() override { return &_own; }

protected:
    nbool _onSame(const typeProvidable& rhs) const override {
        const me& cast = (const me&) rhs;
        return this->isFrom(cast.getContainer()) && _citer == cast._citer;
    }

private:
    ncnt _step(nbool isReversed, ncnt step) {
        WHEN(isEnd()) .ret(0);
        for(int n = 0; n < step; n++) {
            isReversed ? (this->isReversed() ? ++_citer : --_citer) : (this->isReversed() ? --_citer : ++_citer);
            WHEN(isEnd()) .ret(n);
        }
        return step;
    }

private:
    tnmap& _own;
    citer _citer; // cpp map's iterator.
    citer _end;
};
