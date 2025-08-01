#include "memlite/interface/id.hpp"

namespace by {

    BY_DEF_ME(id)

    id::id(nint newTagN, nint newChkN, nuint newSerial): tagN(newTagN), chkN(newChkN), serial(newSerial) {}

    id::id(): tagN(BY_INDEX_ERROR), chkN(BY_INDEX_ERROR), serial(0) {}

    nbool me::operator==(const id& rhs) const { return tagN == rhs.tagN && chkN == rhs.chkN && serial == rhs.serial; }

    nbool me::operator!=(const id& rhs) const { return !operator==(rhs); }

    void me::rel() {
        tagN = chkN = BY_INDEX_ERROR;
        serial = 0;
    }

    nbool me::isHeap() const { return chkN >= 0; }

    nbool me::isValid() const { return tagN >= 0; }
}
