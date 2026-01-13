/// @file
#include "core/builtin/container/iter/uiterable.hpp"

/** @ingroup core
 *  nested class of tucontainable.hpp
 */
class iteration: public instance, public iterable {
    BY(ADT(iteration, instance))
    friend class iter;

public:
    iteration(nbool isReversed): _isReversed(isReversed) {}

public:
    using iterable::isFrom;

    nbool isFrom(const tucontainable& rhs) const override { return this->getContainer() == &rhs; }

    nbool isReversed() const override { return _isReversed; }

private:
    nbool _isReversed;
};
