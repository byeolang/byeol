/// @file
#pragma once

#include "memlite/pool/chunks.hpp"

namespace by {

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
