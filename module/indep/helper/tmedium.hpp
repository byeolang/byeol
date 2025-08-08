#pragma once

#include "indep/common.hpp"
#include "indep/macro/byeolMeta.hpp"
#include "indep/macro/declThis.hpp"
#include "indep/macro/unconstFunc.hpp"

namespace by {

    //  the medium class is only used in the OR macro:
    //      one of the characteristics of the OR macro is that the final return value can be received as a reference.
    //      the fact that the final output type of this OR macro is always a reference type suggests that
    //      there is a logic to convert nullptr to a null reference and return it in the middle, even if the rhs
    //      entered into the OR macro is nullptr.
    //      sometimes, there are cases where you need to immediately wrap the T& returned by the OR macro with
    //      another container or wrapper class that can receive it. for example,
    //
    //          A& got = makeAOnHeap() OR.ret();
    //          // since this `got` variable is on the heap, we will use tstr to easily manage memory.
    //          tstr<A> gotLife(got);
    //
    //      if you don't want to create two variables like above, you may want to do this:
    //
    //          str wrap = getA() OR.ret();
    //
    //      but, there is one problem in this way.
    //      the c++ standard does not specify that the address value of a null reference is null.
    //      (and in fact, in an environment where optimized code is run by clang on silicon macOS,
    //      referencing the address value of a nulled reference returns a garbage value.)
    //      so, when a null reference is received in the constructor of the tstr class, there is no way
    //      for the tstr class to determine whether it is correct or not.
    //
    //      then you might be thinking, `Why not just make the OR macro return a T*?`.
    //      if it returns T*, the meaning of using OR macro is greatly diminished.
    //      because users can no longer change from unsafe type to safe type easily, they might need to use
    //      some sort of c-style casting.
    //
    //      that's where `tmedium` class comes in. `tmedium` has the following purposes:
    //
    //          1. it should be very lightweight.
    //          2. it should have T* inside, but act like T& outside.
    //          3. it should be able to determine whether it is a valid T& or not from outside through tmedium.
    template <typename T>
    class tmedium {
        NM(ME(tmedium))

    public:
        tmedium(T* value);
        tmedium(const T* value);

    public:
        operator T&();
        operator const T&() const;
        operator nbool() const;
        T* operator->();
        const T* operator->() const BY_CONST_FUNC(operator->());

    public:
        nbool has() const;
        T* get();
        const T* get() const;

    private:
        T* _value;
    };
}
