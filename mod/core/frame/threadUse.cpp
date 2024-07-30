#include "threadUse.hpp"

namespace nm {
    NM(DEF_ME(threadUse))

    me::threadUse(): _thr() { use(); }

    me::threadUse(const errReport& new1): _thr(new1) { use(); }

    me::~threadUse() { rel(); }

    thread& me::get() { return _thr; }

    void me::use() { thread::set(_thr); }

    void me::rel() { thread::set(); }
}
