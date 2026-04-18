#include "frontend/cli.hpp"
#include <algorithm>
using namespace by;

namespace by {

    BY(DEF_ME(cli))

    me::cli(): me(*new errReport()) {}

    me::cli(errReport& rpt): super(*new errReport()), _stacker(*this) {}

    namespace {
        std::string _joinString(const flagArgs& v) {
            std::string ret;
            nint first = 1;
            for(const auto& s: v)
                ret += (first-- ? "" : ", ") + s;
            return ret;
        }

        programRes _reportUnknownFlags(programRes& res, const flagArgs& remains) {
            auto options = _joinString(remains);
            res.rpt->add(nerr::newErr(UNKNOWN_OPTION, &options));
            res.rpt->log();
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

        strings _refineFlagArgs(flagStrs& a) {
            for(nidx n = a.len() - 1; n >= 0; n--)
                if(_isWhiteSpace(a[n].get())) a.del(n);

            strings ret;
            for(const auto& f: a)
                ret.push_back(f.get());
            return ret;
        }
    }

    interpreter& me::getInterpreter() { return _interpreter; }

    starter& me::getStarter() { return _starter; }

    programRes me::_onWork() {
        flagStrs& a = getTask() OR.ret(programRes{errReport(), BY_INDEX_ERROR});
        strings converted = _refineFlagArgs(a);

        _interpreter.rel();
        _starter.rel();
        programRes ret{getReport(), 0};

        auto evalRes = _stacker.take(converted);
        WHEN(evalRes == flag::EXIT_PROGRAM) .ret(ret);
        WHEN(converted.size() > 0) .ret(_reportUnknownFlags(ret, converted));
        // apply flags after evaluation:
        _interpreter.setReport(*ret.rpt).setFlag(getFlag());
        _starter.setFlag(starter::DUMP_ON_EX).setReport(*ret.rpt);

        {
            defaultSigZone<interpreter> zone(_interpreter);
            _interpreter.work();
        }

        if(!_interpreter.isVerified()) {
            ret.res = -1;
            return ret;
        }

        str res;
        {
            defaultSigZone<starter> zone(_starter);
            res = _starter.setTask(_interpreter.getSubPod()).work();
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
} // namespace by
