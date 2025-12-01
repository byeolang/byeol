#include "frontend/cli.hpp"
using namespace by;

namespace by {

    BY(DEF_ME(cli))

    namespace {
        std::string _joinString(const std::vector<std::string>& v) {
            std::string ret;
            nint first = 1;
            for(const std::string& s: v)
                ret += (first-- ? "" : ", ") + s;
            return ret;
        }

        nbool _reportUnknownFlags(const flagArgs& remains) {
            err("unknown flags: " + _joinString(remains)).log();
            return false;
        }
    }

    nint me::eval(flagArgs& a) {
        interpreter ip;
        errReport rpt(true); // it's noisy now.
        ip.setReport(rpt).setFlag(interpreter::DEFAULT);
        starter s;
        s.setFlag(starter::DUMP_ON_EX);

        for(const auto& op: getFlags()) {
            if(op->take(ip, s, *this, a) == flag::EXIT_PROGRAM) return true;
            if(a.size() <= 0) break;
        }
        if(a.size() > 0) return _reportUnknownFlags(a);

        {
            defaultSigZone<interpreter> zone(ip);
            ip.work();
        }

        if(!ip.isVerified()) return -1;

        str res;
        {
            defaultSigZone<starter> zone(s);
            res = s.setTask(ip.getSubPack()).work();
        }

        nint ret = 0;
        if(res) {
            if(res->isSub<nInt>()) ret = *res->cast<nint>();
            else if(res->isSub<baseErr>()) ret = -1;
        }

#ifdef __EMSCRIPTEN__
        // I don't know the detail reason but if user doesn't put '\n' at the
        // end, the line won't be printed.
        std::cout << "\n";
#endif
        if(rpt) // has an err
            ret = -1;
        return ret;
    }

    const flags& cli::getFlags() const {
        static flags inner;
        if(inner.size() <= 0) {
            inner.push_back(tstr<flag>(new helpFlag()));
            inner.push_back(tstr<flag>(new fileFlag()));
            inner.push_back(tstr<flag>(new logStructureFlag()));
            inner.push_back(tstr<flag>(new bufferSrcFlag()));
            inner.push_back(tstr<flag>(new verboseFlag()));
            inner.push_back(tstr<flag>(new verFlag()));
        }

        return inner;
    }
} // namespace by
