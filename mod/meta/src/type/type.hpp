#pragma once

#include "../common.hpp"

namespace wrd {
    /// @remark type returning Ttype<type> as result of getType()
    ///         because this func always returns metaclass no matter of what me type is,
    ///         users need to care about getting meta of metaclass on calling getType().
    ///         for example,
    ///             Thing& thing1 = ...; // let's assume that got from outside.
    ///             Object obj;
    ///
    ///             wbool compare = obj.isSub(thing1.getType()); // user intend to get class of Thing.
    ///             // however, value 'compare' will definitely be false if
    ///             // thing was actually a retrived one by calling Thing.getType() before.
    ///
    ///             // because type::getType() will return Ttype<Ttype<T> >,
    ///             // that stmt will be translated that checks object vs Ttype<T>.
    class type {
        WRD_DECL_ME(type)

    public:
        virtual ~type() {}

        wbool operator==(const me& rhs) const;
        wbool operator!=(const me& rhs) const;

        virtual wbool isTemplate() const = 0;
        virtual wbool isAbstract() const = 0;
        virtual const std::string& getName() const = 0;

        /// @brief  create an instance to be refered this type.
        /// @remark available when the type defines a ctor without any params.
        /// @return return an address of new instance, however, if ctor without any params
        ///         isn't defined, then returns null.
        virtual void* make() const = 0;
        template <typename T>
        T* makeAs() const {
            return (T*) make();
        }

        virtual wcnt size() const = 0;
        virtual wbool init();
        virtual wbool rel();
        virtual const type& getSuper() const = 0;
        virtual const wbool& isInit() const = 0;

        /// returns all most derived class from this class.
        const types& getLeafs() const;
        const types& getSubs() const {
            return (const_cast<me*>(this))->_getSubs();
        }
        const types& getSupers() const {
            return (const_cast<me*>(this))->_getSupers();
        }

        wbool isSuper(const type& it) const;
        template <typename T> wbool isSuper() const;
        wbool isSub(const type& it) const { return it.isSuper(*this); }
        template <typename T> wbool isSub() const;
        const type& getStatic() const WRD_UNCONST_FUNC(_getStatic())

    protected:
        //  type:
        virtual types& _getSubs() = 0;
        virtual types& _getSupers() = 0;
        virtual type& _getStatic() const = 0;
        void _setInit(wbool newState) {
            const wbool& res = isInit();
            wbool& isInit = const_cast<wbool&>(res);

            isInit = newState;
        }

        virtual void _onAddSubClass(const me& subClass);
        virtual types** _onGetLeafs() const = 0;
        void _setLeafs(types* newLeafs) const {
            types** leafs = _onGetLeafs();
            if(*leafs == newLeafs) return;

            if(*leafs)
                delete *leafs;

            *leafs = newLeafs;
        }
        void foo() {}

    private:
        wbool _logInitOk(wbool res);
        void _findLeafs(const type& cls, types& tray) const;
    };
}
