#pragma once

#include "../builtin/container/native/tnarr.hpp"

namespace nm {
    enum priorType {
        EXACT_MATCH = 0,      // lv0: exact match.
        NUMERIC_MATCH = 1,    // lv1: numeric match.
                              //      it's almost same level of lv0. except lv1 allows numeric implicit type cast ('byte to int' or reverse order)
        IMPLICIT_MATCH = 2,   // lv2: implicit cast match.
                              //      it's almost same level of lv1 except it allows all kind of implicit type cast.
        NO_MATCH,             // lv+: no match.
    };

    _nout const nchar* getPriorTypeName(priorType type);

    template <typename T>
    struct tprior: instance {
        NM(CLASS(tprior, instance))

    public:
        tprior(const node& newElem, priorType type, ncnt lv);

    public:
        T* operator->();
        T& operator*();
        const T* operator->() const NM_UNCONST_FUNC(operator->())
        const T& operator*() const NM_UNCONST_FUNC(operator*())

    public:
        T& get();
        const T& get() const NM_UNCONST_FUNC(get())

        /// @brief precedence is more detail concept of priority.
        ///        it considers the owner of them are equal above priority.
        nbool isSamePrecedence(const me& rhs) const;

    public:
        tstr<T> elem;
        priorType type;
        ncnt lv;
    };

    template <typename T>
    class tmatches : public tnarr<T> {
        NM(CLASS(tmatches, tnarr<T>))
        template <typename E> friend class tpriorities;

    public:
        tmatches();
        /// @param  elems   instances to derived type of T.
        ///                 should be created on Heap.
        template <typename... Es>
        explicit tmatches(const Es&... elems) {
            static_assert(areBaseOfT<T, Es...>::value, "some of type of args are not base of type 'T'");
            add( { (T*) &elems... } );
        }

    public:
        nbool isMatched() const;

        using super::get;
        T& get();
        const T& get() const NM_UNCONST_FUNC(get())

        /// @return priority of matched one.
        ///         this'll be NO_MATCH if isMatched() returns false.
        priorType getPriorType() const;

    protected:
        void _setPriorType(priorType new1);

    private:
        priorType _type;
    };

    template <typename T>
    class tpriorities : public tnarr<tprior<T>> {
        NM(CLASS(tpriorities, tprior<T>))

    public:
        tpriorities();
        tpriorities(const node& elem);

    public:
        /// @return finally matched sub when you want to access.
        ///         if there is any ambigious err, this will return nulOf<T>().
        tmatches<T> getMatches() const;
        tstr<T> getMatch();
        const tstr<T> getMatch() const NM_UNCONST_FUNC(getMatch())
        tpriorities split(priorType by) const;
    };

    typedef tpriorities<node> priorities;
}
