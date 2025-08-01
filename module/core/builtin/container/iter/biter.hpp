/// @file
#include "core/builtin/container/iter/biteration.hpp"

/// @ingroup core
/// @brief Bidirectional iterator for key-value containers
/// @details Iterator implementation for bidirectional containers with key-value pair access.
/// nested class of tbicontainable.hpp:
/// this file allows to be refered by 'tbicontainable.hpp' file only.
class iter: public iterable, public clonable, public typeProvidable {
    BY(CLASS(iter))
    friend class iteration;
    template <typename K1, typename V1, typename defaultContainer1> friend class tnchain;
    template <typename K1, typename V1, typename TACTIC> friend class tnmap;

public:
    explicit iter();
    explicit iter(iteration* newStep);

    iter(const me& rhs);

public:
    me operator+(ncnt step);
    me& operator++();
    me operator++(int);
    me& operator+=(ncnt step);
    V& operator*();
    V* operator->();
    const V& operator*() const BY_CONST_FUNC(operator*())
    const V* operator->() const BY_CONST_FUNC(operator->())
    me& operator=(const me& rhs);
    explicit operator nbool() const;

public:
    nbool isReversed() const override;

    /// once rel(), an iterator enters a state where it can never be reused again.
    void rel() override;

    using iterable::isFrom;
    nbool isFrom(const tbicontainable& it) const override;

    nbool isEnd() const override;

    ncnt next(ncnt step) override;
    ncnt prev(ncnt step) override;
    ncnt stepForward(ncnt step) override;
    ncnt stepBackward(ncnt step) override;

    const K* getKey() const override;

    template <typename E> const E* getKey() const { return getKey().template cast<E>(); }

    using iterable::getVal;
    V* getVal() override;

    template <typename E> E* getVal() { return getVal() TO(template cast<E>()); }

    using iterable::setVal;
    void setVal(const V& new1) override;

    using iterable::getContainer;
    tbicontainable<K, V>* getContainer() override;

private:
    me& _assign(const me& rhs);
    nbool _onSame(const typeProvidable& rhs) const override;

    /// iterates until points to object of compatible type to given parameterized type T.
    /// iter should be alwyas stable state which points to object of proper type.
    void _nextToMatchParamType(typename iterable::iterationType type);
    ncnt _step(typename iterable::iterationType type, ncnt step);
    ncnt _iterate(typename iterable::iterationType type);

protected:
    tstr<iteration> _iteration;
};
