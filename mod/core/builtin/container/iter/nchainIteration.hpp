// nested class of tnchain.hpp:
//  this file allows to be refered by 'tnchain.hpp' file only.
class chainIteration: public iteration {
    NM(CLASS(chainIteration, iteration))
    friend class tnchain;

public:
    chainIteration(tnchain& iteratingChain, const K& key, nbool isReversed):
        super(isReversed), _ownIter(iteratingChain), _key(key), _iter(_makeSubIter()) {
        if(!_iter) next(1);
    }

    nbool isEnd() const override {
        if(!nul(_getNextContainer())) return false;
        return !_iter;
    }

    void rel() override {
        _iter.rel();
        _ownIter.rel();
    }

    ncnt next(ncnt step) override { return _step(super::NEXT, step); }

    ncnt stepForward(ncnt step) override { return _step(super::FORWARD, step); }

    ncnt stepBackward(ncnt step) override { return _step(super::BACKWARD, step); }

    using super::getContainer;

    tbicontainable<K, V>& getContainer() override {
        if(!_ownIter) return nulOf<tbicontainable<K, V>>();
        return *_ownIter;
    }

    const K& getKey() const override { return _iter.getKey(); }

    using super::getVal;

    V& getVal() override { return _iter.getVal(); }

    void setVal(const V& new1) override { _iter.setVal(new1); }

protected:
    nbool _onSame(const typeProvidable& rhs) const override {
        const me& cast = (const me&) rhs;
        return (isEnd() && cast.isEnd()) || _iter == cast._iter;
    }

private:
    ncnt _step(typename super::IterationType type, ncnt step) {
        ncnt remain = step;

        // if _ownIter was invalidated then _iter too.
        while(remain > 0) {
            remain -= _iterate(type, remain);
            if(remain <= 0) break;

            // _iter moved to 'End' state now.
            if(isEnd()) break;
            _ownIter.bind(_getNextContainer());
            _iter = _makeSubIter();
            if(_iter) remain--;
        }

        return step - remain;
    }

    ncnt _iterate(typename super::IterationType type, ncnt step) {
        switch(type) {
            case super::FORWARD: return _iter.stepForward(step);
            case super::BACKWARD: return _iter.stepBackward(step);
            default:
            case super::NEXT: return _iter.next(step);
        }
    }

    const tnchain& _getNextContainer() const {
        if(!_ownIter) return nulOf<tnchain>();
        return this->isReversed() ? _ownIter->getPrev() : _ownIter->getNext();
    }

    iter _makeSubIter() const {
        return this->isReversed() ? _ownIter->_map->riterate(_key) : _ownIter->_map->iterate(_key);
    }

private:
    tstr<tnchain> _ownIter; // _ownIter shouldn't be null always.
    const K& _key;
    iter _iter;
};
