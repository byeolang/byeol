/// @file
#pragma once

#include "core/loader/pack/packLoading.hpp"
#include "core/loader/pack/packMakable.hpp"
#include "core/ast/statable.hpp"
#include "core/ast/scope.hpp"
#include "core/ast/slot.hpp"

namespace by {

    class visitor;

    /** @ingroup core
     *  @brief Automatic pack loading slot
     *  @details Inherits from @ref slot, so see the slot class first if unfamiliar with it.
     *
     *  The byeol language lazily and dynamically loads packs. autoslot implements this feature. When
     *  @ref slotLoader finds a pack file, it creates appropriate @ref packLoading objects and places them in
     *  autoslot. Later, when accessing autoslot to retrieve contained symbols, packLoading operates lazily to load
     *  symbols from the file.
     *
     *  See slotLoader and packLoading for details.
     *
     *  @section multiple_packloadings Multiple packLoadings
     *  packLoading can be retrieved from the native environment (dll or so files) or from the managed environment
     *  (.byeol files). Or both may exist in a single pack. Therefore, autoslot doesn't hold just one packLoading but
     *  handles them as an array.
     *
     *  @section dynamic_verification_dependencies Dynamic Verification and Dependency Issues
     *  We cannot simply assume all packs are verified and flawless and just load them. Sometimes packs need to be
     *  verified again to ensure correctness, so some autoslots must parse or verify before loading symbols. The
     *  problem is that packs very frequently depend on other packs. If a dependent pack fails verification, that fact
     *  must propagate so all dependent packs also become unusable. This is solved by an algorithm controlling the
     *  four states described below.
     *
     *  @section autoslot_state autoslot State
     *  Has four states total with the following loading pipeline flow:
     *
     *  @code
     *                     ┌────────────────┐
     *                     │Make an instance│
     *                     └────────┬───────┘
     *                              │
     *                          ┌───▼────┐
     *                          │RELEASED│
     *                          └───┬────┘
     *                           ┌──▼───┐
     *                           │PARSED│
     *                           └──┬───┘
     *                          ┌───▼────┐
     *                          │VERIFIED│
     *                          └───┬────┘
     *                           ┌──▼───┐
     *                           │LINKED│
     *                           └──────┘
     *  @endcode
     *
     *  - **slot creation**: slotLoader creates slot objects and adds them to the system. At this time, dependencies
     *                       between slots are also recorded.
     *  - **RELEASED**: Initial state, occupies no memory. Most unused slots belong here.
     *  - **PARSED**: When autoslot is accessed, code is parsed for actual use. If parsing is unnecessary (optimized
     *                pack distribution or native pack), skips directly to LINKED state.
     *  - **VERIFIED**: After parsing, code integrity is verified. If verification fails, isValid is set to false.
     *  - **LINKED**: If verification failed, propagates that failure to all dependents referencing it.
     *
     *  @section recursive_loading Recursive Loading
     *  Packs depending on other packs is very common. Because autoslot operates lazily, loading one slot may access
     *  another autoslot, causing that autoslot to also recursively enter the loading sequence. This checks for
     *  duplicate initialization or incomplete pipeline traversal.
     *
     *  @section raii_section RAII
     *  autoslot points to the _pack object defined in @ref slot. This object contains symbols loaded from the
     *  external `pack` file. Since autoslot is responsible for pack loading through packLoading, it's also
     *  responsible for symbol and pack destruction. Using RAII, when the autoslot object is destroyed, all symbols
     *  are removed first, then the packLoading object is also removed to perform operations like closing so files.
     *  See packLoading for details.
     */
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

        /**
         *  this report will be used while interpret @ref pack source code.
         *  @remark when 'subs()' @ref func of the @ref pack has been called, it interprets src codes
         *          if it's first time. during parsing and verification, this report will
         *          collects warning & @ref err.
         *          this @ref func usually will be called by @ref verifier when it detects the access
         *          to a @ref pack.
         *          please see @ref verifier class if you want to know further.
         */
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
