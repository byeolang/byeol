/// @file
#pragma once

#include "memlite/pool/chunks.hpp"

namespace by {

    /// @ingroup memlite
    /// @brief Memory pool manager for efficient instance allocation
    /// @details The first class to handle memory allocation requests from external sources.
    /// Internally has an array of @ref chunks, and since chunks has @ref chunk, effectively
    /// manages all low-level memory-related classes.
    ///
    /// @section pool lazily has chunks per allocatable size
    /// A key point in creating a custom memory pool is increasing speed by listing same-size
    /// memory in one place. chunks has a block concept where each block can only allocate/
    /// deallocate pre-specified sized memory. pool creates chunks with fixed block size.
    /// When external requests memory allocation of a specific size, pool finds chunks
    /// handling that block size. If none exist, creates lazily.
    class _nout pool: public memoryHaver {
        BY_ME(pool, memoryHaver)
        BY_INIT_META(me)

    public:
        //  pool:
        pool();
        virtual ~pool();

    public:
        chunks* get(const instance& it);
        chunks* get(const instance* it) BY_SIDE_FUNC(get);
        chunks* get(nidx n);
        const chunks* get(const instance& it) const BY_CONST_FUNC(get(it))
        const chunks* get(const instance* it) const BY_CONST_FUNC(get(it))
        const chunks* get(nidx n) const BY_CONST_FUNC(get(n))

        using super::has;
        nbool has(const instance& it) const override;

        ncnt size() const override;
        ncnt len() const override;
        nbool rel() override;

    protected:
        //  memoryHaver:
        void* _get(nidx n) override;

    private:
        std::vector<chunks*> _chunks;
    };
} // namespace by
