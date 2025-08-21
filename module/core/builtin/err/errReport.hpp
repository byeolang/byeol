/// @file
#pragma once

#include "core/common.hpp"
#include "core/type/ntype.hpp"
#include "core/builtin/err/baseErr.hpp"

namespace by {

    /// @ingroup core
    /// @brief Error reporting and collection system
    /// @details Collects and manages errors and warnings during compilation and runtime.
    /// Provides filtering, logging, and reporting capabilities for diagnostic messages.
    class _nout errReport: public instance, public dumpable {
        BY(CLASS(errReport, instance))

    public:
        errReport();
        errReport(nbool isNoisy);

    public:
        nbool operator==(const me& rhs) const;
        nbool operator!=(const me& rhs) const;
        const baseErr& operator[](nidx n) const;
        operator nbool() const;

    public:
        nbool inErr() const;
        nbool inErr(nidx since) const;
        nbool inWarn() const;
        nbool inWarn(nidx since) const;
        nbool in(errLv::level type) const;
        nbool in(errLv::level type, nidx since) const;

        const baseErr* get(nidx n) const;

        ncnt len() const;

        virtual const baseErr& add(const baseErr& new1);
        const baseErr* add(const baseErr* it) BY_SIDE_FUNC(it, &add(*it), nullptr);
        void add(const me& rhs);

        std::vector<tstr<baseErr>>::const_iterator begin() const;
        std::vector<tstr<baseErr>>::const_iterator last() const;
        std::vector<tstr<baseErr>>::const_iterator end() const;

        void log(nidx since) const;
        void log() const;
        virtual void dump(nidx since) const;
        void dump() const override;

        void rel();

        nbool isNoisy() const;
        me& setNoisy(nbool isNoisy);

    private:
        void _noise(const baseErr& new1);

    private:
        std::vector<tstr<baseErr>> _errs;

        /// if _isNoisy is true, an error is logged immediately whenever an err instance is added to
        /// errReport.
        nbool _isNoisy;
    };

    /// @ingroup core
    /// @brief Dummy error reporter that discards all errors
    /// @details No-op error reporter used when error collection is not needed.
    class _nout dummyErrReport: public errReport {
        BY(CLASS(dummyErrReport, errReport))

    public:
        const baseErr& add(const baseErr& new1) override;

        static me singleton;
    };
} // namespace by
