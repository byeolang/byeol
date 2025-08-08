#include "frontend/cli.hpp"
using namespace nm;

namespace nm {

    NM(DEF_ME(cli))

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

    nbool me::run(flagArgs& a) {
        _res = -1;
        interpreter ip;
        errReport rpt;
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

        if(!ip.isVerified()) return _res;

        str res;
        {
            defaultSigZone<starter> zone(s);
            res = s.setTask(ip.getSubPack()).work();
        }
        _res = 0;
        if(res) {
            if(res->isSub<nInt>()) _res = *res->cast<nint>();
            else if(res->isSub<baseErr>()) _res = -1;
        }

#ifdef __EMSCRIPTEN__
        // I don't know the detail reason but if user doesn't put '\n' at the
        // end, the line won't be printed.
        std::cout << "\n";
#endif
        if(rpt) // has an err
            _res = -1;
        return _res;
    }

    nint me::getRes() const { return _res; }

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
} // namespace nm

int main(int argc, char* argv[]) {
    cli ep;
    flagArgs a;
    for(int n = 1; n < argc; n++)
        a.push_back(argv[n]);

    ep.run(a);
    return ep.getRes();
}
