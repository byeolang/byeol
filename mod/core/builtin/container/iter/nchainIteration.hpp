// nested class of tnchain.hpp:
//  this file allows to be refered by 'tnchain.hpp' file only.
class chainIteration: public iteration {
    NM(CLASS(chainIteration, iteration))
    friend class tnchain;

public:
    chainIteration(tnchain& iteratingChain, const K& key, nbool isReversed):
        me(iteratingChain, key, isReversed, false) {}

    chainIteration(tnchain& iteratingChain, const K& key, nbool isReversed, nbool isBoundary):
        super(isReversed),
        _ownIter(iteratingChain),
        _key(key),
        _iter(_makeSubIter()),
        _isBoundary(isBoundary) {
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

    nbool isBoundary() const { return _isBoundary; }

    ncnt next(ncnt step) override { return _step(super::NEXT, step); }

    ncnt prev(ncnt step) override { return _step(super::PREV, step); }

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
    ncnt _step(typename super::iterationType type, ncnt step) {
        ncnt remain = step;

        // if _ownIter was invalidated then _iter too.
        while(remain > 0) {
            remain -= _iterate(type, remain);
            if(remain <= 0) break;

            // _iter moved to 'End' state now.
            if(isEnd()) break;
            _updateIter();
            if(_iter) remain--;
        }

        return step - remain;
    }

    ncnt _iterate(typename super::iterationType type, ncnt step) {
        switch(type) {
            case super::FORWARD: return _iter.stepForward(step);
            case super::BACKWARD: return _iter.stepBackward(step);
            case super::PREV: return _iter.prev(step);
            default:
            case super::NEXT: return _iter.next(step);
        }
    }

    const iter& _getNextIter() const {
        if(!_ownIter) return nulOf<iter>();
        return this->isReversed() ? _ownIter->_prev : _ownIter->_next;
    }

    const tnchain& _getNextContainer() const {
        if(!_ownIter) return nulOf<tnchain>();
        return this->isReversed() ? _ownIter->getPrev() : _ownIter->getNext();
    }

    void _updateIter() {
        const iter& nextIter = _getNextIter();
        if(nul(nextIter)) {
            _ownIter.rel();
            _iter.rel();
        }
        _ownIter.bind(typeProvidable::safeCast<tnchain>(nextIter.getContainer()));
        me& nextIteration = typeProvidable::safeCast<me>(*nextIter._iteration);
        _iter = nextIteration._isBoundary ? _makeSubIter() : nextIteration._iter;
        if(!nul(_key) && (nul(_iter.getKey()) || _key != _iter.getKey())) _iter.next(1);
    }

    iter _makeSubIter() const {
        return this->isReversed() ? _ownIter->_map->riterate(_key) : _ownIter->_map->iterate(_key);
    }

private:
    /// iter for tnchain.
    /// this shouldn't be null always.
    tstr<tnchain> _ownIter;
    const K& _key;
    //// iter for container of tnchain
    iter _iter;

    /// the `_boundary` means that this `_iter` is at the boundary of the chain pointed to by
    /// `_ownIter` when it is updated.
    ///
    /// the chain is linked on an iterator basis. in special cases, chains may be linked by an
    /// iterator based on a particular key.
    /// in this case, you will have a link in a specific direction from the key, not the entire
    /// chain with that key.
    ///
    /// in most cases, however, you'll want to have a link to the chain itself.
    /// typically, you'll link to the `iter' returned by the chain's `begin()`.
    /// but what happens if the pair that was the first iter is deleted?
    /// in this case, it behaves just like linking a chain based on a specific key, which we
    /// discussed earlier.
    /// it'll go one step forward in the direction of the `iter` and find the next pair.
    ///
    /// the user should be able to put a new `pair` at the front of the chain after linking, or
    /// delete the linked `pair`.
    /// so, my linkage algorithm should be based on `iter`, but I'll need to distinguish between
    /// linking the whole thing and linking a part of it.
    /// and this is where `_boundary` comes in.
    nbool _isBoundary;
};
