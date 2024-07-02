#pragma once

#include "chunks.hpp"

namespace nm {

    class _nout pool : public memoryHaver {
        NAMU_ME(pool, memoryHaver)
        NAMU_INIT_META(me)

    public:
        //  pool:
        pool();
        virtual ~pool();

    public:
        chunks& operator[](nidx n);
        chunks& operator[](const instance& inst);
        const chunks& operator[](nidx n) const NAMU_UNCONST_FUNC(operator[](n))
        const chunks& operator[](const instance& inst) const NAMU_UNCONST_FUNC(operator[](inst))

    public:
        chunks& get(const instance& inst);
        chunks& get(nidx n);
        const chunks& get(const instance& inst) const NAMU_UNCONST_FUNC(get(inst))
        const chunks& get(nidx n) const NAMU_UNCONST_FUNC(get(n))
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
}
