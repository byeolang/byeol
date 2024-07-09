#include "cppPackLoading.hpp"

namespace nm {

    NM_DEF_ME(cppPackLoading)

    static constexpr const nchar* ENTRYPOINT_NAME = "namu_bridge_cpp_entrypoint";
    static srcs dummySrcs;

    tstr<srcs> me::parse(errReport& rpt, bicontainable& tray) {
        tstr<srcs> ret(dummySrcs);
        for(const std::string& path : _getPaths()) {
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
        libHandle newHandle = nullptr;
        for(const std::string& path : _getPaths()) {
#ifdef NM_BUILD_PLATFORM_IS_WINDOWS
            newHandle = LoadLibraryA(path.c_str());
#else
            newHandle = dlopen(path.c_str(), RTLD_LAZY);
#endif
            if(!newHandle) {
#ifdef NM_BUILD_PLATFORM_IS_WINDOWS
                NM_E("couldn't open %s slot: %d", path, GetLastError());
#else
                NM_E("couldn't open %s slot: %s", path, dlerror());
#endif
                goto FINALIZE;
            }

#ifdef NM_BUILD_PLATFORM_IS_WINDOWS
            entrypointFunc ep = (entrypointFunc) GetProcAddress(newHandle, ENTRYPOINT_NAME);
#else
            entrypointFunc ep = (entrypointFunc) dlsym(newHandle, ENTRYPOINT_NAME);
#endif
            if(!ep) {
#ifdef NM_BUILD_PLATFORM_IS_WINDOWS
                NM_E("couldn't locate entrypoint of %s slot: %d", path, GetLastError());
#else
                NM_E("couldn't locate entrypoint of %s slot: %s", path, dlerror());
#endif
                goto FINALIZE;
            }

            ep(&tray);
            if(tray.len() <= 0) {
                NM_W("slot returns no origin object.");
                goto FINALIZE;
            }

            _handles.push_back(newHandle); // don't close yet.
            NM_I("slot[%s] loads origins from %s", getName(), path);
        }

        NM_I("slot[%s] origins loaded.", getName());
        return true;

FINALIZE:
        if(newHandle)
#ifdef NM_BUILD_PLATFORM_IS_WINDOWS
            FreeLibrary(newHandle);
#else
            dlclose(newHandle);
#endif
        me::rel();
        return false;
    }

    void me::rel() {
        for (libHandle e : _handles)
            if (e)
#ifdef NM_BUILD_PLATFORM_IS_WINDOWS
                FreeLibrary(e);
#else
                dlclose(e);
#endif
        _handles.clear();

        super::rel();
    }

    const std::string& me::getName() const {
        static std::string inner = "cpp";
        return inner;
    }
}
