#include "clog/filter/filters.hpp"

namespace nm {
    NM(DEF_ME(filters))

    me::~filters() { rel(); }

    ncnt me::len() const { return _arr.size(); }

    void me::rel() {
        while(len() > 0)
            del();
    }

    void me::add(const filterable* new1) { _arr.push_back(new1); }

    void me::del() {
        if(len() <= 0) return;

        auto e = _arr.back();
        _arr.pop_back();
        delete e;
    }

    nbool me::filt(errLv::level lv, const std::string& tag) const {
        for(auto* f: _arr)
            if(!f->filt(lv, tag)) return false;
        return true;
    }
} // namespace nm
