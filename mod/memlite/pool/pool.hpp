#pragma once

#include "chunks.hpp"

namespace nm {

    class _nout pool: public memoryHaver {
        NM_ME(pool, memoryHaver)
        NM_INIT_META(me)

    public:
        //  pool:
        pool();
        virtual ~pool();

    public:
        chunks& operator[](nidx n);
        chunks& operator[](const instance& inst);
        const chunks& operator[](nidx n) const NM_CONST_FUNC(operator[](n))
        const chunks& operator[](const instance& inst) const NM_CONST_FUNC(operator[](inst))

    public:
        chunks& get(const instance& inst);
        chunks& get(nidx n);
        const chunks& get(const instance& inst) const NM_CONST_FUNC(get(inst))
        const chunks& get(nidx n) const NM_CONST_FUNC(get(n))
        //  memoryHaver:
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
} // namespace nm
