#include "watcher.hpp"

namespace wrd {

    WRD_DEF_ME(watcher, chunk)

    me::watcher() : chunk(sizeof(watchCell), false) {}

    watchCell& me::operator[](widx n) { return get(n); }
    watchCell& me::operator[](id newId) { return get(newId); }

    watchCell& me::get(widx n) { return *(watchCell*)_get(n); }

    watchCell& me::get(id newId) {
        watchCell& got = get(newId.tagN);
        if(nul(got)) return nulOf<watchCell>();

        id gotId = got.blk.getId();
        if(gotId.tagN != newId.tagN) {
            WRD_W("bindTag was corrupted! watchCell.id(%d.%d.%d) != id(%d.%d.%d)",
                    gotId.tagN, gotId.chkN, gotId.serial, newId.tagN, newId.chkN, newId.serial);
            return nulOf<watchCell>();
        }
        if(gotId.chkN != newId.chkN || gotId.serial != newId.serial)
            // bindTag has been changed its instance to bind.
            return nulOf<watchCell>();

        return got;
    }

    void* me::new1() {
        if(isFull())
            if(!_resize(size()*2 + 1))
                return WRD_E("resize watcher failed! this damage system seriously !!!!"), nullptr;

        watchCell* res = (watchCell*)super::new1();
        if(!res)
            return res;

        ::new (&res->blk) bindTag(_genId(res));
        return res;
    }

    wbool me::del(void* used, wcnt sz) {
        watchCell& cell = *((watchCell*) used);
        cell.~watchCell();

        return super::del(used, sz);
    }

    id me::_genId(void* pt) const {
        static wcnt serial = 0;
        // watcher concern about bkl_n at Id. on the other hand, chunk is chkN.
        // eventually, if Instance was born from heap, first it take chkN from chunk when it borns.
        // and take tagN from watcher when user try to access its Block instance.
        return id(_getIdx(pt), WRD_INDEX_ERROR, ++serial);
    }

    widx me::_getIdx(void* it) const {
        if(!has(*(instance*)it)) // "has" func will treat it as void*, too.
            return -1;

        widx ret = ((wuchar*)it - _getHeap()) / getBlkSize();
        return ret;
    }
}
