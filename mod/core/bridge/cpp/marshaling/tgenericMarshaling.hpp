#pragma once

#include "tmarshaling.hpp"
#include "../../../ast/exprs/getExpr.hpp"
#include "../../../ast/obj.hpp"

namespace nm {
    template <typename T, typename S, nbool isNode = tifSub<T, node>::is>
    struct tgenericMarshaling : public tmarshaling<T, S, isNode> {};

    template <>
    struct _nout tgenericMarshaling<node&, obj, true> : public metaIf {
        typedef getExpr mgdType;

        template <typename E>
        static str toMgd(E& it) {
            return it;
        }

        static node& toNative(node& it) { return it; }
        static mgdType& onAddParam() { return *new mgdType("T"); }
        static mgdType& onGetRet() { return *new mgdType("T"); }
        static yes canMarshal();
    };

    template <>
    struct _nout tgenericMarshaling<node*, obj, true> : public metaIf {
        typedef getExpr mgdType;

        template <typename E>
        static str toMgd(E& it) {
            return it;
        }

        static node* toNative(node& it) { return &it; }
        static mgdType& onAddParam() { return *new mgdType("T"); }
        static mgdType& onGetRet() { return *new mgdType("T"); }
        static yes canMarshal();
    };
    template <>
    struct _nout tgenericMarshaling<const node&, obj, true> : public metaIf {
        typedef getExpr mgdType;

        template <typename E>
        static str toMgd(E& it) {
            return it;
        }

        static node& toNative(node& it) { return it; }
        static mgdType& onAddParam() { return *new mgdType("T"); }
        static mgdType& onGetRet() { return *new mgdType("T"); }
        static yes canMarshal();
    };

    template <>
    struct _nout tgenericMarshaling<const node*, obj, true> : public metaIf {
        typedef getExpr mgdType;

        template <typename E>
        static str toMgd(E& it) {
            return it;
        }

        static node* toNative(node& it) { return &it; }
        static mgdType& onAddParam() { return *new mgdType("T"); }
        static mgdType& onGetRet() { return *new mgdType("T"); }
        static yes canMarshal();
    };
}
