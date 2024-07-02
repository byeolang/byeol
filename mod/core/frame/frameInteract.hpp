#pragma once

#include "frameInteractable.hpp"

namespace nm {

    class frameInteract {
        NM(ME(frameInteract))

    public:
        frameInteract(frameInteractable& it, const bicontainable& args);
        frameInteract(frameInteractable& it);
        ~frameInteract();

    private:
        frameInteractable& _it;
        const bicontainable& _args;
    };
}
