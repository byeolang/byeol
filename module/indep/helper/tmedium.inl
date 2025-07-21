#include "indep/helper/tmedium.hpp"

namespace nm {

#define ME tmedium<T>
#define TEMPLATE template <typename T>

    TEMPLATE
    ME::tmedium(T* value): _value(value) {}

    TEMPLATE
    ME::tmedium(const T* value): _value((T*) value) {}

    TEMPLATE
    ME::operator T&() { return *get(); }

    TEMPLATE
    ME::operator const T&() const { return *get(); }

    TEMPLATE
    ME::operator nbool() const { return has(); }

    TEMPLATE
    nbool ME::has() const { return _value; }

    TEMPLATE
    T* ME::get() { return _value; }

    TEMPLATE
    const T* ME::get() const { return _value; }

#undef TEMPLATE
#undef ME
}
