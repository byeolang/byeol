#pragma once

#include "../../../ast/node.hpp"
#include "tnucontainer.hpp"
#include "../tarrayable.hpp"
#include "../../primitive/nInt.hpp"

namespace namu {

    class _nout nseq : public tnucontainer<nInt>, public tarrayable<nInt> {
        NAMU(CLASS(nseq, tnucontainer<nInt>))
        friend class seq;

    public:
#include "../iter/nseqIteration.hpp"

    public:
        nseq(nInt start, nInt end);
        nseq(nInt start, nInt end, nInt step);

    public:
        using tarrayable<nInt>::operator[];
        virtual nInt& operator[](nidx n) override;

    public:
        const nInt& getStart() const;
        const nInt& getEnd() const;
        const nInt& getStep() const;

        ncnt len() const override;

        void rel() override;

        nInt& get(nidx n) override;

        nbool has(nidx n) const override;

    protected:
        iteration* _onMakeIteration(ncnt step) const override;

    private:
        // these method has been prohibited.
        using super::add;
        using tarrayable<nInt>::add;
        nbool add(const iter& at, const nInt& new1) override;
        void add(const iter& at, const iter& from, const iter& to) override;
        nbool add(nidx n, const nInt& new1) override;
        using super::set;
        using tarrayable<nInt>::set;
        nbool set(const iter& at, const nInt& new1) override;
        nbool set(nidx n, const nInt& new1) override;
        using super::del;
        using tarrayable<nInt>::del;
        nbool del(const iter& it) override;
        nbool del(const iter& from, const iter& end) override;
        nbool del(nidx n) override;

        void _updateStep();

    private:
        nInt _start;
        nInt _end;
        nInt _step;
    };
}
