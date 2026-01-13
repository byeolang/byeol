/// @file
#pragma once

#include "meta/type/ttypeBase.hpp"

namespace by {
    /** @ingroup meta
     *  @brief Template type wrapper providing entry point for meta information
     *  @details The entry point class for users to access meta information. Always create a
     *  new object when using ttype<T>. The entire design uses the monostate pattern, so
     *  creating objects each time shares values internally with no additional cost.
     *
     *  To understand the overall design of the meta module, first examine the core @ref type
     *  class.
     *
     *  @remark Custom meta type injection
     *  If a class T defines `typedef metaType MyType`, then ttype<T> will be based on MyType
     *  instead of the default type class. This allows modules depending on meta to inject
     *  additional type information. For example, the core module uses this to inject @ref ntype.
     *
     *  @section Usage
     *  Basic usage pattern:
     *  @code
     *      // Always create new ttype<T> objects
     *      ttype<MyClass> t1;
     *      ttype<MyClass> t2; // Shares internal data with t1 via monostate pattern
     *
     *      // Access type information
     *      cout << t1.getName() << "\n";
     *      cout << t1.isAbstract() << "\n";
     *
     *      // Check type relationships
     *      if(t1.isSuper<BaseClass>())
     *          cout << "MyClass derives from BaseClass\n";
     *  @endcode
     */
    template <typename T> class ttype: public ttypeBase<T, typename tmetaTypeDef<T, tifHasMetaTypeDef<T>::is>::is> {};
}