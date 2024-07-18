#pragma once

#include "../../ast/baseFunc.hpp"
#include "../../ast/args.hpp"
#include "../../ast/params.hpp"
#include "ast/mockNode.hpp"

namespace nm {

    template <typename Ret, typename T, template <typename, nbool> class Marshaling, typename... Args>
    class tbaseBridgeFunc : public baseFunc {
        NM(ADT(tbaseBridgeFunc, baseFunc))
    protected:
        typedef Ret (T::*fptrType)(Args...);

    public:
        /// @param mgdOrg it's managed pointer.
        ///               the instance refered by this pointer should be managed at somewhere.
        ///               that is, this class will just get the value of address and won't release
        ///               memory.
        tbaseBridgeFunc(fptrType fptr): super(), _fptr(fptr) {}

    public:
        static_assert(allTrues<(sizeof(Marshaling<Args, tifSub<Args, node>::is>::canMarshal() ) ==
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

        nbool setRet(const node& newRet) override {
            return _ret.bind(newRet);
        }

        const params& getParams() const override {
            if(!_params) {
                _params.bind(new params());
                (_params->add(new param("", Marshaling<Args, tifSub<Args, node>::is>::onAddParam())), ...);
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

    template <typename Ret, typename T, nbool isBaseObj, template <typename, nbool> class Marshaling,
             typename... Args>
    class tbridgeFunc : public tbaseBridgeFunc<Ret, T, Marshaling, Args...> {
        typedef tbaseBridgeFunc<Ret, T, Marshaling, Args...> _super_;
        NM(CLASS(tbridgeFunc, _super_))

    public:
        tbridgeFunc(typename _super_::fptrType fptr): super(fptr) {}

    public:
        str getRet() const override {
            if(!this->_ret)
                this->_ret.bind(Marshaling<Ret, tifSub<Ret, node>::is>::onGetRet());

            return this->_ret;
        }

    protected:
        str _runNative(args& args) override {
            return _marshal(args, std::index_sequence_for<Args...>());
        }

        template <size_t... index>
        str _marshal(args& args, std::index_sequence<index...>);
    };
    template <typename T, template <typename, nbool> class Marshaling,
             typename... Args>
    class tbridgeFunc<void, T, false, Marshaling, Args...> : public tbaseBridgeFunc<void, T, Marshaling, Args...> {
        typedef tbaseBridgeFunc<void, T, Marshaling, Args...> _super_;
        NM(CLASS(tbridgeFunc, _super_))

    public:
        tbridgeFunc(typename _super_::fptrType fptr): super(fptr) {}

    public:
        str getRet() const override {
            if(!this->_ret)
                this->_ret.bind(Marshaling<void, false>::onGetRet());

            return this->_ret;
        }

    protected:
        str _runNative(args& args) override {
            return _marshal(args, std::index_sequence_for<Args...>());
        }

        template <size_t... index>
        str _marshal(args& args, std::index_sequence<index...>);
    };
    template <typename T, template <typename, nbool> class Marshaling,
             typename... Args>
    class tbridgeFunc<void, T, true, Marshaling, Args...> : public tbaseBridgeFunc<void, T, Marshaling, Args...> {
        typedef tbaseBridgeFunc<void, T, Marshaling, Args...> _super_;
        NM(CLASS(tbridgeFunc, _super_))

    public:
        tbridgeFunc(typename _super_::fptrType fptr): super(fptr) {}

    public:
        str getRet() const override {
            if(!this->_ret)
                this->_ret.bind(Marshaling<void, false>::onGetRet());

            return this->_ret;
        }

    protected:
        str _runNative(args& args) override {
            return _marshal(args, std::index_sequence_for<Args...>());
        }

        template <size_t... index>
        str _marshal(args& a, std::index_sequence<index...>) {
            T* me = (T*) &a.getMe();
            if(nul(me)) return NM_E("object from frame does not exists."), str();
            if(me->template isSub<mockNode>()) {
                mockNode& mock = me->template cast<mockNode>();
                me = (T*) &mock.getTarget();
            }
            (me->*(this->_fptr))(Marshaling<Args, tifSub<Args, node>::is>::toNative(a[index])...);
            return Marshaling<void, tifSub<void, node>::is>::toMgd();
        }
    };
    template <typename Ret, typename T, template <typename, nbool> class Marshaling,
             typename... Args>
    class tbridgeFunc<Ret, T, true, Marshaling, Args...> : public tbaseBridgeFunc<Ret, T, Marshaling, Args...> {
        typedef tbaseBridgeFunc<Ret, T, Marshaling, Args...> _super_;
        NM(CLASS(tbridgeFunc, _super_))

    public:
        tbridgeFunc(typename _super_::fptrType fptr): super(fptr) {}

    public:
        str getRet() const override {
            if(!this->_ret)
                this->_ret.bind(Marshaling<Ret, tifSub<Ret, node>::is>::onGetRet());

            return this->_ret;
        }

    protected:
        str _runNative(args& args) override {
            return _marshal(args, std::index_sequence_for<Args...>());
        }

        template <size_t... index>
        str _marshal(args& a, std::index_sequence<index...>) {
            T* me = (T*) &a.getMe();
            if(nul(me)) return NM_E("object from frame does not exists."), str();
            if(me->template isSub<mockNode>()) {
                mockNode& mock = me->template cast<mockNode>();
                me = (T*) &mock.getTarget();
            }
            return Marshaling<Ret, tifSub<Ret, node>::is>::toMgd((me->*(this->_fptr)) // funcptr
                    (Marshaling<Args, tifSub<Args, node>::is>::toNative(a[index])...)); // and args.ZZZ
        }
    };
}
