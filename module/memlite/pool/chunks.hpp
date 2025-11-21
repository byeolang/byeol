/// @file
#pragma once

#include "memlite/pool/chunk.hpp"

namespace by {

    /// @ingroup memlite
    /// @brief Collection of memory chunks for scalable allocation
    /// @details Manages multiple memory chunks to provide scalable memory allocation.
    /// Automatically manages chunk creation and selection for optimal performance.
    class _nout chunks: public allocator {
        BY_ME(chunks, allocator)
        BY_INIT_META(me)

    public:
        //  chunks:
        explicit chunks(ncnt blkbyte = 0);
        virtual ~chunks();

    public:
        chunk& operator[](nidx n);
        const chunk& operator[](nidx n) const BY_CONST_FUNC(operator[](n));
        chunk& operator[](const instance& it);
        const chunk& operator[](const instance& it) const BY_CONST_FUNC(operator[](it));

    public:
        //  chunks:
        chunk* get(nidx n);
        chunk* get(const instance& it);
        chunk* get(const instance* it) BY_SIDE_FUNC(get);
        const chunk* get(nidx n) const BY_CONST_FUNC(get(n))
        const chunk* get(const instance& it) const BY_CONST_FUNC(get(it))
        const chunk* get(const instance* it) const BY_CONST_FUNC(get(it))

        //  allocator:
        void* new1() override;
        nbool del(void* pt, ncnt sz) override;
        /// @remark @ref chunk can resize its data. but can't persist whole memory allocated before,
        ///         it's a kind of memory flashing and can't give a way for accessing it.
        ///         at outside, ptr for them should be daggled.
        virtual nbool resize(ncnt new1);

        using super::has;
        nbool has(const instance& it) const override;

        ncnt len() const override;
        ncnt size() const override;
        nbool rel() override;

    private:
        //  chunks:
        nidx _findCapable();
        nbool _rel();
        //  MemoryHaver:
        void* _get(nidx n) override;

    private:
        std::vector<chunk*> _chunks;
        int _s;
    };
} // namespace by
