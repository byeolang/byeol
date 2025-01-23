// nested class of tnmap.hpp:
//  this file allows to be refered by 'tnmap.hpp' file only.
class nmapIteration: public iteration {
    NM(CLASS(nmapIteration, iteration))
    friend class tnmap;

public:
    nmapIteration(tnmap& own, citer start, citer end): _own(own), _citer(start), _end(end) {}

    nbool isEnd() const override { return _citer == _end; }

    /// if iteration reached to the last element to iterate, it can precede to next,
    /// which means to the End of a buffer.
    /// however, this step wasn't regarded to a step even though it proceeds.
    ncnt next(ncnt step) override {
        if(isEnd()) return 0;

        for(int n = 0; n < step; n++) {
            ++_citer;
            if(isEnd()) return n;
        }
        return step;
    }

    const K& getKey() const override {
        if(isEnd()) return nulOf<K>();
        return _citer.getKey();
    }

    using super::getVal;

    V& getVal() override {
        if(isEnd()) return nulOf<V>();
        return *_citer.getVal();
    }

    void setVal(const V& new1) override {
        if(isEnd()) return;
        _citer->bind(new1);
    }

    using super::getContainer;

    tbicontainable<K, V>& getContainer() override { return _own; }

protected:
    nbool _onSame(const typeProvidable& rhs) const override {
        const me& cast = (const me&) rhs;
        return this->isFrom(cast.getContainer()) && _citer == cast._citer;
    }

private:
    tnmap& _own;
    citer _citer; // cpp map's iterator.
    citer _end;
};
