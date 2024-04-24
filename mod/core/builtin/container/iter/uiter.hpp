#include "uiteration.hpp"

// nested class of tucontainable.hpp:
//  this file allows to be refered by 'tucontainable.hpp' file only.
class iter : public iterable, public clonable, public typeProvidable {
    NAMU(CLASS(iter))
    friend class iteration;
    template <typename E, typename TACTIC> friend class tnarr;
    template <typename K, typename V, typename defaultContainer> friend class tnchain;

public:
    iter() { _nextToMatchParamType(); }
    explicit iter(iteration* newStep): _step(newStep) { _nextToMatchParamType(); }
    iter(const me& rhs) { _assign(rhs); }

public:
    me operator+(ncnt step);
    me& operator++();
    me operator++(int);
    me& operator+=(ncnt step);
    T& operator*();
    T* operator->();
    const T& operator*() const NAMU_UNCONST_FUNC(operator*())
    const T* operator->() const NAMU_UNCONST_FUNC(operator->())
    explicit operator nbool() const;

public:
    nbool isFrom(const tucontainable& it) const override;
    nbool isEnd() const override;
    ncnt next(ncnt step) override;

    using iterable::get;
    T& get() override;

    template <typename E>
    E& get() {
        return get().template cast<E>();
    }

    tucontainable<T>& getContainer() override;
    const tucontainable<T>& getContainer() const NAMU_UNCONST_FUNC(getContainer());

    node& getOwner();
    const node& getOwner() const NAMU_UNCONST_FUNC(getOwner())

private:
    me& _assign(const me& rhs);
    nbool _onSame(const typeProvidable& rhs) const override;

    /// iterates until points to object of compatible type to given parameterized type T.
    /// iter should be alwyas stable state which points to object of proper type.
    void _nextToMatchParamType();

protected:
    tstr<iteration> _step;
};
