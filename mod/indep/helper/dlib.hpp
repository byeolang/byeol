#pragma once

#include "../common.hpp"
#include "../def.hpp"
#include "../macro.hpp"

namespace nm {

    template <typename F> struct funcInfo {
        F func;
        const char* errMsg;
    };

    // dlib:
    //  dynamic library class.
    class _nout dlib {
        NM(ME(dlib))

    public:
#ifdef NM_BUILD_PLATFORM_IS_WINDOWS
        typedef HMODULE dlibHandle;
#else
        typedef void* dlibHandle;
#endif

    public:
        dlib();
        dlib(const char* path);
        dlib(const std::string& path);
        dlib(const me& rhs);
        virtual ~dlib();

    public:
        me& operator=(const me& rhs);

    public:
        const std::string& getPath() const;
        void setPath(const std::string& path);

        /// load dynamic library with given path.
        /// @return nullptr if it's success. or return error msg.
        const nchar* load();
        nbool isLoaded() const;

        /// access function and get address of it inside library.
        /// @return `func` as nullptr if it failed or return `errMsg` as nullptr if it's success.
        template <typename R> funcInfo<R> accessFunc(const std::string& name) {
            return accessFunc<R>(name.c_str());
        }

        template <typename R> funcInfo<R> accessFunc(const nchar* name) {
            auto&& res = _accessFunc(name);
            return funcInfo<R>{(R) res.func, res.errMsg};
        }

        void rel();

    private:
        funcInfo<void*> _accessFunc(const nchar* name);

    private:
        std::string _path;
        dlibHandle _handle;
    };

    typedef std::vector<dlib> dlibs;
} // namespace nm
