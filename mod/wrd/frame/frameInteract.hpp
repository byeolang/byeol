#pragma once

#include "frameInteractable.hpp"

namespace wrd {

    class frameInteract {
        WRD(ME(frameInteract))

    public:
        frameInteract(frameInteractable& it, const bicontainable& args): _it(it) { _it._inFrame(args); }
        frameInteract(frameInteractable& it): _it(it) { _it._inFrame(nulOf<bicontainable>()); }
        ~frameInteract() { _it._outFrame(); }

    private:
        frameInteractable& _it;
    };
}
