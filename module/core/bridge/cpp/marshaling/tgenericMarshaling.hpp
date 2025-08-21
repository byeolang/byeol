/// @file
#pragma once

#include "core/ast/exprs/getExpr.hpp"
#include "core/ast/obj.hpp"
#include "core/bridge/cpp/marshaling/tmarshaling.hpp"

namespace by {
    /// @ingroup core
    /// @brief Generic marshaling for C++ bridge
    /// @details Template specializations for marshaling generic node types between byeol and C++.
    template <typename T, nbool isNode = tifSub<T, node>::is>
    struct tgenericMarshaling: public tmarshaling<T, isNode> {};

    /// @ingroup core
    /// @brief Generic marshaling specialization for node reference
    /// @details Marshaling implementation for node reference types.
    template <> struct _nout tgenericMarshaling<node&, true>: public metaIf {
        typedef getExpr mgdType;

        template <typename E> static str toMgd(E& it) { return it; }

        static node& toNative(node& it) { return it; }

        static mgdType& onAddParam() { return *new mgdType("T"); }

        static mgdType* onGetRet() { return new mgdType("T"); }

        static yes canMarshal();
    };

    /// @ingroup core
    /// @brief Generic marshaling specialization for node pointer
    /// @details Marshaling implementation for node pointer types.
    template <> struct _nout tgenericMarshaling<node*, true>: public metaIf {
        typedef getExpr mgdType;

        template <typename E> static str toMgd(E* it) { return it; }

        static node* toNative(node& it) { return &it; }

        static mgdType& onAddParam() { return *new mgdType("T"); }

        static mgdType* onGetRet() { return new mgdType("T"); }

        static yes canMarshal();
    };

    /// @ingroup core
    /// @brief Generic marshaling specialization for const node reference
    /// @details Marshaling implementation for const node reference types.
    template <> struct _nout tgenericMarshaling<const node&, true>: public metaIf {
        typedef getExpr mgdType;

        template <typename E> static str toMgd(E& it) { return it; }

        static node& toNative(node& it) { return it; }

        static mgdType& onAddParam() { return *new mgdType("T"); }

        static mgdType* onGetRet() { return new mgdType("T"); }

        static yes canMarshal();
    };

    /// @ingroup core
    /// @brief Generic marshaling specialization for const node pointer
    /// @details Marshaling implementation for const node pointer types.
    template <> struct _nout tgenericMarshaling<const node*, true>: public metaIf {
        typedef getExpr mgdType;

        template <typename E> static str toMgd(E* it) { return it; }

        static node* toNative(node& it) { return &it; }

        static mgdType& onAddParam() { return *new mgdType("T"); }

        static mgdType* onGetRet() { return new mgdType("T"); }

        static yes canMarshal();
    };
} // namespace by
