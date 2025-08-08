#pragma once

#include "meta/type/ttypeBase.hpp"

namespace by {
    /// @remark ttype returns ttype<type> as its meta class.
    ///         however, this makes impossible to get specific ttype instance
    ///         at a binder.
    ///
    ///         if user typedefs SuperType at their type T, then ttype
    ///         inherits from given SuperType.
    ///         this eventually make user add API to want to ttype class.
    template <typename T>
    class ttype: public ttypeBase<T, typename tmetaTypeDef<T, tifHasMetaTypeDef<T>::is>::is> {};
}