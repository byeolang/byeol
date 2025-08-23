/// @file
#pragma once

#include <iostream>

#include "meta/common.hpp"
#include "meta/type/adam.hpp"

namespace by {

    /// @ingroup meta
    /// @brief Base interface for SFINAE-based type checking
    /// @details Provides yes/no types for template metaprogramming pattern
    /// used in compile-time type trait detection.
    struct _nout metaIf {
        typedef short yes;
        typedef char no;
    };

    /// @ingroup meta
    /// @brief Type trait to check if type has default constructor
    /// @details Uses SFINAE to determine at compile-time whether
    /// type T can be default-constructed.
    template <typename T> struct tifHasDefaultCtor: public metaIf {
        template <typename X, typename = decltype(X())> static yes test(X*);

        template <typename X> static no test(...);

        static constexpr nbool is = sizeof(decltype(test<T>(nullptr))) == sizeof(yes);
    };

    /// @ingroup meta
    /// @brief Type trait to check if type is a template specialization
    /// @details Uses SFINAE to determine if T is a template specialization
    /// at compile-time.
    template <typename T> struct tifTemplate: public metaIf {
        template <template <typename> class Template, typename X> static yes _foo(Template<X>*);
        static no _foo(...);

        static inline constexpr nbool is = sizeof(_foo((T*) 0)) == sizeof(yes);
    };

    /// @ingroup meta
    /// @brief Type trait to check if T is subclass of super
    /// @details Determines inheritance relationship between types
    /// with support for reference types.
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

    /// @ingroup meta
    /// @brief Helper struct for SFINAE detection patterns
    /// @details Provides void typedef for enabling SFINAE
    /// in template specializations.
    template <typename T> struct taEmptyCan {
        typedef void is;
    };

    /// @ingroup meta
    /// @brief Type trait to check if type has super typedef
    /// @details Detects whether type T defines a 'super' typedef
    /// for inheritance hierarchy information.
    template <typename T, typename = void> struct tifHasSuperTypedef: public metaIf {
        static inline constexpr nbool is = false;
    };

    template <typename T> struct tifHasSuperTypedef<T, typename taEmptyCan<typename T::super>::is>: public metaIf {
        static inline constexpr nbool is = true;
    };

    /// @ingroup meta
    /// @brief Adaptive super type selector
    /// @details Automatically selects appropriate super type: either
    /// T::super if defined, or adam as fallback base type.
    template <typename T, nbool typedefsuper = tifHasSuperTypedef<T>::is> struct tadaptiveSuper: metaIf {
        typedef adam super;
    };

    template <typename T> struct tadaptiveSuper<T, true> {
        typedef typename T::super super;
    };

    /// @ingroup meta
    /// @brief Instance factory for type creation
    /// @details Creates instances of type T if constructible,
    /// returns nullptr otherwise.
    template <typename T, nbool canMake = std::is_constructible<T>::value> struct tinstanceMaker {
        static void* make() { return nullptr; }
    };

    template <typename T> struct tinstanceMaker<T, true> {
        static void* make() { return (void*) new T(); }
    };

    /// @ingroup meta
    /// @brief Template name getter for type introspection
    /// @details TClass is a class template using monostate pattern.
    /// so it duplicates all static variables on each modules and it causes that can't
    /// check hierarchy properly.
    /// sure, I can exports them,template classes, as external symbol for 3rd party modules
    /// to link and make it act well. but this requires user to declare another macro or
    /// statements such as burden to get them be annoy. instead of that, I put those inside
    /// of static symbols at normal nested private class. now, TClass of course will be
    /// duplicated on each modules and code bloated but it will act like proxy pointing this
    /// nested class.
    ///
    /// in conclusion, users can ignore this background reasons and use CLASS macro without
    /// additional consideration.
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

    /// @ingroup meta
    /// @brief Type trait to check if type has metaType definition
    /// @details metaTypeChecker: if user defined metaType on their own, we let their TType classes inherit their
    /// superType class. otherwise, just use Type as base class.
    template <typename T, typename = void> struct tifHasMetaTypeDef: public metaIf {
        static inline constexpr nbool is = false;
    };

    template <typename T> struct tifHasMetaTypeDef<T, typename taEmptyCan<typename T::metaType>::is>: public metaIf {
        static inline constexpr nbool is = true;
    };

    /// @ingroup meta
    /// @brief Meta type definition selector
    /// @details Selects appropriate meta type: either T::metaType
    /// if defined, or default 'type' as fallback.
    template <typename T, nbool hasMeta = tifHasMetaTypeDef<T>::is> struct tmetaTypeDef {
        using is = type;
    };

    template <typename T> struct tmetaTypeDef<T, true> {
        using is = typename T::metaType;
    };

    template <typename T, typename... Es> using areBaseOfT = std::conjunction<std::is_base_of<T, Es>...>;
} // namespace by
