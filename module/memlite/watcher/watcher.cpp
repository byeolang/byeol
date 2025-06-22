#include "memlite/watcher/watcher.hpp"

namespace nm {

    NM_DEF_ME(watcher, chunk)

    me::watcher(): chunk(sizeof(watchCell), false) {}

    watchCell* me::get(nidx n) { return (watchCell*) _get(n); }

    watchCell* me::get(id newId) {
        watchCell& got = get(newId.tagN) OR.ret(nullptr);

        id gotId = got.blk.getId();
        WHEN(gotId.tagN != newId.tagN)
            .warn("bindTag was corrupted! watchCell.id(%d.%d.%d) != id(%d.%d.%d)", gotId.tagN,
                gotId.chkN, gotId.serial, newId.tagN, newId.chkN, newId.serial)
            .ret(nullptr);
        WHEN(gotId.chkN != newId.chkN || gotId.serial != newId.serial)
            .ret(nullptr); // bindTag has been changed its instance to bind.

        return &got;
    }

    void* me::new1() {
        WHEN(isFull() && !_resize(size() * 2 + 1))
            .err("resize watcher failed! this damage system seriously !!!!")
            .ret(nullptr);

        watchCell& res = (watchCell*) super::new1() OR.ret(&res);

        ::new(&res.blk) bindTag(_genId(&res));
        return &res;
    }

    nbool me::del(void* used, ncnt sz) {
        watchCell& cell = *((watchCell*) used);
        cell.~watchCell();

        return super::del(used, sz);
    }

    id me::_genId(void* pt) const {
        static ncnt serial = 0;
        // watcher concern about bkl_n at Id. on the other hand, chunk is chkN.
        // eventually, if Instance was born from heap, first it take chkN from chunk when it borns.
        // and take tagN from watcher when user try to access its Block instance.
        return id(_getIdx(pt), NM_INDEX_ERROR, ++serial);
    }

    nidx me::_getIdx(void* it) const {
        WHEN(!has(*(instance*) it)).ret(-1); // "has" func will treat it as void*, too.

        nidx ret = ((nuchar*) it - _getHeap()) / getBlkSize();
        return ret;
    }
} // namespace nm
