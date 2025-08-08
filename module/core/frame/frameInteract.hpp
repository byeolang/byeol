#pragma once

#include "core/frame/frameInteractable.hpp"

namespace by {

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
        void _inFrame(const bicontainable* args);

    private:
        const frameInteractable* _it;
    };
}
