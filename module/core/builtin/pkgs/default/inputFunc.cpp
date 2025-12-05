#include "core/builtin/pkgs/default/inputFunc.hpp"

#include <iostream>
#include "core/builtin/scalar/nStr.hpp"
#ifdef __EMSCRIPTEN__
#    include <emscripten/emscripten.h>
#endif

namespace by {

    BY(DEF_ME(inputFunc))

    const ntype& me::getType() const {
        static mgdType inner("input", ttype<me>::get(), params(), false, new nStr());
        return inner;
    }

    const baseObj& me::getOrigin() const {
        static obj inner(scope{
            {"input", new me()}
        });
        return inner;
    }

    str me::eval(const args& a) {
        const params& ps = getParams();
        if(a.len() != ps.len())
            return BY_E("length of args(%d) and typs(%d) doesn't match.", a.len(), ps.len()), nullptr;

        nStr* ret = new nStr();

#ifdef __EMSCRIPTEN__
        ret->get() = emscripten_run_script_string("prompt('input:');");
#else
        std::getline(std::cin, ret->get());
#endif

        return str(ret);
    }
}
