#pragma once

#include "../common.hpp"
#include "as/ases.hpp"

namespace nm {

    class node;
    template <typename T, typename TACTIC> class tnarr;
    typedef tnarr<node, strTactic> narr;
    class params;

    class _nout ntype: public type {
        NM_ME(ntype, type)
        typedef std::map<const ntype*, const ntype*> deducer;
        typedef std::map<const ntype*, deducer> deducers;
        friend class seq;
        friend class arr;
        friend class genericOrigin;

    public:
        ntype();
        ntype(const me& rhs);

        ~ntype() override;

    public:
        nbool operator==(const type& rhs) const override;
        me& operator=(const me& rhs);

    public:
        // ntype:
        /// whether variable 'it' can be a subtype of T
        template <typename T> nbool isImpli() const { return this->isImpli(ttype<T>::get()); }

        virtual nbool isImpli(const type& to) const;
        nbool isImpli(const typeProvidable& to) const;

        template <typename T> tstr<T> asImpli(const node& it) const {
            return this->asImpli(ttype<T>::get());
        }

        virtual str asImpli(const node& from, const type& to) const;

        /// @return whether this's a custom type.
        virtual nbool isCustom() const { return false; }

        template <typename T> nbool is() const { return this->is(ttype<T>::get()); }

        nbool is(const type& to) const;

        template <typename T> tstr<T> as(const node& it) const {
            return this->as(it, ttype<T>::get());
        }

        str as(const node& from, const type& to) const;

        virtual nbool isImmutable() const;
        /// @return null if it's not relative between l & r.
        const ntype& deduce(const ntype& r) const;
        const ntype& deduce(const typeProvidable& r) const;

        template <typename T> const ntype& deduce() const { return deduce(ttype<T>::get()); }

        /// @return null it it's not relative between l & r.
        static const ntype& deduce(const ntype& l, const ntype& r);

        virtual const params& getParams() const NM_CONST_FUNC(_getParams())

    protected:
        // ntype:
        virtual const ases& _getImpliAses() const;
        virtual const ases& _getAses() const;
        params& _getParams();

    private:
        static deducers* _makeDeducers();
        static const ntype& _deduceSuperType(const ntype& l, const ntype& r);
        me& _assign(const me& rhs);

    private:
        params* _params;
    };

    typedef std::vector<const ntype*> ntypes;
} // namespace nm
