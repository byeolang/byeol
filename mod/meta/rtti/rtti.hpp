#pragma once

#include "../common.hpp"
#include <iostream>
#include "./type/adam.hpp"

namespace namu {

    struct _nout metaIf {
        typedef short yes;
        typedef char no;
    };

    template <typename T>
    struct tifTemplate : public metaIf {
        template <template<typename> class Template, typename X>
        static yes _foo(Template<X>*);
        static no _foo(...);

        static inline constexpr nbool is = sizeof(_foo((T*)0)) == sizeof(yes);
    };

    template <typename T, typename super> // is T is sub of super
    struct tifSub {
        static inline constexpr nbool is = std::is_base_of<super, T>::value;
    };

    template <typename T>
    struct taEmptyCan {
        typedef void is;
    };
    template <typename T, typename = void>
    struct tifHasSuperTypedef : public metaIf {
        static inline constexpr nbool is = false;
    };
    template <typename T>
    struct tifHasSuperTypedef<T, typename taEmptyCan<typename T::super>::is> : public metaIf {
        static inline constexpr nbool is = true;
    };

    template <typename T, nbool typedefsuper = tifHasSuperTypedef<T>::is>
    struct tadaptiveSuper : metaIf {
        typedef adam super;
    };
    template <typename T>
    struct tadaptiveSuper<T, true> {
        typedef typename T::super super;
    };

    template <typename T, nbool canMake = std::is_constructible<T>::value>
    struct tinstanceMaker {
        static void* make() { return nullptr; }
    };
    template <typename T>
    struct tinstanceMaker<T, true> {
        static void* make() { return new T(); }
    };

    struct _nout nameDemangler {
#ifndef NAMU_BUILD_PLATFORM_IS_WINDOWS
        static std::string demangle(const nchar* org) {
            nchar* demangled = nullptr;
            int status = 0;

            demangled = ::abi::__cxa_demangle(org, 0, 0, &status);
            std::string ret(demangled);

            free(demangled);
            return ret;
        }
#endif
        static std::string filterDemangle(const nchar* org) {
#ifdef NAMU_BUILD_PLATFORM_IS_WINDOWS
            std::string raw = std::string(org);
            int n = raw.rfind(" ");
#else
            const std::string& raw = demangle(org);
            int n = raw.rfind(":");
#endif
            return raw.substr(n + 1);
        }
    };

    /// @remark TClass is a class template using monostate pattern.
    ///         so it duplicates all static variables on each modules and it causes that can't
    ///         check hierarchy properly.
    ///         sure, I can exports them,template classes, as external symbol for 3rd party modules to link
    ///         and make it act well. but this requires user to declare another macro or statements such as
    ///         burden to get them be annoy.
    ///         instead of that, I put those inside of static symbols at normal nested private class.
    ///         now, TClass of course will be duplicated on each modules and code bloated but it will act
    ///         like proxy pointing this nested class.
    ///
    ///         in conclusion, users can ignore this background reasons and use CLASS macro without
    ///         additional consideration.
    template <typename T>
    struct tnameGetter {
        static const nchar* getRawName() { return typeid(T).name(); }
        static std::string getName() {
            std::string ret = nameDemangler::filterDemangle(getRawName());

            return ret;
        }
    };

    // famous void_t def:
    //  predefined at c++17 (if we use c++17, we can remove this)
    template <typename T>
    using void_t = void;

    // metaTypeChecker:
    //  if user defined metaType on their own, we let their TType classes inherit their superType class.
    //  otherwise, just use Type as base class.
    template <typename T, typename = void>
    struct tmetaTypeDef {
        using is = type;
    };
    template <typename T>
    struct tmetaTypeDef<T, void_t<int T::metaType::*>> {
        using is = typename T::metaType;
    };

    template <typename T, typename... Es>
    using areBaseOfT = std::conjunction<std::is_base_of<T, Es>...>;
}
