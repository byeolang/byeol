#pragma once

#include "core/loader/pack/packLoading.hpp"
#include "core/loader/pack/packMakable.hpp"
#include "core/ast/statable.hpp"
#include "core/ast/scope.hpp"
#include "core/ast/slot.hpp"

namespace nm {

    class visitor;

    class _nout autoslot: public slot, public packMakable, public statable {
        NM(CLASS(autoslot, slot), VISIT())

    public:
        autoslot(const manifest& manifest, const packLoadings& loadingsInHeap);
        ~autoslot() override;

    public:
        state getState() const override;
        void setState(state new1) override;

        using super::getPack;
        obj& getPack() override;

        void rel() override;

        /// this report will be used while interpret pack source code.
        /// @remark when 'subs()' func of the pack has been called, it interprets src codes
        ///         if it's first time. during parsing and verification, this report will
        ///         collects warning & errors.
        ///         this func usually will be called by verifier when it detects the access
        ///         to a pack.
        ///         please refer 'verifier' class if you want to know further.
        void setReport(errReport& rpt);
        void setReport(errReport* it) NM_SIDE_FUNC(setReport);

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
} // namespace nm
