#include "core/frame/threadUse.hpp"

namespace by {
    BY(DEF_ME(threadUse))

    me::threadUse(): _thr(), _prev(thread::get())  { use(); }

    me::threadUse(const errReport& new1): _thr(new1), _prev(thread::get()) { use(); }

    me::~threadUse() { rel(); }

    thread& me::get() { return _thr; }

    void me::use() { thread::set(_thr); }

    void me::rel() { thread::set(_prev.get()); }
}
