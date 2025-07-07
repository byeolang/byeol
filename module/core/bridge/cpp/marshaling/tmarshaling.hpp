#pragma once

#include "core/ast/tmock.hpp"
#include "core/builtin/primitive.hpp"

namespace nm {

    struct marshalErr {};
    template <typename T> class tbridge;
    template <typename T, typename defaultElemType> class tarr;
    class arr;

    template <typename T, nbool isNode = tifSub<typename typeTrait<T>::Org, node>::is>
    struct tmarshaling: public metaIf {
        typedef T mgd;

        template <typename E> static str toMgd(E& it) { throw marshalErr(); }

        static T& toNative(const str& it) { throw marshalErr(); }

        static const mgd& onAddParam() { return *new mgd(); }

        static const mgd* onGetRet() { throw marshalErr(); }

        static no canMarshal();
    };

    template <typename T, typename TACTIC>
    struct tmarshaling<tweak<T, TACTIC>, true>: public metaIf {
        typedef tbridge<tweak<T, TACTIC>> mgd;

        template <typename E> static str toMgd(E& it) { return it; }

        static tweak<T, TACTIC> toNative(node& it) { return it; }

        static const mgd& onAddParam() { return *new mgd(new T()); }

        static const mgd* onGetRet() { return new mgd(new T()); }

        static yes canMarshal();
    };

    template <typename T, typename TACTIC>
    struct tmarshaling<tstr<T, TACTIC>, true>: public metaIf {
        typedef tbridge<tstr<T, TACTIC>> mgd;

        template <typename E> static str toMgd(E& it) { return it; }

        static tstr<T, TACTIC> toNative(node& it) { return it; }

        static const mgd& onAddParam() { return *new mgd(new T()); }

        static const mgd* onGetRet() { return new mgd(new T()); }

        static yes canMarshal();
    };

    template <> struct tmarshaling<node&, true>: public metaIf {
        typedef node mgd;

        template <typename E> static str toMgd(E& it) { return it; }

        static node& toNative(node& it) { return it; }

        static const mgd& onAddParam() { return *new mockNode(); }

        static const mgd* onGetRet() { return new mockNode(); }

        static yes canMarshal();
    };

    template <> struct tmarshaling<node*, true>: public metaIf {
        typedef node mgd;

        template <typename E> static str toMgd(E* it) { return it; }

        static node* toNative(node& it) { return &it; }

        static const mgd& onAddParam() { return *new mockNode(); }

        static const mgd* onGetRet() { return new mockNode(); }

        static yes canMarshal();
    };

    template <typename T> struct tmarshaling<T&, true>: public metaIf {
        typedef T mgd;

        template <typename E> static str toMgd(E& it) { return it; }

        static T& toNative(node& it) { return *it.cast<T>(); }

        static const mgd& onAddParam() { return *new mgd(); }

        static const mgd* onGetRet() { return new mgd(); }

        static yes canMarshal();
    };

    template <typename T> struct tmarshaling<T*, true>: public metaIf {
        typedef T mgd;

        template <typename E> static str toMgd(E* it) { return it; }

        static T* toNative(node& it) { return it.cast<T>(); }

        static const mgd& onAddParam() { return *new mgd(); }

        static const mgd* onGetRet() { return new mgd(); }

        static yes canMarshal();
    };

    template <typename T> struct tmarshaling<T, true>: public metaIf {
        typedef T mgd;

        template <typename E> static str toMgd(const E& it) { return str((E*) it.clone()); }

        static T& toNative(node& it) { return *it.cast<T>(); }

        static const mgd& onAddParam() { return *new mgd(); }

        static const mgd* onGetRet() { return new mgd(); }

        static yes canMarshal();
    };

    template <typename tnativeType, typename tmarshalType> struct tnormalMarshaling: public metaIf {
        typedef tmarshalType mgd;
        typedef tnativeType native;

        static native toNative(node& it) { return ((mgd&) it).get(); }

        static str toMgd(native it) { return str(new mgd(it)); }

        static const mgd& onAddParam() { return *new mgd(); }

        static const mgd* onGetRet() { return new mgd(); }

        static yes canMarshal();
    };

