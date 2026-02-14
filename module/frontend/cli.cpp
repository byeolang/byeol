#include "frontend/cli.hpp"
#include <algorithm>
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

        me::programRes _reportUnknownFlags(me::programRes& res, const flagArgs& remains) {
            auto options = _joinString(remains);
            res.rpt.add(nerr::newErr(UNKNOWN_OPTION, &options));
            res.rpt.log();
            res.res = -1;
            return res;
        }
    }

    namespace {
        nbool _isWhiteSpace(const std::string& it) {
            for(nchar c: it)
                if(!std::isspace(static_cast<unsigned char>(c))) return false;

            return true;
        }

        void _refineFlagArgs(flagArgs& a) {
            for(nidx n = a.size() - 1; n >= 0; n--)
                if(_isWhiteSpace(a[n])) a.erase(a.begin() + n);
        }
    }

    me::programRes me::eval(flagArgs& a) {
        _refineFlagArgs(a);

        interpreter ip;
        programRes ret{errReport(true) /* it's noisy now */, 0};
        ip.setReport(ret.rpt).setFlag(interpreter::DEFAULT);
        starter s;
        s.setFlag(starter::DUMP_ON_EX);

        auto evalRes = _evalArgs(ip, a, s, ret.rpt);
        WHEN(evalRes == flag::EXIT_PROGRAM) .ret(ret);
        WHEN(a.size() > 0) .ret(_reportUnknownFlags(ret, a));

        {
            defaultSigZone<interpreter> zone(ip);
            ip.work();
        }

        if(!ip.isVerified()) {
            ret.res = -1;
            return ret;
        }

        str res;
        {
            defaultSigZone<starter> zone(s);
            res = s.setTask(ip.getSubPack()).work();
        }

        if(res) {
            if(res->isSub<nInt>()) ret.res = *res->cast<nint>();
            else if(res->isSub<baseErr>()) ret.res = -1;
        }

#ifdef __EMSCRIPTEN__
        // I don't know the detail reason but if user doesn't put '\n' at the
        // end, the line won't be printed.
        std::cout << "\n";
#endif
        return ret;
    }

    flag::res me::_evalArgs(interpreter& ip, flagArgs& a, starter& s, errReport& rpt) {
        while(a.size() > 0) {
            flag::res r = flag::NOT_MATCH;
            for(const auto& op: getFlags()) {
                r = op->take(ip, s, *this, a, rpt);
                WHEN(r == flag::EXIT_PROGRAM) .ret(r);
                if(r == flag::MATCH) break;
            }
            WHEN(r == flag::NOT_MATCH) .ret(r); // if all flags couldn't match the first argument.
        }
        return flag::MATCH;
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
