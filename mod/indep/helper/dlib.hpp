#pragma once

#include "indep/common.hpp"
#include "indep/def.hpp"
#include "indep/macro.hpp"
#include "tres.inl"
#ifdef NM_BUILD_PLATFORM_IS_WINDOWS
#    include <windows.h> // for dll loading
#endif

namespace nm {

#ifdef NM_BUILD_PLATFORM_IS_WINDOWS
    typedef HMODULE dlibHandle;
#else
    typedef void* dlibHandle;
#endif
    template <typename F> using tmayFunc = tres<F, std::string>;

    // dlib:
    //  dynamic library class.
    class _nout dlib {
        NM(ME(dlib))

    public:
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
        /// @return empty may object if it's success. or return error msg.
        tmay<std::string> load();
        nbool isLoaded() const;

        /// access function and get address of it inside library.
        /// @return `func` as nullptr if it failed or return `errMsg` as nullptr if it's success.
        template <typename F> tmayFunc<F> accessFunc(const std::string& name) {
            return accessFunc<F>(name.c_str());
        }

        template <typename F> tmayFunc<F> accessFunc(const nchar* name) {
            auto&& res = _accessFunc(name);
            return res.has() ? tmayFunc<F>((F) res.get()) : tmayFunc<F>(res.getErr());
        }

        void rel();

    private:
        tmayFunc<void*> _accessFunc(const nchar* name);

    private:
        std::string _path;
        dlibHandle _handle;
    };

    typedef std::vector<dlib> dlibs;
} // namespace nm
