/// @file
#pragma once

#include "core/common.hpp"

namespace by {

    class node;
    template <typename T, typename TACTIC> class tnarr;
    typedef tnarr<node, strTactic> narr;
    class params;
    class ases;
    class impliAses;

    /** @ingroup core
     *  @brief represents native c++ type system for byeol language
     *  @details Core type class that provides type information, type conversion, and type deduction.
     *           Supports implicit/explicit casting, type compatibility checking, and parameter management.
     *           ntype stands for native type, representing meta-type information for C++ classes.
     *           mgdType stands for managed type, representing meta-type information for types defined
     *           in the Byeol language (such as functions or objects).
     *
     *           One important note:
     *           Although mgdType inherits from ntype in terms of implementation, this inheritance
     *           relationship does not apply at the type level when represented as type.
     *           In other words, when inheritance hierarchies such as getSupers() or getSubs() are represented,
     *           their contents can include both ntype and mgdType instances.
     *           mgdType::getSupers() is not restricted to containing only mgdType entries.
     *
     *           The name of an mgdType corresponds to the type name as defined in the Byeol language,
     *           while an ntype corresponds to the C++ class name.
     */
    class _nout ntype: public type {
        BY_ME(ntype, type)
        typedef std::map<const ntype*, const ntype*> promoter;
        typedef std::map<const ntype*, promoter> promoters;
        friend class seq;
        friend class arr;
        friend class genericOrigin;

    public:
        ntype() = default;

    public:
        /**
         *  @brief check whether two ntype are same in both of native and managed environments
         *  @details mgdType inherits ntype, so this operator==() is also able to checks equality in managed code.
         */
        nbool operator==(const type& rhs) const override;

    public:
        /**
         *  @brief whether variable 'it' can be a subtype of T in managed code
         */
        template <typename T> nbool isImpli() const { return this->isImpli(ttype<T>::get()); }

        virtual nbool isImpli(const type& to) const;
        nbool isImpli(const type* it) const BY_SIDE_FUNC(isImpli);
        nbool isImpli(const typeProvidable& to) const;
        nbool isImpli(const typeProvidable* it) const BY_SIDE_FUNC(isImpli);

        template <typename T> tstr<T> asImpli(const node& it) const { return this->asImpli(ttype<T>::get()); }

        template <typename T> tstr<T> asImpli(const node* it) const BY_SIDE_FUNC(asImpli<T>);

        virtual str asImpli(const node& from, const type& to) const;
        str asImpli(const node& from, const type* to) const BY_SIDE_FUNC(to, asImpli(from, *to), str());
        str asImpli(const node* from, const type& to) const BY_SIDE_FUNC(from, asImpli(*from, to), str());
        str asImpli(const node* from, const type* to) const BY_SIDE_FUNC(from&& to, asImpli(*from, *to), str());

        /**
         *  @return whether this's a custom type.
         */
        virtual nbool isCustom() const { return false; }

        template <typename T> nbool is() const { return this->is(ttype<T>::get()); }

        nbool is(const type& to) const;
        nbool is(const type* it) const BY_SIDE_FUNC(is);

        template <typename T> tstr<T> as(const node& it) const { return this->as(it, ttype<T>::get()); }

        template <typename T> tstr<T> as(const node* it) const BY_SIDE_FUNC(as<T>);

        str as(const node& from, const type& to) const;
        str as(const node& from, const type* to) const BY_SIDE_FUNC(to, as(from, *to), str());
        str as(const node* from, const type& to) const BY_SIDE_FUNC(from, as(*from, to), str());
        str as(const node* from, const type* to) const BY_SIDE_FUNC(from&& to, as(*from, *to), str());

        virtual nbool isImmutable() const;

        /**
         *  @return true if rhs has same params and return type in managed code
         *  @details operator==() compares type equality in the native (C++) environment — it returns true
         *           if both objects are of the same class.
         *           In contrast, isSameSign() compares type equality in the managed environment.
         */
        nbool isSameSign(const type& rhs) const;

        /**
         *  @return null if it's not relative between l & r.
         */
        const ntype* promote(const ntype& r) const;
        const ntype* promote(const ntype* it) const BY_SIDE_FUNC(promote);
        const ntype* promote(const typeProvidable& r) const;
        const ntype* promote(const typeProvidable* it) const BY_SIDE_FUNC(promote);

        template <typename T> const ntype* promote() const { return promote(ttype<T>::get()); }

        /**
         *  @return null it it's not relative between l & r.
         */
        static const ntype* promote(const ntype& l, const ntype& r);
        static const ntype* promote(const ntype& l, const ntype* r) BY_SIDE_FUNC(r, promote(l, *r), nullptr);
        static const ntype* promote(const ntype* l, const ntype& r) BY_SIDE_FUNC(l, promote(*l, r), nullptr);
        static const ntype* promote(const ntype* l, const ntype* r) BY_SIDE_FUNC(l&& r, promote(*l, *r), nullptr);

        const params& getParams() const BY_CONST_FUNC(getParams())
        virtual params& getParams();

        virtual const node* getRet() const;
        virtual void setRet(const node& new1);
        void setRet(const node* it) BY_SIDE_FUNC(setRet);

        std::string createNameWithParams() const;

        const nchar* getMetaTypeName() const override;

    protected:
        virtual const impliAses& _getImpliAses() const;
        virtual const ases& _getAses() const;

    private:
        static promoters* _makeDeducers();
        static const ntype& _promoteSuperType(const ntype& l, const ntype& r);

    public:
        const static inline std::string META_TYPENAME = "ntype";
    };

    typedef std::vector<const ntype*> ntypes;
} // namespace by
