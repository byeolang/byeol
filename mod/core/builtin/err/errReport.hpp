#pragma once

#include "../../common.hpp"
#include "../../type/dumpable.hpp"
#include "../../type/ntype.hpp"
#include "baseErr.hpp"

namespace nm {

    class _nout errReport: public instance, public dumpable {
        NM(CLASS(errReport, instance))

    public:
        nbool operator==(const me& rhs) const;
        nbool operator!=(const me& rhs) const;
        const baseErr& operator[](nidx n) const;
        operator nbool() const;

    public:
        nbool hasErr() const;
        nbool hasErr(nidx since) const;
        nbool hasWarn() const;
        nbool hasWarn(nidx since) const;
        nbool has(logLv::level type) const;
        nbool has(logLv::level type, nidx since) const;

        const baseErr& get(nidx n) const;

        ncnt len() const;

        virtual const baseErr& add(const baseErr* new1);
        const baseErr& add(const baseErr& new1);
        void add(const me& rhs);

        std::vector<tstr<baseErr>>::const_iterator begin() const;
        std::vector<tstr<baseErr>>::const_iterator last() const;
        std::vector<tstr<baseErr>>::const_iterator end() const;

        void log(nidx since) const;
        void log() const;
        virtual void dump(nidx since) const;
        void dump() const override;

        void rel();

    private:
        std::vector<tstr<baseErr>> _errs;
    };

    class _nout dummyErrReport: public errReport {
        NM(CLASS(dummyErrReport, errReport))

    public:
        const baseErr& add(const baseErr* new1) override;

        static me singletone;
    };
} // namespace nm
