/// @file
#pragma once

#include "core/common.hpp"
#include "core/type/ntype.hpp"
#include "core/builtin/err/baseErr.hpp"

namespace by {

    /** @ingroup core
     *  @brief Error report collector
     *  @details A class that holds @ref baseErr objects. When there's logic that takes a long time or performs complex
     *  operations, function depth becomes very deep. When errors occur in inner functions, use errReport to record
     *  errors and continue work as much as possible while recording that errors occurred.
     *
     *  @section container_section Container
     *  Since errReport's almost all functionality is managing @ref baseErr objects, it has an interface similar to
     *  @ref tucontainable.
     *
     *  @section noisy_section Noisy
     *  Some errReports want to automatically perform log() each time a new err object is added. In this case, specify
     *  `setNoisy(true)`.
     */
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

        /**
         * @param code specific errCode to check.
         *             if you pass NOT_SPECIFIED, then all errors can be candidate.
         */
        nbool inErr(errCode code) const;
        nbool inErr(errCode code, nidx since) const;
        nbool inWarn() const;

        /**
         * @param code specific errCode to check.
         *             if you pass NOT_SPECIFIED, then all errors can be candidate.
         */
        nbool inWarn(errCode code) const;
        nbool inWarn(errCode code, nidx since) const;
        nbool in(errLv::level type) const;
        nbool in(errLv::level type, errCode code) const;
        nbool in(errLv::level type, errCode code, nidx since) const;

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
        virtual me& setNoisy(nbool isNoisy);

    private:
        void _noise(const baseErr& new1);

    private:
        std::vector<tstr<baseErr>> _errs;

        /**
         *  if _isNoisy is true, an error is logged immediately whenever an err instance is added to
         *  errReport.
         */
        nbool _isNoisy;
    };

    /** @ingroup core
     *  @brief Dummy error reporter that discards all errors
     *  @details No-op error reporter used when error collection is not needed.
     */
    class _nout dummyErrReport: public errReport {
        BY(CLASS(dummyErrReport, errReport))

    public:
        dummyErrReport();

    public:
        const baseErr& add(const baseErr& new1) override;

        super& setNoisy(nbool isNoisy) override;

        static me singleton;
    };
} // namespace by
