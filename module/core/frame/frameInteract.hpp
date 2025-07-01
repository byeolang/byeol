#pragma once

#include "core/frame/frameInteractable.hpp"

namespace nm {

    class frameInteract {
        NM(ME(frameInteract))

    public:
        frameInteract(const frameInteractable& it);
        frameInteract(const frameInteractable* it);
        frameInteract(const frameInteractable& it, const bicontainable& args);
        frameInteract(const frameInteractable& it, const bicontainable* args);
        frameInteract(const frameInteractable* it, const bicontainable* args);
        ~frameInteract();

    private:
        void _inFrame();

    private:
        const frameInteractable* _it;
        const bicontainable* _args;
    };
}
