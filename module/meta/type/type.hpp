#pragma once

#include "meta/common.hpp"

namespace nm {

    /// @remark type returning ttype<type> as result of getType()
    class _nout type {
        NM_ME(type)

    public:
        virtual ~type() {}

    public:
        virtual nbool operator==(const me& rhs) const;
        nbool operator!=(const me& rhs) const;

    public:
        virtual nbool isTemplate() const = 0;
        virtual nbool isAbstract() const = 0;
        virtual const std::string& getName() const;

        /// @brief  create an instance to be refered this type.
        /// @remark available when the type defines a ctor without any params.
        /// @return return an address of new instance, however, if ctor without any params
        ///         isn't defined, then returns null.
        virtual void* make() const = 0;

        template <typename T> T* makeAs() const {
            // c++ is not allow for covariant against void*.
            // that's is why I make another variant func of already made make().
            return (T*) make();
        }

        virtual ncnt size() const = 0;
        virtual nbool init();
        virtual nbool rel();
        virtual const type& getSuper() const = 0;
        virtual const nbool& isInit() const = 0;

        /// returns all most derived class from this class.
        const types& getLeafs() const;
        const types& getSubs() const;
        const types& getSupers() const;

        virtual nbool isSuper(const type& it) const;
        nbool isSuper(const type* it) const NM_SIDE_FUNC(isSuper);
        template <typename T> nbool isSuper() const;
        nbool isSub(const type& it) const;
        nbool isSub(const type* it) const NM_SIDE_FUNC(isSub);
        template <typename T> nbool isSub() const;
        const type& getStatic() const NM_CONST_FUNC(_getStatic())

        /// this returns metaTypename.
        /// metaTypename can be used like 'dynamic_cast<yourType>'.
        /// as you may know, c++'s dynamic_cast is slow. because normally compilers tries to loop
        /// in order to figure out which type is fit to given your type parameter.
        /// 'meta' library, however, uses 'tier' algorithm and it's O(1), so it's faster.
        ///
        /// you can use your own metaType to represent more data on type class.
        /// for instance, 'core' module uses 'ntype' custom type class.
        /// but in that case, when you compare custom type class, you must compare extended data to
        /// 'rhs' variable to base type class, 'type'.
        ///
        /// so how can you know that 'type' is actually instance of your derived custom type class
        /// in 'tier' algorithm? please don't think about 'dynamic_cast'. it'll vanish our
        /// effectiveness to use 'tier' algorithm. that's why I make 'getMetaTypeName()' func.
        ///
        /// @return static literal c-style string for meta type name.
        ///         so you are able to use c-style casting if address of 'rhs' variables's
        ///         getMetaTypeName() isn just same to yours. e.g.
        ///             yourType& a = ....;
        ///             type& rhs = ....;
        ///             if(a.getMetaTypeName() != rhs.getMetaTypeName()) return;
        ///             yourType& rhsCasted = (yourType&) rhs;
        ///
        ///                ...now you can do something on yourType's data...
        ///
        virtual const nchar* getMetaTypeName() const;

    protected:
        //  type:
        virtual types& _getSubs() = 0;
        virtual types& _getSupers() = 0;
        virtual type& _getStatic() const = 0;
        void _setInit(nbool newState);
        virtual void _onAddSubClass(const me& subClass);
        virtual types** _onGetLeafs() const = 0;
        void _setLeafs(types* newLeafs) const;
        virtual const std::string& _getNativeName() const;

    private:
        nbool _logInitOk(nbool res);
        void _findLeafs(const type& cls, types& tray) const;
    };
} // namespace nm
