/// @file
#pragma once

#include "memlite/pool/pool.hpp"
#include "memlite/watcher/watcher.hpp"

namespace by {

    /** @ingroup memlite
     *  @brief Singleton manager for instance lifecycle and memory allocation
     *  @details Contains @ref pool class for low-level memory management and @ref watcher
     *  for managing @ref instance lifecycles. Controls both to manage instance lifecycle
     *  (allocation/destruction). Effectively serves as the entry point for dispatching work
     *  to control classes or issuing commands for core memlite operations.
     */
    class _nout instancer {
        BY_ME(instancer)
        BY_INIT_META(me)
        friend class instance;

    public:
        ~instancer();

    public:
        /**
         * @brief Binds instance for lifecycle management
         * @return true on success, false if already bound or binding fails
         * @note Allocates life object from watcher for reference counting
         */
        nbool bind(const instance& it);
        nbool bind(const instance* it) BY_SIDE_FUNC(bind);

        /**
         * @brief Releases instance from lifecycle management
         * @return true on success, false if not bound or release fails
         * @note Decrements reference count. Destroys instance when count reaches 0.
         */
        nbool rel(const instance& old);
        nbool rel(const instance* it) BY_SIDE_FUNC(rel);
        const pool& getPool() const;
        const watcher& getWatcher() const;

        /**
         * @return singleton instance. this returns nul of reference if app is about to be
         *         terminated.
         */
        static me* get();

    private:
        /**
         * @brief Allocates memory block from pool for new instance
         * @param sz Size in bytes to allocate
         * @return Pointer to allocated memory, or nullptr on failure
         * @note Called by instance::operator new(). Routes to pool for allocation.
         */
        void* _new1(size_t sz);

        /**
         * @brief Deallocates memory block back to pool
         * @param pt Pointer to memory to deallocate
         * @param sz Size in bytes of memory block
         * @note Called by instance::operator delete(). Returns memory to pool.
         */
        void _del(void* pt, ncnt sz);

        /**
         * @brief Checks if instance has life tag assigned
         * @return true if instance has associated life object, false otherwise
         * @note Used to verify if instance is properly bound before operations
         */
        nbool _hasBindTag(const instance& it) const;

    private:
        instancer() = default;

    private:
        static nbool _isRel;
        pool _pool;
        watcher _watcher;
    };
} // namespace by
