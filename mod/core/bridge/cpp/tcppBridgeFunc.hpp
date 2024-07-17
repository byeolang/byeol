#pragma once

#include "../../ast/baseFunc.hpp"
#include "../../ast/args.hpp"
#include "../../ast/params.hpp"

namespace nm {

    template <typename Ret, typename T, typename S, template <typename, typename, nbool> class Marshaling, typename... Args>
    class tcppBridgeFuncBase : public baseFunc {
        NM(ADT(tcppBridgeFuncBase, baseFunc))
    protected:
        typedef Ret (T::*fptrType)(Args...);

    public:
        tcppBridgeFuncBase(fptrType fptr): super(), _fptr(fptr) {}

        static_assert(allTrues<(sizeof(Marshaling<Args, S, tifSub<Args, node>::is>::canMarshal() ) ==
                sizeof(metaIf::yes))...>::value,
                "can't marshal one of this func's parameter ntypes.");

    public:
        using super::run;
        str run(const args& a) override {
            args tray;
            args& evaluated = _evalArgs(a, tray);
            if(nul(evaluated))
                return NM_E("evaluated == null"), str();

            return _runNative(evaluated);
        }

        str getRet() const override {
            if(!_ret)
                _ret.bind(Marshaling<Ret, S, tifSub<Ret, node>::is>::onGetRet());

            return _ret;
        }

        nbool setRet(const node& newRet) override {
            return _ret.bind(newRet);
        }

        const params& getParams() const override {
            if(!_params) {
                _params.bind(new params());
                (_params->add(new param("", Marshaling<Args, S, tifSub<Args, node>::is>::onAddParam())), ...);
            }
            return *_params;
        }

        clonable* cloneDeep() const override {
            me* ret = (me*) clone();
            if(_params)
                ret->_params.bind((params*) _params->cloneDeep());

            return ret;
        }

    protected:
        virtual str _runNative(args& args) = 0;

    private:
        args& _evalArgs(const args& a, args& tray) {
            const params& ps = getParams();
            if(a.len() != ps.len())
                return NM_E("length of a(%d) and typs(%d) doesn't match.", a.len(), ps.len()),
                       nulOf<args>();

            int n = 0;
            for(const node& e: a) {
                str ased = e.as(ps[n++].getOrigin());
                if(!ased)
                    return nulOf<args>();

                tray.add(*ased);
            }
            tray.setMe(a.getMe());
            return tray;
        }

    protected:
        fptrType _fptr;
        mutable tstr<params> _params;
        mutable str _ret;
    };

    template <typename Ret, typename T, typename S, nbool isBaseObj, template <typename, typename, nbool> class Marshaling,
             typename... Args>
    class tcppBridgeFunc : public tcppBridgeFuncBase<Ret, T, S, Marshaling, Args...> {
        typedef tcppBridgeFuncBase<Ret, T, S, Marshaling, Args...> _super_;
        NM(CLASS(tcppBridgeFunc, _super_))

    public:
        tcppBridgeFunc(typename _super_::fptrType fptr): super(fptr) {}

    protected:
        str _runNative(args& args) override {
            return _marshal(args, std::index_sequence_for<Args...>());
        }

        template <size_t... index>
        str _marshal(args& args, std::index_sequence<index...>);
    };
    template <typename T, typename S, template <typename, typename, nbool> class Marshaling,
             typename... Args>
    class tcppBridgeFunc<void, T, S, false, Marshaling, Args...> : public tcppBridgeFuncBase<void, T, S, Marshaling, Args...> {
        typedef tcppBridgeFuncBase<void, T, S, Marshaling, Args...> _super_;
        NM(CLASS(tcppBridgeFunc, _super_))

    public:
        tcppBridgeFunc(typename _super_::fptrType fptr): super(fptr) {}

    protected:
        str _runNative(args& args) override {
            return _marshal(args, std::index_sequence_for<Args...>());
        }

        template <size_t... index>
        str _marshal(args& args, std::index_sequence<index...>);
    };

    template <typename Ret, typename T, typename S, template <typename, typename, nbool> class Marshaling,
             typename... Args>
    class tcppBridgeFunc<Ret, T, S, true, Marshaling, Args...> : public tcppBridgeFuncBase<Ret, T, S, Marshaling, Args...> {
        typedef tcppBridgeFuncBase<Ret, T, S, Marshaling, Args...> _super_;
        NM(CLASS(tcppBridgeFunc, _super_))

    public:
        tcppBridgeFunc(typename _super_::fptrType fptr): super(fptr) {}

    protected:
        str _runNative(args& args) override {
            return _marshal(args, std::index_sequence_for<Args...>());
        }

        template <size_t... index>
        str _marshal(args& args, std::index_sequence<index...>);
    };
    template <typename T, typename S, template <typename, typename, nbool> class Marshaling,
             typename... Args>
    class tcppBridgeFunc<void, T, S, true, Marshaling, Args...> : public tcppBridgeFuncBase<void, T, S, Marshaling, Args...> {
        typedef tcppBridgeFuncBase<void, T, S, Marshaling, Args...> _super_;
        NM(CLASS(tcppBridgeFunc, _super_))

    public:
        tcppBridgeFunc(typename _super_::fptrType fptr): super(fptr) {}

    protected:
        str _runNative(args& args) override {
            return _marshal(args, std::index_sequence_for<Args...>());
        }

        template <size_t... index>
        str _marshal(args& args, std::index_sequence<index...>);
    };
}
