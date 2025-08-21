/// @file
#pragma once

#include "core/ast/args.hpp"
#include "core/ast/baseFunc.hpp"
#include "core/ast/params.hpp"

namespace by {

    template <typename T, nbool isBaseObj> class tbridger;

    /// @ingroup core
    /// @brief Bridge closure for C++ lambda/function objects
    /// @details Template that bridges C++ function objects and lambdas with byeol function interface.
    template <typename Ret, typename T, template <typename, nbool> class Marshaling, typename... Args>
    class tbridgeClosure: public baseFunc {
        BY(ME(tbridgeClosure, baseFunc), CLONE(tbridgeClosure))
        static_assert(
            allTrues<(sizeof(Marshaling<Args, tifSub<Args, node>::is>::canMarshal()) == sizeof(metaIf::yes))...>::value,
            "can't marshal one of this func's parameter ntypes.");

    public:
        tbridgeClosure(std::function<Ret(T&, Args...)> closure): _closure(closure) {}

    public:
        using super::run;

        const ntype& getType() const override {
            static mgdType inner("ctor", ttype<me>::get(),
                params(*new param("", Marshaling<Args, tifSub<Args, node>::is>::onAddParam())...), false,
                Marshaling<Ret, tifSub<typename typeTrait<Ret>::Org, node>::is>::onGetRet());
            return inner;
        }

        const baseObj& getOrigin() const override {
            static obj inner(tbridger<T, tifSub<typename tadaptiveSuper<T>::super, baseObj>::is>::subs());
            return inner;
        }

        str run(const args& a) override {
            args tray;
            args& evaluated = _evalArgs(a, tray) OR.err("evaluated == null").ret(str());

            return _marshal(evaluated, std::index_sequence_for<Args...>());
        }

    private:
        template <size_t... index> str _marshal(args& a, std::index_sequence<index...>) {
            T& me = (T*) a.getMe() OR.err("object from frame does not exists.").ret(str());

            return Marshaling<Ret, tifSub<typename typeTrait<Ret>::Org, node>::is>::toMgd(
                _closure(me, Marshaling<Args, tifSub<typename typeTrait<Args>::Org, node>::is>::toNative(a[index])...));
        }

        args* _evalArgs(const args& a, args& tray) {
            const params& ps = getParams();
            WHEN(a.len() != ps.len())
                .err("length of a(%d) and typs(%d) doesn't match.", a.len(), ps.len()).ret(nullptr);

            int n = 0;
            for(const node& e: a) {
                str ased = e.as(ps[n++].getOrigin()) OR.ret(nullptr);
                tray.add(*ased);
            }
            tray.setMe(a.getMe());
            return &tray;
        }

    private:
        std::function<Ret(T&, Args...)> _closure;
    };

    /// @ingroup core
    /// @brief Bridge closure specialization for void return type
    /// @details Specialized bridge closure for C++ functions returning void.
    template <typename T, template <typename, nbool> class Marshaling, typename... Args>
    class tbridgeClosure<void, T, Marshaling, Args...>: public baseFunc {
        BY(ME(tbridgeClosure, baseFunc), CLONE(tbridgeClosure))
        static_assert(
            allTrues<(sizeof(Marshaling<Args, tifSub<typename typeTrait<Args>::Org, node>::is>::canMarshal()) ==
                sizeof(metaIf::yes))...>::value,
            "can't marshal one of this func's parameter ntypes.");

    public:
        tbridgeClosure(std::function<void(T&, Args...)> closure): _closure(closure) {}

    public:
        const ntype& getType() const override {
            static mgdType inner("ctor", ttype<me>::get(),
                params(
                    *new param("", Marshaling<Args, tifSub<typename typeTrait<Args>::Org, node>::is>::onAddParam())...),
                false, Marshaling<void, false>::onGetRet());
            return inner;
        }

        const baseObj& getOrigin() const override {
            static obj inner(tbridger<T, tifSub<typename tadaptiveSuper<T>::super, baseObj>::is>::subs());
            return inner;
        }

        using super::run;

        str run(const args& a) override {
            args tray;
            args& evaluated = _evalArgs(a, tray) OR.err("evaluated == null").ret(str());

            return _marshal(evaluated, std::index_sequence_for<Args...>());
        }

    private:
        template <size_t... index> str _marshal(args& a, std::index_sequence<index...>) {
            T& me = (T*) a.getMe() OR.err("object from frame does not exists.").ret(str());

            _closure(me, Marshaling<Args, tifSub<typename typeTrait<Args>::Org, node>::is>::toNative(a[index])...);
            return Marshaling<void, tifSub<void, node>::is>::toMgd();
        }

        args* _evalArgs(const args& a, args& tray) {
            const params& ps = getParams();
            WHEN(a.len() != ps.len())
                .err("length of a(%d) and typs(%d) doesn't match.", a.len(), ps.len()).ret(nullptr);

            int n = 0;
            for(const node& e: a) {
                str ased = e.as(ps[n++].getOrigin()) OR.ret(nullptr);
                tray.add(*ased);
            }
            tray.setMe(a.getMe());
            return &tray;
        }

    private:
        std::function<void(T&, Args...)> _closure;
    };
} // namespace by
