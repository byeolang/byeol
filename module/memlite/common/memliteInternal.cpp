#include "memliteInternal.hpp"
#include "memlite/binder/binder.hpp"
#include "memlite/interface/id.hpp"
#include "memlite/interface/instancer.hpp"

namespace by {
    BY(DEF_ME(memliteInternal))

    void me::setBinderItsId(binder& me, const id& newId) { me._itsId = newId; }

    life* me::getBindTag(binder& me) { return me._getBindTag(); }

    void me::setId(instance& me, const id& newId) { me._setId(newId); }

    id me::getId(instance& me) { return me._id; }

    void me::rel(binder& me) { me._itsId.rel(); }

    void* me::new1(size_t sz) { return instancer::get() TO(_new1(sz)); }

    void me::del(void* pt, size_t sz) { instancer::get() TO(_del(pt, sz)); }
}
