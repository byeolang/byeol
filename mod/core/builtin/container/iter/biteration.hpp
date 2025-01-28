#include "biterable.hpp"

// nested class of tbicontainable.hpp
class iteration: public instance, public iterable {
    NM(ADT(iteration, instance))
    friend class iter;

public:
    iteration(nbool isReversed): _isReversed(isReversed) {}

public:
    nbool isFrom(const tbicontainable& rhs) const override { return &this->getContainer() == &rhs; }

    nbool isReversed() const override { return _isReversed; }

private:
    nbool _isReversed;
};
