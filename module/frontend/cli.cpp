#include "frontend/cli.hpp"
#include <algorithm>
using namespace by;

namespace by {

    BY(DEF_ME(cli))

    namespace {
        std::string _joinString(const flagArgs& v) {
            std::string ret;
            nint first = 1;
            for(const nStr& s: v)
                ret += (first-- ? "" : ", ") + s.get();
            return ret;
        }

        programRes _reportUnknownFlags(programRes& res, const flagArgs& remains) {
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
            for(nidx n = a.len() - 1; n >= 0; n--)
                if(_isWhiteSpace(a[n].get())) a.del(n);
        }
    }

    programRes me::_onWork() {
        flagArgs& a = getTask() OR.ret(programRes{errReport(), BY_INDEX_ERROR});
        _refineFlagArgs(a);

        interpreter ip;
        programRes ret{errReport(true) /* it's noisy now */, 0};
        ip.setReport(ret.rpt).setFlag(interpreter::DEFAULT);
        starter s;
        s.setFlag(starter::DUMP_ON_EX);

        auto evalRes = _evalArgs(ip, a, s, ret.rpt);
        WHEN(evalRes == flag::EXIT_PROGRAM) .ret(ret);
        WHEN(a.len() > 0) .ret(_reportUnknownFlags(ret, a));

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
        while(a.len() > 0) {
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