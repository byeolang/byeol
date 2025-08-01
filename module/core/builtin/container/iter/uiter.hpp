/// @file
#include <type_traits>

#include "core/builtin/container/iter/uiteration.hpp"

/// @ingroup core
/// @brief Unidirectional iterator for sequence containers
/// @details Iterator implementation for unidirectional containers with element access.
/// nested class of tucontainable.hpp:
///  this file allows to be refered by 'tucontainable.hpp' file only.
class iter: public iterable, public clonable, public typeProvidable {
    BY(CLASS(iter))
    friend class iteration;
    template <typename E, typename TACTIC> friend class tnarr;
    template <typename K, typename V, typename defaultContainer> friend class tnchain;

public:
    explicit iter();
    explicit iter(iteration* newStep);
    iter(const me& rhs);

public:
    me operator+(ncnt step);
    me& operator++();
    me operator++(int);
    me& operator+=(ncnt step);

    using iterable::operator*;
    RSquare operator*() override;

    typename std::remove_reference<R>::type operator->();
    const typename std::remove_reference<R>::type operator->() const BY_CONST_FUNC(operator->())
    me& operator=(const me& rhs);
    explicit operator nbool() const;

public:
    nbool isReversed() const override;

    /// once rel(), an iterator enters a state where it can never be reused again.
    void rel() override;

    using iterable::isFrom;
    nbool isFrom(const tucontainable& it) const override;

    nbool isEnd() const override;
    ncnt next(ncnt step) override;
    ncnt prev(ncnt step) override;
    ncnt stepForward(ncnt step) override;
    ncnt stepBackward(ncnt step) override;

    using iterable::get;
    R get() override;

    template <typename E> E* get() { return get() TO(template cast<E>()); }

    tucontainable<T, R, RSquare>* getContainer() override;
    const tucontainable<T, R, RSquare>* getContainer() const BY_CONST_FUNC(getContainer())

private:
    me& _assign(const me& rhs);
    nbool _onSame(const typeProvidable& rhs) const override;

    /// iterates until points to object of compatible type to given parameterized type T.
    /// iter should be alwyas stable state which points to object of proper type.
    void _nextToMatchParamType(typename iterable::iterationType type);
    ncnt _step(typename iterable::iterationType type, ncnt step);
    ncnt _iterate(typename iterable::iterationType type, ncnt step);

protected:
    tstr<iteration> _iteration;
};
