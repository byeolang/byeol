#pragma once

#include "mgdType.hpp"
#include <functional>

namespace nm {
    // sometimes it goes into infinite loop when mgdType tries to generate retType when it makes
    // itself instance. because most of derived classes from baseObj tries to contains automatically
    // tbaseObjOrigin<T> for their origin object.
    // let me give some example, for instance,
    //
    //  nInt::nInt(): super(tbaseObjOrigin<nInt>(tbridge<T>::ctor<nInt>()) {}
    // when you make an instance of nInt, it will try to make an instance of tbaseObjOrigin<nInt>.
    // however this will trigger chain of infinite loop. see how it goes.
    //
    //  tbridger<nInt>::ctor<nInt>() { return new tbridgeFunc<Args...>(); }
    // tbridger<nInt>'s ctor() will try to make an instance of tbridgeFunc, but,
    //
    //  tbridgeFunc<Args..>::tbridgeFunc<Args...>():
    //  tbaseBridgeFunc(Marshaling<Args...>::onGetRet());
    // as you can see, it calls onGetRet() to include returnType to mgdType of tbridgeFunc.
    // guess what it'll happen if you call onGetRet().
    //
    //  Marshaling<T, Args...>::onGetRet() { return new T(); }
    // in this scenario, T is nInt. so, it eventually calls constructor of nInt.
    // and do you still remember what happen if you call constructor of nInt?
    //
    //  nInt::nInt(): super(tbaseObjOrigin<nInt>(tbridge<T>::ctor<nInt>())) {}
    // we goes back to first step. and you can see beautiful unexpected infinite loop.
    //
    // so this lazyMgdType making returnType in lazy, just like its name.
    // normally, you don't need to use this class. this only requires you to use only special when
    // you're in circumstance.
    class lazyMgdType: public mgdType {
        NM(ME(lazyMgdType, mgdType), CLONE(mgdType))

    public:
        lazyMgdType(const std::string& name, const type& s, const params& ps,
            std::function<const node&()> retLazy);

    public:
        const node& getRet() const override;

    private:
        std::function<const node&()> _retLazy;
    };
}
