#include "memlite/pool/memoryHaver.hpp"

namespace nm {

    NM_DEF_ME(memoryHaver)

    nbool me::isFull() const { return len() >= size(); }

    nbool me::isCapable() const { return len() < size(); }
}
