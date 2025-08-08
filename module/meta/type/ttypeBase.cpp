#include "meta/type/ttypeBase.inl"

namespace by {

#define ME ttypeBase<void, type>
#define SUPER type

    ME::ttypeBase() { this->init(); }

    nbool ME::isTemplate() const { return false; }

    nbool ME::isAbstract() const { return true; }

    void* ME::make() const { return nullptr; }

    ncnt ME::size() const { return 0; }

    const type& ME::getSuper() const { return ttype<typename tadaptiveSuper<void>::super>::get(); }

    const nbool& ME::isInit() const {
        static nbool inner = typeid(void) == typeid(adam);
        return inner;
    }

    const ME& ME::get() {
        static ME* inner = nullptr;
        if(!inner) {
            inner = new ME(false);
            inner->init();
        }

        return *inner;
    }

    types& ME::_getSupers() {
        static types inner;
        return inner;
    }

    types& ME::_getSubs() {
        static types inner;
        return inner;
    }

    type& ME::_getStatic() const { return (ME&) get(); }

    ME::ttypeBase(nbool) {}

    types** ME::_onGetLeafs() const {
        static types* inner = nullptr;
        return &inner;
    }

    void ME::_onAddSubClass(const type& subClass) {
        super::_onAddSubClass(subClass);

        types** leafs = _onGetLeafs();
        if(*leafs) this->_setLeafs(nullptr);
    }

    const std::string& ME::_getNativeName() const {
        static const std::string inner = "void";
        return inner;
    }

#undef TEMPL
#undef ME
#undef SUPER
} // namespace by
