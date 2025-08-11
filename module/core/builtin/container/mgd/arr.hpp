#pragma once

#include "core/bridge/cpp/tbridge.hpp"

struct arrTest;

namespace by {

    class _nout arr: public tbridge<narr>, public tucontainable<node>, public tarrayable<node> {
        // arr uses instance variable 'ntype':
        //  ntype contains beanType as 'const type*' instance variable. so user should be
        //  careful when calling ttype<arr>. because it will also return ntype instance
        //  as a consequences, but it won't contain beanType.
        //
        //  the most appropriate getter for ntype of arr is to call getType() of instance
        //  to arr.
        BY(ME(arr, tbridge<narr>), INIT_META(arr), CLONE(arr), VISIT())

    public:
        typedef typename tucontainable<node>::iter iter;
        typedef typename tucontainable<node>::iteration iteration;
        typedef std::map<const node*, tstr<scope>> cache;
        friend class interpreter; // for _cache
        friend struct ::arrTest;

    public:
        arr();
        arr(const node& elemType);
        arr(const me& rhs);

    public:
        using tarrayable<node>::operator[];
        node& operator[](nidx n) override;

    public:
        const ntype& getType() const override;

        using super::subs;
        scope& subs() override;

        ncnt len() const override;

        using super::in;
        nbool in(nidx n) const override;

        //  get:
        using tarrayable<node>::get;
        using super::get;
        using tucontainable<node>::get;

        /* TODO: can I remove this?
        template <typename E> E* get(std::function<nbool(const E&)> l) const {
            for(const node* elem : *this) {
                const E& cast = elem->template cast<E>() OR_CONTINUE;
                if(l(cast)) // elem should be typeProvidable.
                    return cast;
            }

            return nullptr;
        }
        node& get(std::function<nbool(const node&)> l) const;

        template <typename E> tnarr<E, strTactic> getAll(std::function<nbool(const E&)> l) const {
            tnarr<E> ret;
            for(const node& elem: *this) {
                const E* cast = elem.template cast<E>();
                if(cast && l(*cast)) ret.add(cast);
            }

            return ret;
        }
        narr getAll(std::function<nbool(const node&)> l) const;
        */

        node* get(nidx n) override;

        //  set:
        using tucontainable<node>::set;
        using tarrayable<node>::set;
        nbool set(const iter& at, const node& new1) override;
        nbool set(nidx n, const node& new1) override;

        //  add:
        using tucontainable<node>::add;
        using tarrayable<node>::add;
        nbool add(const iter& at, const node& new1) override;
        nbool add(nidx n, const node& new1) override;
        void add(const iter& here, const iter& from, const iter& to) override;

        //  del:
        using tucontainable<node>::del;
        nbool del(nidx n) override;
        nbool del(const iter& it) override;
        nbool del(const iter& from, const iter& to) override;

        //  etc:
        void rel() override;

        const baseObj& getOrigin() const override;

        std::string asStr() const;

    protected:
        iteration* _onMakeIteration(ncnt step, nbool isReversed) const override;

    private:
        scope& _defGeneric(const baseObj& paramType);
        scope& _getOriginScope();

    private:
        static inline cache _cache;
        mutable str _org;
        mgdType _type;
    };
} // namespace by
