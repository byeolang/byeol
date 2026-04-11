#include "core/loader/pod/cpp/cppPodLoading.hpp"

namespace by {

    BY_DEF_ME(cppPodLoading)

    static constexpr const nchar* ENTRYPOINT_NAME = "byeol_bridge_cpp_entrypoint";
    static srcs dummySrcs;

    nbool me::parse(errReport& rpt, pod& pak) {
        enablesZone zone(rpt.isNoisy());
        tstr<srcs> ret(dummySrcs);
        for(const std::string& path: _getPaths()) {
            ncnt cnt = rpt.len();
            // With the current implementation, it is not yet possible to create an srcs
            // object for a C++ class.
            // ret.r variables won't be assigned to new data till this procedure has done.
            if(!_loadLibs(rpt, pak)) {
                _tryDump(rpt, pak, cnt);
                pak.getShares().rel();
                return BY_E("couldn't load c++ library at %s", path), false;
            }
        }

        return true;
    }

    nbool me::_loadLibs(errReport& rpt, pod& pak) {
        // TODO: use 'rpt' variable.
        for(const std::string& path: _getPaths()) {
            dlib lib = dlib(path);
            auto res = lib.load(); // `res` evaluated as true when it has an error.
            WHEN(res) .err("couldn't open %s pod: %d", path, res.get()).ret((rel(), false));

            auto info = lib.accessFunc<entrypointFunc>(ENTRYPOINT_NAME);
            WHEN(!info.has())
                .err("couldn't access entrypoint of %s pod: %d", path, info.getErr()).ret((rel(), false));

            bicontainable& shares = pak.getShares();
            ncnt prevLen = shares.len();
            (*info)(&shares);
            if(shares.len() <= prevLen) {
                BY_W("pod returns no origin object.");
                lib.rel();
            }

            _dlibs.push_back(std::move(lib)); // don't close yet.
            BY_I("pod[%s] loads origins from %s", getName(), path);
        }

        BY_I("pod[%s] origins loaded.", getName());
        return true;
    }

    void me::rel() {
        _dlibs.clear();
        super::rel();
    }

    const std::string& me::getName() const {
        static std::string inner = "cpp";
        return inner;
    }
} // namespace by
