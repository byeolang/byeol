// nested class of tnchain.hpp:
//  this file allows to be refered by 'tnchain.hpp' file only.
class chainIteration: public iteration {
    NM(CLASS(chainIteration, iteration))
    friend class tnchain;

public:
    chainIteration(const tnchain& iteratingChain, const iter& conIter, const K& byKey = nulOf<K>()):
        _ownIter(iteratingChain), _iter(conIter), _byKey(byKey) {
        if(!_iter) next(1);
    }

    nbool isEnd() const override { return !_ownIter->_next && !_iter; }

    ncnt next(ncnt step) override {
        ncnt remain = step;

        // if _ownIter was invalidated then _iter too.
        while(remain > 0) {
            remain -= _iter.next(remain);
            if(remain <= 0) break;

            // _iter moved to 'End' state now.
            if(isEnd()) break;
            _ownIter = _ownIter->_next;
            _iter = nul(_byKey) ? _ownIter->_map->begin() : _ownIter->_map->iterate(_byKey);
            if(_iter) remain--;
        }

        return step - remain;
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
        if(nul(_byKey) ? !nul(cast._byKey) : _byKey != cast._byKey) return false;

        return (isEnd() && cast.isEnd()) || _iter == cast._iter;
    }

private:
    tstr<tnchain> _ownIter; // _ownIter shouldn't be null always.
    iter _iter;
    const K& _byKey;
};
