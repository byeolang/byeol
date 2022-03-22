// nested class of tnarr.hpp:
//  this file allows to be refered by 'tnarr.hpp' file only.
class narrIteration : public super::iteration {
    WRD(CLASS(narrIteration, typename super::iteration))
    friend class tnarr;

public:
    narrIteration(tnarr& own, widx n): _n(n), _own(own) {}

    wbool isEnd() const override {
        return !_own.has(_n);
    }

    /// if iteration reached to the last element to iterate, it can precede to next,
    /// which means to the End of a buffer.
    /// however, this step wasn't regarded to a step even though it proceeds.
    wcnt next(wcnt step) override {
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
    T& get() override {
        if(isEnd()) return nulOf<T>();
        return _own.get(_n);
    }

    using super::getContainer;
    tnucontainer<T>& getContainer() override { return _own; }

protected:
    wbool _onSame(const typeProvidable& rhs) const override {
        const me& cast = (const me&) rhs;
        return this->isFrom(cast.getContainer()) && _n == cast._n;
    }

private:
    widx _n;
    tnarr& _own;
};
