/// @file
#pragma once

#include "core/loader/pack/packLoading.hpp"
#include "core/loader/pack/packMakable.hpp"
#include "core/ast/statable.hpp"
#include "core/ast/scope.hpp"
#include "core/ast/slot.hpp"

namespace by {

    class visitor;

    /// @ingroup core
    /// @brief Automatic slot with pack loading capabilities
    /// @details @ref slot that automatically loads and parses @ref pack content when accessed.
    /// Integrates @ref pack loading strategies with lazy evaluation and @ref err reporting.
    class _nout autoslot: public slot, public packMakable, public statable {
        BY(CLASS(autoslot, slot), VISIT())

    public:
        autoslot(const manifest& manifest, const packLoadings& loadingsInHeap);
        ~autoslot() override;

    public:
        state getState() const override;
        void setState(state new1) override;

        using super::getPack;
        obj& getPack() override;

        void rel() override;

        /// this report will be used while interpret @ref pack source code.
        /// @remark when 'subs()' @ref func of the @ref pack has been called, it interprets src codes
        ///         if it's first time. during parsing and verification, this report will
        ///         collects warning & @ref err.
        ///         this @ref func usually will be called by @ref verifier when it detects the access
        ///         to a @ref pack.
        ///         please see @ref verifier class if you want to know further.
        void setReport(errReport& rpt);
        void setReport(errReport* it) BY_SIDE_FUNC(setReport);

    protected:
        nbool _invalidate() override;

    private:
        tstr<srcs> parse(errReport& rpt, bicontainable& tray) override;
        nbool verify(errReport& rpt, obj& pak) override;
        nbool link();
        void _rel();

    private:
        packLoadings _loadings;
        state _state;
        srcs _srcs;
        tstr<errReport> _rpt;
    };
} // namespace by
