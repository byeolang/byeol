#include "core/frame/frameInteract.hpp"

namespace nm {

    NM(DEF_ME(frameInteract))

    me::frameInteract(const frameInteractable& it): _it(&it), _args(nullptr) {
        _inFrame();
    }

    me::frameInteract(const frameInteractable* it): _it(it), _args(nullptr) {
        _inFrame();
    }

    me::frameInteract(const frameInteractable& it, const bicontainable& args):
        _it(&it), _args(&args) {
        _inFrame();
    }

    me::frameInteract(const frameInteractable& it, const bicontainable* args):
        _it(&it), _args(args) {
        _inFrame();
    }

    me::frameInteract(const frameInteractable* it, const bicontainable* args):
        _it(it), _args(args) {
        _inFrame();
    }

    me::~frameInteract() { _it->outFrame(_args); }

    void me::_inFrame() {
        WHEN_NUL(_it).ret();
        _it->inFrame(_args);
    }
}
