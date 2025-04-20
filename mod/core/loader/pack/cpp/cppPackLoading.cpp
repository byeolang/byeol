#include "core/loader/pack/cpp/cppPackLoading.hpp"

namespace nm {

    NM_DEF_ME(cppPackLoading)

    static constexpr const nchar* ENTRYPOINT_NAME = "namu_bridge_cpp_entrypoint";
    static srcs dummySrcs;

    tstr<srcs> me::parse(errReport& rpt, bicontainable& tray) {
        enablesZone zone(true); // set clog enable temporary.
                                // because when importing modules failed, to leave related logs is
                                // very important.
        tstr<srcs> ret(dummySrcs);
        for(const std::string& path: _getPaths()) {
            // With the current implementation, it is not yet possible to create an srcs
            // object for a C++ class.
            // ret.r variables won't be assigned to new data till this procedure has done.
            if(!_loadLibs(rpt, tray)) {
                tray.rel();
                return NM_E("couldn't load c++ library at %s", path), ret;
            }
        }

        return ret;
    }

    nbool me::_loadLibs(errReport& rpt, bicontainable& tray) {
        // TODO: use 'rpt' variable.
        for(const std::string& path: _getPaths()) {
            dlib lib = dlib(path);
            const nchar* res = lib.load();
            WHEN(res).err("couldn't open %s slot: %d", path, res).ret((rel(), false));

            auto&& info = lib.accessFunc<entrypointFunc>(ENTRYPOINT_NAME);
            WHEN(info.errMsg)
                .err("couldn't access entrypoint of %s slot: %d", path, info.errMsg)
                .ret((rel(), false));

            info.func(&tray);
            if(tray.len() <= 0) {
                NM_W("slot returns no origin object.");
                lib.rel();
            }

            _dlibs.push_back(std::move(lib)); // don't close yet.
            NM_I("slot[%s] loads origins from %s", getName(), path);
        }

        NM_I("slot[%s] origins loaded.", getName());
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
} // namespace nm
