#pragma once

#include <iostream>

#include "meta/common.hpp"
#include "meta/type/adam.hpp"

namespace by {

    struct _nout metaIf {
        typedef short yes;
        typedef char no;
    };

    template <typename T> struct tifHasDefaultCtor: public metaIf {
        template <typename X, typename = decltype(X())> static yes test(X*);

        template <typename X> static no test(...);

        static constexpr nbool is = sizeof(decltype(test<T>(nullptr))) == sizeof(yes);
    };

    template <typename T> struct tifTemplate: public metaIf {
        template <template <typename> class Template, typename X> static yes _foo(Template<X>*);
        static no _foo(...);

        static inline constexpr nbool is = sizeof(_foo((T*) 0)) == sizeof(yes);
    };

    template <typename T, typename super> // is T is sub of super
    struct tifSub {
        static inline constexpr nbool is = std::is_base_of<super, T>::value;
    };

    template <typename T, typename super> struct tifSub<T&, super> {
        static inline constexpr nbool is = std::is_base_of<super, T>::value;
    };

    template <typename T, typename super> struct tifSub<T, super&> {
        static inline constexpr nbool is = std::is_base_of<super, T>::value;
    };

    template <typename T, typename super> struct tifSub<T&, super&> {
        static inline constexpr nbool is = std::is_base_of<super, T>::value;
    };

    template <typename T> struct taEmptyCan {
        typedef void is;
    };

    template <typename T, typename = void> struct tifHasSuperTypedef: public metaIf {
        static inline constexpr nbool is = false;
    };

    template <typename T>
    struct tifHasSuperTypedef<T, typename taEmptyCan<typename T::super>::is>: public metaIf {
        static inline constexpr nbool is = true;
    };

    template <typename T, nbool typedefsuper = tifHasSuperTypedef<T>::is>
    struct tadaptiveSuper: metaIf {
        typedef adam super;
    };

    template <typename T> struct tadaptiveSuper<T, true> {
        typedef typename T::super super;
    };

    template <typename T, nbool canMake = std::is_constructible<T>::value> struct tinstanceMaker {
        static void* make() { return nullptr; }
    };

    template <typename T> struct tinstanceMaker<T, true> {
        static void* make() { return (void*) new T(); }
    };

    /// @remark TClass is a class template using monostate pattern.
    ///         so it duplicates all static variables on each modules and it causes that can't
    ///         check hierarchy properly.
    ///         sure, I can exports them,template classes, as external symbol for 3rd party modules
    ///         to link and make it act well. but this requires user to declare another macro or
    ///         statements such as burden to get them be annoy. instead of that, I put those inside
    ///         of static symbols at normal nested private class. now, TClass of course will be
    ///         duplicated on each modules and code bloated but it will act like proxy pointing this
    ///         nested class.
    ///
    ///         in conclusion, users can ignore this background reasons and use CLASS macro without
    ///         additional consideration.
    template <typename T> struct tnameGetter {
        static const nchar* getRawName() { return typeid(T).name(); }

        static std::string getName() {
            std::string ret = platformAPI::filterDemangle(getRawName());

            return ret;
        }
    };

    // famous void_t def:
    //  predefined at c++17 (if we use c++17, we can remove this)
    template <typename T> using void_t = void;

    // metaTypeChecker:
    //  if user defined metaType on their own, we let their TType classes inherit their superType
    //  class. otherwise, just use Type as base class.
    template <typename T, typename = void> struct tifHasMetaTypeDef: public metaIf {
        static inline constexpr nbool is = false;
    };

    template <typename T>
    struct tifHasMetaTypeDef<T, typename taEmptyCan<typename T::metaType>::is>: public metaIf {
        static inline constexpr nbool is = true;
    };

    template <typename T, nbool hasMeta = tifHasMetaTypeDef<T>::is> struct tmetaTypeDef {
        using is = type;
    };

    template <typename T> struct tmetaTypeDef<T, true> {
        using is = typename T::metaType;
    };

    template <typename T, typename... Es>
    using areBaseOfT = std::conjunction<std::is_base_of<T, Es>...>;
} // namespace by
