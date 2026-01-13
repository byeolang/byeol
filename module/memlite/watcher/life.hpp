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
        const chunk* getChunk() const;
        ncnt getStrongCnt() const;
        //  tbindable:
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
        nbool bind(const instance& new1) override;

        //  Instance:
        id getId() const;
        //  typeProvidable:
        const type& getType() const override;
        static const life* getBindTag(id newId);

    private:
        //  life:
        nbool _onStrong(ncnt vote);
        nbool _completeId(instance& it);
        nbool _sync(id new1);

    private:
        instance* _pt;
        ncnt _strong;
        id _id;
    };
} // namespace by
