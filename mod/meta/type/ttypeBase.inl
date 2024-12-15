#pragma once

#include "../rtti/rtti.hpp"
#include "ttypeBase.hpp"
#include "type.inl"

namespace nm {

#define TEMPL template <typename T, typename S>
#define ME ttypeBase<T, S>
#define SUPER S

    TEMPL ME::ttypeBase() { this->init(); }

    TEMPL nbool ME::isTemplate() const {
        static nbool inner = tifTemplate<T>::is;
        return inner;
    }

    TEMPL nbool ME::isAbstract() const {
        static nbool inner = !std::is_constructible<T>::value;
        return inner;
    }

    TEMPL void* ME::make() const { return tinstanceMaker<T>::make(); }

    TEMPL ncnt ME::size() const { return sizeof(T); }

    TEMPL const type& ME::getSuper() const {
        return ttype<typename tadaptiveSuper<T>::super>::get();
    }

    TEMPL const nbool& ME::isInit() const {
        static nbool inner = typeid(T) == typeid(adam);
        return inner;
    }

    TEMPL const ME& ME::get() {
        static ME* inner = nullptr;
        if(!inner) {
            inner = new ME(false);
            inner->init();
        }

        return *inner;
    }

    TEMPL types& ME::_getSupers() {
        static types inner;
        return inner;
    }

    TEMPL types& ME::_getSubs() {
        static types inner;
        return inner;
    }

    TEMPL types** ME::_onGetLeafs() const {
        static types* inner = nullptr;
        return &inner;
    }

    TEMPL void ME::_onAddSubClass(const type& subClass) {
        super::_onAddSubClass(subClass);

        types** leafs = _onGetLeafs();
        if(*leafs) this->_setLeafs(nullptr);
    }

    TEMPL type& ME::_getStatic() const { return const_cast<ME&>(get()); }

    TEMPL const std::string& ME::_getNativeName() const {
        static std::string inner = tnameGetter<T>::getName();
        return inner;
    }

    TEMPL ME::ttypeBase(nbool) {}

#undef TEMPL
#undef ME
#undef SUPER
} // namespace nm
