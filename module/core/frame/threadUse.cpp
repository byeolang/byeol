#include "core/frame/threadUse.hpp"

namespace by {
    BY(DEF_ME(threadUse))

    me::threadUse(): me(*new errReport())  {}

    me::threadUse(const errReport& new1): _thr(new thread(new1)), _prev(thread::get()) { use(); }

    me::~threadUse() { rel(); }

    thread& me::get() { return *_thr; }

    void me::use() { thread::set(_thr.get()); }

    void me::rel() { thread::set(_prev.get()); }
}
