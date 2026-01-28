/// @file
#pragma once

#include "memlite/binder/tbindable.hpp"
#include "memlite/interface/instance.hpp"

namespace by {

    class chunk;

    /** @ingroup memlite
     *  @brief Binding tag for instance reference tracking and lifecycle management
     *  @details While @ref pool manages memory from a low-level block perspective, the
     *  @ref watcher component organically manages information for each block. See watcher
     *  first for details. life has an address value (_pt) allocated in pool and values for
     *  reference counting.
     *
     *  _strong is for reference counting, _pt directly points to an instance allocated in
     *  pool. _id is for identifying objects, see @ref id for details.
     */
    class _nout life: public typeProvidable, public tbindable<instance> {
        BY_ME(life)
        BY_INIT_META(me)
        template <typename T, typename TACTIC> friend class tweak;
        template <typename T, typename TACTIC> friend class tstr;
        friend class weakTactic;
        friend class strTactic;

    public:
        //  life:
        life();
        explicit life(id newId);
        ~life();

    public:
        instance* operator->();
        instance& operator*();
        const instance* operator->() const BY_CONST_FUNC(operator->())
        const instance& operator*() const BY_CONST_FUNC(operator*())

    public:
        /**
         * @brief Returns the memory chunk associated with this life object.
         * @return A const pointer to the chunk.
         */
        const chunk* getChunk() const;

        /**
         * @brief Returns current strong reference count
         * @return Number of strong references (tstr binders) pointing to this instance
         */
        ncnt getStrongCnt() const;

        //  tbindable:
        /**
         * @brief Releases the binding for this life object, decrementing the strong reference count.
         * @details When the strong count reaches zero, the associated instance is typically destroyed
         *          via the instancer.
         */
        void rel() override;
        nbool isBind() const override;
        const type& getBindable() const;

        instance* get();
        const instance* get() const BY_CONST_FUNC(get())

        template <typename E> E* get() { return get() TO(template cast<E>()); }
        template <typename E> const E* get() const BY_CONST_FUNC(get<E>())

        using tbindable<instance>::canBind;
        nbool canBind(const type& cls) const override;

        using tbindable<instance>::bind;

        /**
         * @brief Binds instance to this life object for lifecycle tracking
         * @return true on success, false if already bound or type mismatch
         * @note Initializes _pt pointer and sets up reference counting. Called by instancer.
         */
        nbool bind(const instance& new1) override;

        //  Instance:
        id getId() const;
        //  typeProvidable:
        const type& getType() const override;

        /**
         * @brief Retrieves life object associated with given id
         * @return Pointer to life object, or nullptr if not found
         * @note Used to lookup life objects from watcher by instance id
         */
        static const life* getBindTag(id newId);

    private:
        //  life:
        /**
         * @brief Updates strong reference count by vote amount
         * @param vote Positive to increment, negative to decrement reference count
         * @return true on success, false on error
         * @note When count reaches 0, triggers instance destruction via instancer
         */
        nbool _onStrong(ncnt vote);

        /**
         * @brief Completes id assignment for newly bound instance
         * @return true on success, false on failure
         * @note Sets instance's internal id after binding completes
         */
        nbool _completeId(instance& it);

        /**
         * @brief Synchronizes life object with new id
         * @return true on success, false if already synced or invalid
         * @note Internal synchronization for id changes
         */
        nbool _sync(id new1);

    private:
        instance* _pt;
        ncnt _strong;
        id _id;
    };
} // namespace by
