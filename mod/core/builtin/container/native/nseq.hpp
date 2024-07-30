#pragma once

#include "../../../ast/node.hpp"
#include "../../primitive/nInt.hpp"
#include "../tarrayable.hpp"
#include "tnucontainer.hpp"

namespace nm {

    class _nout nseq: public tnucontainer<nInt, nInt>, public tarrayable<nInt, nInt> {
        typedef tnucontainer<nInt, nInt> _super3;
        NM(CLASS(nseq, _super3))
        friend class seq;

    public:
#include "../iter/nseqIteration.hpp"

    public:
        nseq(const nInt& start, const nInt& end);
        nseq(const nInt& start, const nInt& end, const nInt& step);

    public:
        using tarrayable<nInt, nInt>::operator[];
        virtual nInt operator[](nidx n) override;

    public:
        const nInt& getStart() const;
        const nInt& getEnd() const;
        const nInt& getStep() const;

        ncnt len() const override;

        void rel() override;

        using super::get;
        nInt get(nidx n) override;

        nbool has(nidx n) const override;

    protected:
        iteration* _onMakeIteration(ncnt step) const override;

    private:
        // these method has been prohibited.
        using super::add;
        using tarrayable<nInt, nInt>::add;
        nbool add(const iter& at, const nInt& new1) override;
        void add(const iter& at, const iter& from, const iter& to) override;
        nbool add(nidx n, const nInt& new1) override;
        using super::set;
        using tarrayable<nInt, nInt>::set;
        nbool set(const iter& at, const nInt& new1) override;
        nbool set(nidx n, const nInt& new1) override;
        using super::del;
        using tarrayable<nInt, nInt>::del;
        nbool del(const iter& it) override;
        nbool del(const iter& from, const iter& end) override;
        nbool del(nidx n) override;

        void _updateStep();

    private:
        nInt _start;
        nInt _end;
        nInt _step;
    };
} // namespace nm
