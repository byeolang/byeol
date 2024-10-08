#include "err.hpp"
#include "../../bridge/cpp/tbridger.hpp"

namespace nm {
    NM(DEF_ME(err))

    me::err(const nStr& msg): super(logLv::ERR), _msg(msg) {}

    me::err(): super(logLv::ERR) {}

    nbool me::operator==(const super& rhs) const {
        const me& cast = rhs.cast<me>() orRet1 false;
        if(!_msg || !cast._msg) return false;

        return &_msg.get() == &cast._msg.get();
    }

    void me::log() const {
        using platformAPI::foreColor;
        logger::get().logFormatBypass("%serr: %s%s\n", foreColor(LIGHTRED).c_str(),
            foreColor(LIGHTGRAY).c_str(), _msg->get().c_str());
    }

    const std::string& me::getMsg() const { return _msg->get(); }

    const baseObj& me::getOrigin() const {
        static tbaseObjOrigin<me> org(
            tbridger<me>::ctor<nStr>().extend(me::super::makeSubs()).subs());
        const baseObj& supers = super::getOrigin();
        return &supers == this ? org : supers;
    }
} // namespace nm
