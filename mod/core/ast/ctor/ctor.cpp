#include "ctor.hpp"

namespace nm {
    NM(DEF_ME(ctor))

    me::ctor(const params& ps): super(ps, nullptr) {}

    str me::run(const args& a) {
        str ret = (node*) getRet()->clone();

        a.setMe(*ret);
        return super::run(a);
    }
}
