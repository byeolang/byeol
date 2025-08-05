#include "core/frame/frameInteract.hpp"

namespace nm {

    NM(DEF_ME(frameInteract))

    me::frameInteract(const frameInteractable& it): _it(&it) {
        _inFrame(nullptr);
    }

    me::frameInteract(const frameInteractable* it): _it(it) {
        _inFrame(nullptr);
    }

    me::frameInteract(const frameInteractable& it, const bicontainable& args):
        _it(&it) {
        _inFrame(&args);
    }

    me::frameInteract(const frameInteractable& it, const bicontainable* args):
        _it(&it) {
        _inFrame(args);
    }

    me::frameInteract(const frameInteractable* it, const bicontainable* args):
        _it(it) {
        _inFrame(args);
    }

    me::~frameInteract() { _it->outFrame(); }

    void me::_inFrame(const bicontainable* args) {
        WHEN_NUL(_it).ret();
        _it->inFrame(args);
    }
}
