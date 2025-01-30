#include <type_traits>

#include "uiteration.hpp"

// nested class of tucontainable.hpp:
//  this file allows to be refered by 'tucontainable.hpp' file only.
class iter: public iterable, public clonable, public typeProvidable {
    NM(CLASS(iter))
    friend class iteration;
    template <typename E, typename TACTIC> friend class tnarr;
    template <typename K, typename V, typename defaultContainer> friend class tnchain;

public:
    iter() { _nextToMatchParamType(); }

    explicit iter(iteration* newStep): _iteration(newStep) { _nextToMatchParamType(); }

    iter(const me& rhs) { _assign(rhs); }

public:
    me operator+(ncnt step);
    me& operator++();
    me operator++(int);
    me& operator+=(ncnt step);
    R operator*();
    const R operator*() const NM_CONST_FUNC(operator*())
    typename std::remove_reference<R>::type* operator->();
    const typename std::remove_reference<R>::type* operator->() const NM_CONST_FUNC(operator->())
    me& operator=(const me& rhs);
    explicit operator nbool() const;

public:
    nbool isReversed() const override;

    void rel() override;

    nbool isFrom(const tucontainable& it) const override;
    nbool isEnd() const override;
    ncnt next(ncnt step) override;
    ncnt stepForward(ncnt step) override;
    ncnt stepBackward(ncnt step) override;

    using iterable::get;
    R get() override;

    template <typename E> E& get() { return get().template cast<E>(); }

    tucontainable<T, R>& getContainer() override;
    const tucontainable<T, R>& getContainer() const NM_CONST_FUNC(getContainer())

private:
    me& _assign(const me& rhs);
    nbool _onSame(const typeProvidable& rhs) const override;

    /// iterates until points to object of compatible type to given parameterized type T.
    /// iter should be alwyas stable state which points to object of proper type.
    void _nextToMatchParamType();
    ncnt _step(std::function<ncnt(void)> closure, ncnt step);

protected:
    tstr<iteration> _iteration;
};
