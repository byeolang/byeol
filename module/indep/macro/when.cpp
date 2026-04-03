#include "indep/macro/when.hpp"

namespace by {
    BY(DEF_ME(__WHEN_OBJECT__))

    const me& me::get() {
        static me inner;
        return inner;
    }

    const me& me::run(const std::function<void(void)>& closure) const {
        closure();
        return *this;
    }

    void me::ret() const {}

    void me::crash() const { abort(); }
}