    template <> struct tnormalMarshaling<char, nStr>: public metaIf {
        typedef nStr mgd;
        typedef char native;

        static native toNative(node& it) {
            const std::string& cast = ((mgd&) it).get();
            return cast.length() > 0 ? cast[0] : '\0';
        }

        static str toMgd(native it) { return str(new mgd(it)); }

        static const mgd& onAddParam() { return *new mgd(); }

        static const mgd* onGetRet() { return new mgd(); }

        static yes canMarshal();
    };

    template <> struct tnormalMarshaling<const char*, nStr>: public metaIf {
        typedef nStr mgd;
        typedef const char* native;

        static native toNative(node& it) { return ((mgd&) it).get().c_str(); }

        static str toMgd(native it) { return str(new mgd(it)); }

        static const mgd& onAddParam() { return *new mgd(); }

        static const mgd* onGetRet() { return new mgd(); }

        static yes canMarshal();
    };

    template <> struct _nout tnormalMarshaling<void, nVoid>: public metaIf {
        typedef nVoid mgd;
        typedef void native;

        static str toMgd() { return str(new nVoid()); }

        static const mgd& onAddParam() { return *new mgd(); }

        static const mgd* onGetRet() { return new mgd(); }

        static yes canMarshal();
    };

    template <> struct _nout tmarshaling<nint, false>: public tnormalMarshaling<nint, nInt> {};

    template <> struct _nout tmarshaling<nint&, false>: public tnormalMarshaling<nint, nInt> {};

    template <>
    struct _nout tmarshaling<const nint&, false>: public tnormalMarshaling<nint, nInt> {};

    template <> struct _nout tmarshaling<nbool, false>: public tnormalMarshaling<nbool, nBool> {};

    template <> struct _nout tmarshaling<nflt, false>: public tnormalMarshaling<nflt, nFlt> {};

    template <> struct _nout tmarshaling<nchar, false>: public tnormalMarshaling<nchar, nStr> {};

    template <>
    struct _nout tmarshaling<nchar*, false>: public tnormalMarshaling<const nchar*, nStr> {};

    template <>
    struct _nout tmarshaling<const nchar*, false>: public tnormalMarshaling<const nchar*, nStr> {};

    template <>
    struct _nout tmarshaling<std::string, false>
        : public tnormalMarshaling<const std::string&, nStr> {};

    template <>
    struct _nout tmarshaling<std::string&, false>
        : public tnormalMarshaling<const std::string&, nStr> {};

    template <>
    struct _nout tmarshaling<const std::string&, false>
        : public tnormalMarshaling<const std::string&, nStr> {};

    template <>
    struct _nout tmarshaling<const std::string, false>
        : public tnormalMarshaling<const std::string&, nStr> {};

    template <> struct _nout tmarshaling<void, false>: public tnormalMarshaling<void, nVoid> {};

    template <typename T> struct tmarshaling<T, false>: public metaIf {
        typedef class tbridge<T> mgd;

        template <typename E> static str toMgd(const E& it) { return new mgd(new E(it)); }

        static T& toNative(node& it) { return *it.cast<T>(); }

        static const mgd& onAddParam() { return *new mgd(); }

        static const mgd* onGetRet() { return new mgd(); }

        static yes canMarshal();
    };

    template <typename T> struct tmarshaling<T&, false>: public metaIf {
        typedef class tbridge<T> mgd;

        template <typename E> static str toMgd(E& it) { return new mgd(&it); }

        static T& toNative(node& it) { return it.cast<tbridge<T>>()->get(); }

        static const mgd& onAddParam() { return *new mgd(); }

        static const mgd* onGetRet() { return new mgd(); }

        static yes canMarshal();
    };

    template <typename T> struct tmarshaling<T*, false>: public metaIf {
        typedef tbridge<T> mgd;

        template <typename E> static str toMgd(E* it) { return new mgd(it); }

        static T* toNative(node& it) { return &it.cast<tbridge<T>>()->get(); }

        static const mgd& onAddParam() { return *new mgd(); }

        static const mgd* onGetRet() { return new mgd(); }

        static yes canMarshal();
    };

    template <typename T, typename E> struct tmarshaling<tarr<T, E>, true>: public metaIf {
        typedef arr mgd;

        static tarr<T, E> toNative(node& it);

        template <typename E2> static str toMgd(E2* it);

        static const mgd& onAddParam();
        static const mgd* onGetRet();
        static yes canMarshal();
    };
} // namespace nm
