// nested class of tnchain.hpp:
//  this file allows to be refered by 'tnchain.hpp' file only.
class chainIteration: public iteration {
    NM(CLASS(chainIteration, iteration))
    friend class tnchain;

public:
    chainIteration(tnchain& iteratingChain, nbool isReversed):
        me(iteratingChain, nulOf<K>(), isReversed) {}

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

    ncnt next(ncnt step) override {
        return _step([&](ncnt remain) -> ncnt { return _iter.next(remain); }, step);
    }

    ncnt stepForward(ncnt step) override {
        return _step([&](ncnt remain) -> ncnt { return _iter.stepForward(remain); }, step);
    }

    ncnt stepBackward(ncnt step) override {
        return _step([&](ncnt remain) -> ncnt { return _iter.stepBackward(remain); }, step);
    }

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
    ncnt _step(std::function<ncnt(ncnt)> closure, ncnt step) {
        ncnt remain = step;

        // if _ownIter was invalidated then _iter too.
        while(remain > 0) {
            remain -= closure(remain);
            if(remain <= 0) break;

            // _iter moved to 'End' state now.
            if(isEnd()) break;
            _ownIter.bind(_getNextContainer());
            _iter = _makeSubIter();
            if(_iter) remain--;
        }

        return step - remain;
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
