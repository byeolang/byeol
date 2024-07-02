#include "biterable.hpp"

// nested class of tbicontainable.hpp
class iteration : public instance, public iterable {
    NM(ADT(iteration, instance))
    friend class iter;

public:
    nbool isFrom(const tbicontainable& rhs) const override {
        return &this->getContainer() == &rhs;
    }
};
