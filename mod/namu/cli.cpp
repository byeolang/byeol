#include "cli.hpp"
using namespace namu;

namespace namu {

    NAMU(DEF_ME(cli))

    nbool me::run(flagArgs& a) {
        _res = -1;
        interpreter ip;
        errReport rpt;
        ip.setReport(rpt);

        for(const auto& op : getFlags()) {
            op->take(ip, *this, a);

            if(a.size() <= 0)
                break;
        }

        ip.work();

        if(!ip.isVerified())
            return _res;

        starter s;
        str res = s.run(args((baseObj&) ip.getSubPack()));
        _res = 0;
        if(res) {
            if(res->isSub<nInt>())
                _res = res->cast<nint>();
            else if(res->isSub<err>())
                _res = -1;
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

    nint me::getRes() const {
        return _res;
    }

    const flags& cli::getFlags() const {
        static flags inner;
        if(inner.size() <= 0) {
            inner.push_back(tstr<flag>(new helpFlag()));
            inner.push_back(tstr<flag>(new fileFlag()));
            inner.push_back(tstr<flag>(new logStructureFlag()));
            inner.push_back(tstr<flag>(new bufferSrcFlag()));
            inner.push_back(tstr<flag>(new verboseFlag()));
        }

        return inner;
    }
}

int main(int argc, char* argv[]) {
    cli ep;
    flagArgs a;
    for(int n=1; n < argc ;n++)
        a.push_back(argv[n]);

    ep.run(a);
    return ep.getRes();
}
