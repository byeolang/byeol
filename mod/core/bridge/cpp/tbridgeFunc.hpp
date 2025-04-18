#pragma once

#include "../../ast/args.hpp"
#include "../../ast/baseFunc.hpp"
#include "../../ast/params.hpp"
#include "../../ast/src/dumSrc.hpp"

namespace nm {

    template <typename T, nbool isBaseObj> class tbridger;

    template <typename Ret, typename T, template <typename, nbool> class Marshaling,
        typename... Args>
    class tbaseBridgeFunc: public baseFunc {
        NM(ME(tbaseBridgeFunc, baseFunc))
    protected:
        typedef Ret (T::*fptrType)(Args...);

    public:
        tbaseBridgeFunc(fptrType fptr, str ret):
            _fptr(fptr),
            _type("bridgeFunc", ttype<me>::get(),
                params(*new param("", Marshaling<Args, tifSub<Args, node>::is>::onAddParam())...),
                false, *ret),
            _src(dumSrc::singleton()) {}

    public:
        static_assert(allTrues<(sizeof(Marshaling<Args, tifSub<Args, node>::is>::canMarshal()) ==
                          sizeof(metaIf::yes))...>::value,
            "can't marshal one of this func's parameter ntypes.");

    public:
        const ntype& getType() const override { return _type; }

        const baseObj& getOrigin() const override {
            static obj inner(
                tbridger<T, tifSub<typename tadaptiveSuper<T>::super, baseObj>::is>::subs());
            return inner;
        }

        using super::run;

        str run(const args& a) override {
            tmay<args> evaluated = a.evalAll(getParams()) OR.ret(str());

            return _runNative(*evaluated);
        }

        void onCloneDeep(const clonable& from) override {
            me& rhs = (me&) from;
            this->_type.onCloneDeep(rhs._type);
        }

        const src& getSrc() const override { return *_src; }

        void setSrc(const src& new1) { _src.bind(new1); }

    protected:
        virtual str _runNative(args& args) = 0;

    protected:
        fptrType _fptr;
        mutable mgdType _type;
        tstr<src> _src;
    };

    template <typename Ret, typename T, nbool isBaseObj,
        template <typename, nbool> class Marshaling, typename... Args>
    class tbridgeFunc: public tbaseBridgeFunc<Ret, T, Marshaling, Args...> {
        typedef tbaseBridgeFunc<Ret, T, Marshaling, Args...> _super_;
        NM(ME(tbridgeFunc, _super_), CLONE(tbridgeFunc))

    public:
        tbridgeFunc(typename super::fptrType fptr):
            super(fptr, Marshaling<Ret, tifSub<Ret, node>::is>::onGetRet()) {}

    protected:
        str _runNative(args& args) override {
            return _marshal(args, std::index_sequence_for<Args...>());
        }

        template <size_t... index> str _marshal(args& args, std::index_sequence<index...>);
    };

    template <typename T, template <typename, nbool> class Marshaling, typename... Args>
    class tbridgeFunc<void, T, false, Marshaling, Args...>
        : public tbaseBridgeFunc<void, T, Marshaling, Args...> {
        typedef tbaseBridgeFunc<void, T, Marshaling, Args...> _super_;
        NM(ME(tbridgeFunc, _super_), CLONE(tbridgeFunc))

    public:
        tbridgeFunc(typename super::fptrType fptr):
            super(fptr, Marshaling<void, false>::onGetRet()) {}

    protected:
        str _runNative(args& args) override {
            return _marshal(args, std::index_sequence_for<Args...>());
        }

        template <size_t... index> str _marshal(args& args, std::index_sequence<index...>);
    };

    template <typename T, template <typename, nbool> class Marshaling, typename... Args>
    class tbridgeFunc<void, T, true, Marshaling, Args...>
        : public tbaseBridgeFunc<void, T, Marshaling, Args...> {
        typedef tbaseBridgeFunc<void, T, Marshaling, Args...> _super_;
        NM(ME(tbridgeFunc, _super_), CLONE(tbridgeFunc))

    public:
        tbridgeFunc(typename super::fptrType fptr):
            super(fptr, Marshaling<void, false>::onGetRet()) {}

    protected:
        str _runNative(args& args) override {
            return _marshal(args, std::index_sequence_for<Args...>());
        }

        template <size_t... index> str _marshal(args& a, std::index_sequence<index...>) {
            T *me = (T*) &a.getMe() OR.err("object from frame does not exists.").ret(str());
            (me->*(this->_fptr))(Marshaling<Args, tifSub<Args, node>::is>::toNative(a[index])...);
            return Marshaling<void, tifSub<void, node>::is>::toMgd();
        }
    };

    template <typename Ret, typename T, template <typename, nbool> class Marshaling,
        typename... Args>
    class tbridgeFunc<Ret, T, true, Marshaling, Args...>
        : public tbaseBridgeFunc<Ret, T, Marshaling, Args...> {
        typedef tbaseBridgeFunc<Ret, T, Marshaling, Args...> _super_;
        NM(ME(tbridgeFunc, _super_), CLONE(tbridgeFunc))

    public:
        tbridgeFunc(typename _super_::fptrType fptr):
            super(fptr, Marshaling<Ret, tifSub<Ret, node>::is>::onGetRet()) {}

    protected:
        str _runNative(args& args) override {
            return _marshal(args, std::index_sequence_for<Args...>());
        }

        template <size_t... index> str _marshal(args& a, std::index_sequence<index...>) {
            T *me = (T*) &a.getMe() OR.err("object from frame does not exists.").ret(str());
            return Marshaling<Ret, tifSub<Ret, node>::is>::toMgd((me->*(this->_fptr)) // funcptr
                (Marshaling<Args, tifSub<Args, node>::is>::toNative(a[index])...)); // and args.ZZZ
        }
    };
} // namespace nm
