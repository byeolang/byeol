/// @file
#pragma once

#include "core/builtin/container/native/tnarr.hpp"

namespace by {
    /**
     *  the lower value, the higher priority.
     */
    enum priorType {
        EXACT_MATCH = 0,    /// lv0: exact match.
        NUMERIC_MATCH = 1,  /// lv1: numeric match.
                            ///      it's almost same level of lv0. except lv1 allows numeric implicit
                            ///      type cast (e.g. @ref nByte "byte" to @ref nInt "int" or reverse order)
        IMPLICIT_MATCH = 2, /// lv2: implicit cast match.
                            ///      it's almost same level of lv1 except it allows all kind of
                            ///      implicit type cast.
        NO_MATCH,           /// lv+: no match.
    };

    const _nout nchar* getPriorTypeName(priorType type);

    /** @ingroup core
     *  @brief Priority wrapper for type matching
     *  @details Template that wraps elements with priority information for overload resolution.
     */
    template <typename T> struct tprior: instance {
        BY(CLASS(tprior, instance))

    public:
        tprior(const node& newElem, priorType type, ncnt lv);

    public:
        T* operator->();
        T& operator*();
        const T* operator->() const BY_CONST_FUNC(operator->())
        const T& operator*() const BY_CONST_FUNC(operator*())

    public:
        T* get();
        const T* get() const BY_CONST_FUNC(get())

        /**
         *  @brief precedence is more detail concept of priority.
         *         it considers the owner of them are equal above priority.
         */
        nbool isSamePrecedence(const me& rhs) const;

    public:
        tstr<T> elem;
        priorType type;
        ncnt lv;
    };

    /** @ingroup core
     *  @brief Collection of prioritized elements
     *  @details Container for managing elements with priority information for overload resolution.
     */
    template <typename T> class tpriorities: public tnarr<T> {
        BY(CLASS(tpriorities, tnarr<T>))

    public:
        tpriorities();

        template <typename... Es> tpriorities(const Es&... elems) {
            static_assert(areBaseOfT<T, Es...>::value, "some of type of args are not based on type 'T'");
            this->add({(T&) elems...});
        }

    public:
        nbool isMatched() const;

        using super::get;
        T* get();
        const T* get() const BY_CONST_FUNC(get())

        /**
         *  @return priority of matched one.
         *          this'll be NO_MATCH if isMatched() returns false.
         */
        priorType getPriorType() const;
        void setPriorType(priorType new1);

    private:
        priorType _type;
    };

    typedef tpriorities<node> priorities;
} // namespace by
