#pragma once

#include "../../../builtin/primitive.hpp"
#include "../../../ast/mockNode.hpp"

namespace namu {

    struct marshalErr {};
    template <typename T, typename S> class tcppBridge;
    template <typename T, typename defaultElemType> class tarr;
    class arr;

    template <typename T, typename S, nbool isNode = tifSub<T, node>::is>
    struct tmarshaling : public metaIf {
        template <typename E>
        static str toMgd(E& it) {
            throw marshalErr();
        }

        static T& toNative(node& it) { throw marshalErr(); }
        static T& onAddParam() { return *new T(); }
        static T& onGetRet() { throw marshalErr(); }
        static no canMarshal();
    };
    template <typename S>
    struct tmarshaling<node&, S, true> : public metaIf {
        typedef node mgd;
        template <typename E>
        static str toMgd(E& it) {
            return it;
        }

        static node& toNative(node& it) { return it; }
        static mgd& onAddParam() { return *new mockNode(); }
        static mgd& onGetRet() { return *new mockNode(); }
        static yes canMarshal();
    };
    template <typename T, typename S>
    struct tmarshaling<T&, S, true> : public metaIf {
        typedef T mgd;
        template <typename E>
        static str toMgd(E& it) {
            return it;
        }

        static T& toNative(node& it) { return it.cast<T>(); }
        static mgd& onAddParam() { return *new mgd(); }
        static mgd& onGetRet() { return *new mgd(); }
        static yes canMarshal();
    };
    template <typename T, typename S>
    struct tmarshaling<T, S, true> : public metaIf {
        typedef T mgd;
        template <typename E>
        static str toMgd(const E& it) {
            return str((E*) it.clone());
        }

        static T& toNative(node& it) { return it.cast<T>(); }
        static mgd& onAddParam() { return *new mgd(); }
        static mgd& onGetRet() { return *new mgd(); }
        static yes canMarshal();
    };
    template <typename T, typename S>
    struct tmarshaling<T&, S, false> : public metaIf {
        typedef class tcppBridge<T, S> mgd;
        template <typename E>
        static str toMgd(E& it) {
            return new mgd(&it);
        }

        static T& toNative(node& it) { return it.cast<tcppBridge<T, S>>().get(); }
        static mgd& onAddParam() { return *new mgd(); }
        static mgd& onGetRet() { return *new mgd(); }
        static yes canMarshal();
    };
    template <typename T, typename S>
    struct tmarshaling<T*, S, false> : public metaIf {
        typedef tcppBridge<T, S> mgd;
        template <typename E>
        static str toMgd(E* it) {
            return new mgd(it);
        }

        static T* toNative(node& it) { return &it.cast<tcppBridge<T, S>>().get(); }
        static mgd& onAddParam() { return *new mgd(); }
        static mgd& onGetRet() { return *new mgd(); }
        static yes canMarshal();
    };
    template <typename T, typename S>
    struct tmarshaling<tarr<T, T>, S, true> : public metaIf {
        typedef arr mgd;

        static tarr<T, T> toNative(node& it);

        template <typename E>
        static str toMgd(E* it);

        static mgd& onAddParam();
        static mgd& onGetRet();
        static yes canMarshal();
    };

    template <typename tnativeType, typename tnativeGenericType, typename tmarshalType>
    struct tnormalMarshaling : public metaIf {
        typedef tmarshalType mgd;
        typedef tnativeType native;

        static native toNative(node& it) { return ((mgd&) it).get(); }
        static str toMgd(native it) { return str(new mgd(it)); }
        static mgd& onAddParam() { return *new mgd(); }
        static mgd& onGetRet() { return *new mgd(); }
        static yes canMarshal();
    };
    template <typename S>
    struct _nout tnormalMarshaling<void, S, nVoid> : public metaIf {
        typedef nVoid mgd;
        typedef void native;

        static str toMgd() { return str(new nVoid()); }
        static mgd& onAddParam() { return *new mgd(); }
        static mgd& onGetRet() { return *new mgd(); }
        static yes canMarshal();
    };

    template <typename S> struct _nout tmarshaling<nint, S, false> : public tnormalMarshaling<nint, S, nInt> {};
    template <typename S> struct _nout tmarshaling<nint&, S, false> : public tnormalMarshaling<nint, S, nInt> {};
    template <typename S> struct _nout tmarshaling<const nint&, S, false> : public tnormalMarshaling<nint, S, nInt> {};
    template <typename S> struct _nout tmarshaling<nbool, S, false> : public tnormalMarshaling<nbool, S, nBool> {};
    template <typename S> struct _nout tmarshaling<nflt, S, false> : public tnormalMarshaling<nflt, S, nFlt> {};
    template <typename S> struct _nout tmarshaling<nchar, S, false> : public tnormalMarshaling<nchar, S, nByte> {};
    template <typename S> struct _nout tmarshaling<std::string, S, false> : public tnormalMarshaling<const std::string&, S, nStr> {};
    template <typename S> struct _nout tmarshaling<std::string&, S, false> : public tnormalMarshaling<const std::string&, S, nStr> {};
    template <typename S> struct _nout tmarshaling<const std::string&, S, false> : public tnormalMarshaling<const std::string&, S, nStr> {};
    template <typename S> struct _nout tmarshaling<const std::string, S, false> : public tnormalMarshaling<const std::string&, S, nStr> {};
    template <typename S> struct _nout tmarshaling<void, S, false> : public tnormalMarshaling<void, S, nVoid> {};
}
