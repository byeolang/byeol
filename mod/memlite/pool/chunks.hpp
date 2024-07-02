#pragma once

#include "chunk.hpp"

namespace nm {

    class _nout chunks : public allocator {
        NAMU_ME(chunks, allocator)
        NAMU_INIT_META(me)

    public:
        //  chunks:
        explicit chunks(ncnt blkbyte = 0);
        virtual ~chunks();

    public:
        chunk& operator[](nidx n);
        const chunk& operator[](nidx n) const NAMU_UNCONST_FUNC(operator[](n))
        chunk& operator[](const instance& inst);
        const chunk& operator[](const instance& inst) const NAMU_UNCONST_FUNC(operator[](inst))

    public:
        //  chunks:
        chunk& get(nidx n);
        chunk& get(const instance& it);
        const chunk& get(nidx n) const NAMU_UNCONST_FUNC(get(n))
        const chunk& get(const instance& it) const NAMU_UNCONST_FUNC(get(it))
        //  allocator:
        void* new1() override;
        nbool del(void* pt, ncnt sz) override;
        /// @remark @ref chunk can resize its data. but can't persist whole memory allocated before,
        ///         it's a kind of memory flashing and can't give a way for accessing it.
        ///         at outside, ptr for them should be daggled.
        virtual nbool resize(ncnt new1);
        //  MemoryHaver:
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

        std::vector<chunk*> _chunks;
        int _s;
    };
}
