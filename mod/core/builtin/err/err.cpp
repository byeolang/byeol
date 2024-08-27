#include "err.hpp"
#include "../../bridge/cpp/tbridger.hpp"

namespace nm {
    NM(DEF_ME(err))

    namespace {
        static baseObjOrigin org(/*TODO:*/ dumSrc::singletone(),
             tbridger<me>::ctor<nStr>().extend(me::super::makeSubs()).subs());
    }

    me::err(const nStr& msg): super(org), _msg(msg) {}

    me::err(): super(org) {}

    nbool me::operator==(const super& rhs) const {
        const me& cast = rhs.cast<me>();
        if(nul(cast)) return false;
        if(!_msg || !cast._msg) return false;

        return &_msg.get() == &cast._msg.get();
    }

    void me::log() const {
        using platformAPI::foreColor;
        logger::get().logFormatBypass("%serr: %s%s\n", foreColor(LIGHTRED).c_str(),
            foreColor(LIGHTGRAY).c_str(), _msg->get().c_str());
    }

    const std::string& me::getMsg() const { return _msg->get(); }
} // namespace nm
