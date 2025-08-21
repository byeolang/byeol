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

    /// @ingroup core
    /// @brief Native type system for byeol language
    /// @details Core type class that provides type information, type conversion, and type deduction.
    /// Supports implicit/explicit casting, type compatibility checking, and parameter management.
    class _nout ntype: public type {
        BY_ME(ntype, type)
        typedef std::map<const ntype*, const ntype*> deducer;
        typedef std::map<const ntype*, deducer> deducers;
        friend class seq;
        friend class arr;
        friend class genericOrigin;

    public:
        ntype() = default;

    public:
        nbool operator==(const type& rhs) const override;

    public:
        // ntype:
        /// whether variable 'it' can be a subtype of T
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

        /// @return whether this's a custom type.
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
        /// @return null if it's not relative between l & r.
        const ntype* deduce(const ntype& r) const;
        const ntype* deduce(const ntype* it) const BY_SIDE_FUNC(deduce);
        const ntype* deduce(const typeProvidable& r) const;
        const ntype* deduce(const typeProvidable* it) const BY_SIDE_FUNC(deduce);

        template <typename T> const ntype* deduce() const { return deduce(ttype<T>::get()); }

        /// @return null it it's not relative between l & r.
        static const ntype* deduce(const ntype& l, const ntype& r);
        static const ntype* deduce(const ntype& l, const ntype* r) BY_SIDE_FUNC(r, deduce(l, *r), nullptr);
        static const ntype* deduce(const ntype* l, const ntype& r) BY_SIDE_FUNC(l, deduce(*l, r), nullptr);
        static const ntype* deduce(const ntype* l, const ntype* r) BY_SIDE_FUNC(l&& r, deduce(*l, *r), nullptr);

        const params& getParams() const BY_CONST_FUNC(getParams())
        virtual params& getParams();

        virtual const node* getRet() const;
        virtual void setRet(const node& new1);
        void setRet(const node* it) BY_SIDE_FUNC(setRet);

        std::string createNameWithParams() const;

        const nchar* getMetaTypeName() const override;

    protected:
        // ntype:
        virtual const impliAses& _getImpliAses() const;
        virtual const ases& _getAses() const;

    private:
        static deducers* _makeDeducers();
        static const ntype& _deduceSuperType(const ntype& l, const ntype& r);

    public:
        const static inline std::string META_TYPENAME = "ntype";
    };

    typedef std::vector<const ntype*> ntypes;
} // namespace by
