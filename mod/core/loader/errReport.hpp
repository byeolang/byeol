#pragma once

#include "err.hpp"
#include "../type/ntype.hpp"
#include "../type/dumpable.hpp"
#include "../common.hpp"

namespace namu {

    class _nout errReport : public instance, public dumpable {
        NAMU(CLASS(errReport, instance))

    public:
        nbool operator==(const me& rhs) const;
        nbool operator!=(const me& rhs) const;
        const err& operator[](nidx n) const;
        operator nbool() const;

    public:
        nbool hasErr() const;
        nbool hasErr(nidx since) const;
        nbool hasWarn() const;
        nbool hasWarn(nidx since) const;
        nbool has(logLv::level type) const;
        nbool has(logLv::level type, nidx since) const;
        nbool has(errCode code) const;

        const err& get(nidx n) const;

        ncnt len() const;

        virtual const err& add(const err* new1);
        const err& add(const err& new1);
        void add(const me& rhs);

        std::vector<tstr<err>>::const_iterator begin() const;
        std::vector<tstr<err>>::const_iterator last() const;
        std::vector<tstr<err>>::const_iterator end() const;

        void log(nidx since) const;
        void log() const;
        virtual void dump(nidx since) const;
        void dump() const override;

        void rel();

    private:
        std::vector<tstr<err>> _errs;
    };

    class _nout dummyErrReport : public errReport {
        NAMU(CLASS(dummyErrReport, errReport))

    public:
        const err& add(const err* new1) override;

        static me singletone;
    };
}
