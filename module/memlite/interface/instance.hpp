/// @file
#pragma once

#include <map>

#include "memlite/interface/clonable.hpp"
#include "memlite/interface/id.hpp"

struct byeolTest;

namespace by {

    class life;
    class instancer;

    /** @ingroup memlite
     *  @brief Base class for all managed instances in memlite system
     *  @details Objects managed by memlite's custom memory pool must inherit from instance.
     *  Only by inheriting instance can objects be referenced through @ref binder as weak
     *  or strong pointers. Instances are identified through @ref id.
     *
     *  @section ID assignment algorithm
     *  The weakest part of memlite is this id assignment algorithm. Instance creation is
     *  done by @ref instancer which manages the memory pool. The instancer pushes the
     *  instance address and id to a map in the internal vault class. When
     *  instance::operator new() is called, it internally accesses vault using code like
     *  `map[this]` to retrieve the id value.
     *
     *  While seemingly fine, it has many disadvantages:
     *  1. Accessing vault during constructor phase is not truly safe
     *  2. It's slow
     *
     *  The initial implementation used only vector managed as FIFO, but when creating other
     *  objects inside a constructor, the order of added ids no longer follows FIFO,
     *  causing ID混乱 issues.
     *
     *  @remark Performance importance
     *  The most frequent operation in byeol is creating objects while assigning ids or
     *  performing binding. This needs improvement when time permits.
     */
    class _nout instance: public typeProvidable, public clonable {
        BY_ME(instance)
        BY_INIT_META(me)
        friend class life;
        friend class watcher;      // for vault.
        friend class chunks;       // for vault.
        friend class instancer;    // for _id.
        friend struct ::byeolTest; // for vault.

    public:
        class vault {
        public:
            void set(void* ptr, nidx chkN);
            nidx get(void* ptr);
            ncnt len() const;
            nbool rel();
            std::map<void*, int>& getVaults();

        private:
            std::map<void*, int> _vaults;
        };

        //  instance:
        instance();
        explicit instance(id newId);
        instance(const me& rhs);
        virtual ~instance();

    public:
        me& operator=(const me& rhs) = default;
        void* operator new(size_t sz) noexcept;
        void operator delete(void* pt, size_t sz) noexcept;

    public:
        virtual id getId() const;
        /**
         * @brief Checks if this instance is allocated on the heap.
         * @return true if the instance is heap-allocated, false otherwise.
         * @note This check is typically related to how the instance is managed by the memory pool.
         */
        virtual nbool isHeap() const;
        /**
         * @brief Returns a pointer to the associated life object for reference counting.
         * @details The `life` object is used by `binder` and `watcher` to manage the
         *          instance's lifecycle.
         * @return A const pointer to the `life` object, or nullptr if none is associated.
         */
        const life* getBindTag() const;

    protected:
        //  instance:
        nbool _setId(id new1);
        static instancer* _getMgr();

    private:
        id _id;
        static vault _vault;
    };
} // namespace by
