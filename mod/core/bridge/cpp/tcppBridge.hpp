#pragma once

#include "tcppBridgeFunc.hpp"
#include "marshaling/tgenericMarshaling.hpp"
#include "../../ast/obj.hpp"
#include "../../type/mgdType.hpp"
#include "../../ast/defaultCopyCtor.hpp"
#include "../../ast/defaultCtor.hpp"

namespace nm {

    /// bridge object only can shares 'shared' sub nodes.
    /// @param T represents native class.
    template <typename T>
    class tcppBridge : public baseObj {
        // TODO: how to impement 'as()' on bridge obj:
        //  each tcppBridge obj has its unique type object. when it got called 'getType()'
        //  it returns its type object.
        //
        //  however, type object is dynamically belongs to this bridge object, when user
        //  tries to get ttype<T>, it's not derived from ntype so it won't have any 'as()'
        //  func. user can't operate conversion in this way.
        NM(ME(tcppBridge, baseObj),
           INIT_META(tcppBridge),
           CLONE(tcppBridge))

    public:
        typedef ntype metaType;
        template <typename Ret, typename T1, nbool, template <typename, nbool> class Marshaling, typename...Args>
        friend class tcppBridgeFunc;
        template <typename T1, nbool>
        friend class tbridger;
        template <typename T1, nbool>
        friend struct tmarshaling;

        static_assert(!tifSub<T, baseObj>::is, "parameterized type 'T' shouldn't be a derived class to baseObj. override subs() if it is.");

    protected:
        /// @hidden this api is only available to tmarshaling.
        tcppBridge(): me(nullptr) {}
        tcppBridge(T* real): me(dumScope::singletone(), real) {}
        tcppBridge(const scope& subs, T* real): super(), _real(real) {
            _subs.bind(subs);
        }

    public:
        tcppBridge(const me& rhs): super(rhs), _real(rhs._real ? new T(*rhs._real) : nullptr), _subs(rhs._subs) {}
        ~tcppBridge() override {
            if(_ownReal && _real)
                delete _real;
        }

    public:
        const ntype& getType() const override {
            static mgdType inner(ttype<T>::get().getName());
            return inner;
        }

        using super::subs;
        scope& subs() override { return *_subs; }

        T& get() {
            return *_real;
        }
        const T& get() const {
            return *_real;
        }

        const baseObj& getOrigin() const override {
            // if an object doesn't have owned sub nodes it means that all instances of that classes
            // are same and origin simulteneously.
            return *this;
        }

    private:
        T* _real;
        nbool _ownReal = _real;
        tstr<scope> _subs;
    };
}
