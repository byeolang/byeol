#include "filters.hpp"

namespace nm {
    NAMU(DEF_ME(filters))

    me::~filters() { rel(); }

    ncnt me::len() const {
        return _arr.size();
    }

    void me::rel() {
        while(len() > 0)
            del();
    }

    void me::add(const filterable* new1) {
        _arr.push_back(new1);
    }

    void me::del() {
        if(len() <= 0) return;

        auto e = _arr.back();
        _arr.pop_back();
        delete e;
    }

    std::string me::filt(logLv::level lv, const nchar* tag, const std::string& msg) const {
        std::string ret = msg;
        for(auto* f : _arr)
            ret = f->filt(lv, tag, ret);
        return ret;
    }
}
