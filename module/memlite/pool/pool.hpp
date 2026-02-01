/// @file
#pragma once

#include "memlite/pool/chunks.hpp"

namespace by {

    /** @ingroup memlite
     *  @brief Memory pool manager for efficient instance allocation
     *  @details The first class to handle memory allocation requests from external sources.
     *  Internally has an array of @ref chunks, and since chunks has @ref chunk, effectively
     *  manages all low-level memory-related classes.
     *
     *  @section lazy_chunks_allocation pool lazily has chunks per allocatable size
     *  A key point in creating a custom memory pool is increasing speed by listing same-size
     *  memory in one place. chunks has a block concept where each block can only allocate/
     *  deallocate pre-specified sized memory. pool creates chunks with fixed block size.
     *  When external requests memory allocation of a specific size, pool finds chunks
     *  handling that block size. If none exist, creates lazily.
     */
    class _nout pool: public memoryHaver {
        BY_ME(pool, memoryHaver)
        BY_INIT_META(me)

    public:
        //  pool:
        pool();
        virtual ~pool();

    public:
        /**
         * @brief Gets chunks container managing memory for given instance
         * @return Pointer to chunks managing this instance's memory, or nullptr if not found
         * @note Finds chunks by instance size to locate appropriate memory block
         */
        chunks* get(const instance& it);
        chunks* get(const instance* it) BY_SIDE_FUNC(get);

        /**
         * @brief Gets chunks at specified index
         * @return Pointer to chunks at index n, or nullptr if out of bounds
         */
        chunks* get(nidx n);
        const chunks* get(const instance& it) const BY_CONST_FUNC(get(it))
        const chunks* get(const instance* it) const BY_CONST_FUNC(get(it))
        const chunks* get(nidx n) const BY_CONST_FUNC(get(n))

        using super::has;

        /**
         * @brief Checks if instance is managed by this pool
         * @return true if instance allocated from this pool, false otherwise
         */
        nbool has(const instance& it) const override;

        ncnt size() const override;
        ncnt len() const override;

        /**
         * @brief Releases all pool resources and destroys chunks
         * @return true on success, false on failure
         * @note Destroys all chunks and frees allocated memory
         */
        nbool rel() override;

    protected:
        //  memoryHaver:
        /**
         * @brief Gets raw memory pointer at index
         * @return Void pointer to memory at index n
         * @note Internal accessor for memoryHaver interface
         */
        void* _get(nidx n) override;

    private:
        std::vector<chunks*> _chunks;
    };
} // namespace by
