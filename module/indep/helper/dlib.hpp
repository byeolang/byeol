/// @file
#pragma once

#include "indep/common.hpp"
#include "indep/def.hpp"
#include "indep/macro.hpp"
#include "tres.inl"
#ifdef BY_BUILD_PLATFORM_IS_WINDOWS
#    include <windows.h> // for dll loading
#endif

namespace by {

#ifdef BY_BUILD_PLATFORM_IS_WINDOWS
    typedef HMODULE dlibHandle;
#else
    typedef void* dlibHandle;
#endif
    template <typename F> using tmayFunc = tres<F, std::string>;

    /** @ingroup indep
     *  @brief Dynamic library loading and management class
     *  @details Abbreviation for "dynamic loading for library". Handles platform-independent
     *  dynamic loading. Can load libraries into memory and find functions to convert them
     *  into function pointers.
     *
     *  Usage follows these steps:
     *  1. Create dlib object
     *  2. Specify the location of the library to load
     *  3. Find desired function by name and get it as function pointer
     *
     *  @remark Requires familiarity with tmay
     *  Uses @ref tmay, so it's recommended to familiarize yourself with tmay beforehand.
     *
     *
     *  @remark RAII idiom
     *  dlib is implemented using RAII idiom. When the instance is destroyed, function
     *  pointers returned externally become unusable.
     *
     *  @section usage Usage
     *  Example showing the complete process of loading a dynamic library and calling a function.
     *  Note that `rel()` explicitly releases resources. When an error occurs, the comma
     *  operator `(rel(), false)` is used to clean up the dlib object before returning:
     *  @code
     *      dlib lib = dlib(path); // Performs steps 1 and 2 simultaneously
     *      auto res = lib.load(); // `res` evaluated as true when it has an error
     *      WHEN(res).err("couldn't open %s pod: %d", path, res.get())
     *               .ret((rel(), false));
     *      // Release resources with rel() first, then return false via comma operator
     *
     *      typedef void (*entrypointFunc)(bicontainable*);
     *      constexpr const nchar* ENTRYPOINT_NAME = "byeol_bridge_cpp_entrypoint";
     *      auto info = lib.accessFunc<entrypointFunc>(ENTRYPOINT_NAME); // Result as tmay
     *      WHEN(!info.has()) // Checking result with tmay's has()
     *          .err("couldn't access entrypoint of %s pod: %d", path, info.getErr())
     *          .ret((rel(), false));
     *
     *      (*info)(&tray); // If function is successfully retrieved, it can be called
     *
     *      // Memory is automatically released when lib is destroyed
     *  @endcode
     */
    class _nout dlib {
        BY(ME(dlib))

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
        void setPath(const std::string* it) BY_SIDE_FUNC(setPath);

        /**
         * @brief Loads dynamic library into memory
         * @return Empty tmay on success, or error message on failure
         * @note Uses LoadLibraryA on Windows, dlopen(RTLD_LAZY) on POSIX.
         *       Automatically calls rel() on failure.
         */
        tmay<std::string> load();

        nbool isLoaded() const;

        /**
         * @brief Accesses function by name and returns typed function pointer
         * @return tres containing function pointer on success, or error message on failure
         * @note Library must be loaded first. Uses GetProcAddress on Windows, dlsym on POSIX.
         *       Automatically calls rel() on failure.
         */
        template <typename F> tmayFunc<F> accessFunc(const std::string& name) { return accessFunc<F>(name.c_str()); }

        template <typename F> tmayFunc<F> accessFunc(const std::string* it) BY_SIDE_FUNC(accessFunc<F>);

        template <typename F> tmayFunc<F> accessFunc(const nchar* name) {
            auto&& res = _accessFunc(name);
            return res.has() ? tmayFunc<F>((F&) *res) : tmayFunc<F>(res.getErr());
        }

        void rel();

    private:
        tmayFunc<void*> _accessFunc(const nchar* name);

    private:
        std::string _path;
        dlibHandle _handle;
    };

    typedef std::vector<dlib> dlibs;
} // namespace by
