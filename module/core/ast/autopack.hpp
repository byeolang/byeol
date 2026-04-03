/// @file
#pragma once

#include "core/loader/pack/packLoading.hpp"
#include "core/loader/pack/packMakable.hpp"
#include "core/ast/statable.hpp"
#include "core/ast/scope.hpp"
#include "core/ast/pack.hpp"

namespace by {

    class visitor;

    /** @ingroup core
     *  @brief Automatic pack loading pack
     *  @details Inherits from @ref pack, so see the pack class first if unfamiliar with it.
     *
     *  The byeol language lazily and dynamically loads packs. autopack implements this feature. When
     *  @ref packLoader finds a pack file, it creates appropriate @ref packLoading objects and places them in
     *  autopack. Later, when accessing autopack to retrieve contained symbols, packLoading operates lazily to load
     *  symbols from the file.
     *
     *  See packLoader and packLoading for details.
     *
     *  @section multiple_packloadings Multiple packLoadings
     *  packLoading can be retrieved from the native environment (dll or so files) or from the byeol runtime environment
     *  (.byeol files). Or both may exist in a single pack. Therefore, autopack doesn't hold just one packLoading but
     *  handles them as an array.
     *
     *  @section dynamic_verification_dependencies Dynamic Verification and Dependency Issues
     *  We cannot simply assume all packs are verified and flawless and just load them. Sometimes packs need to be
     *  verified again to ensure correctness, so some autopacks must parse or verify before loading symbols. The
     *  problem is that packs very frequently depend on other packs. If a dependent pack fails verification, that fact
     *  must propagate so all dependent packs also become unusable. This is solved by an algorithm controlling the
     *  four states described below.
     *
     *  @section autopack_state autopack State
     *  Has four states total with the following loading pipeline flow:
     *
     *  @code
     *                          ┌────────────────┐
     *                          │Make an instance│
     *                          └────────┬───────┘
     *                                   │
     *                               ┌───▼────┐
     *                               │RELEASED├────┐
     *                               └───┬────┘    │
     *                                ┌──▼───┐     │
     *                                │PARSED├─────┤
     *                                └──┬───┘     │
     *                               ┌───▼────┐    │
     *                               │EXPANDED├────┤
     *                               └───┬────┘    │
     *                               ┌───▼────┐    │
     *                               │VERIFIED├────┤
     *                               └───┬────┘    │
     *                                ┌──▼───┐ ┌───▼───┐
     *                                │LINKED│ │INVALID│
     *                                └──────┘ └───────┘
     *  @endcode
     *
     *  - **pack creation**: packLoader creates pack objects and adds them to the system. At this time, dependencies
     *                       between packs are also recorded.
     *  - **RELEASED**: Initial state, occupies no memory. Most unused packs belong here.
     *  - **PARSED**: When autopack is accessed, code is parsed for actual use. If parsing is unnecessary (optimized
     *                pack distribution or native pack), skips directly to LINKED state.
     *  - **EXPANDED**: after parsing resolve dependencies between symbols and expressions.
     *  - **VERIFIED**: After expanding, code integrity is verified. If verification fails, isValid is set to false.
     *  - **LINKED**: If verification failed, propagates that failure to all dependents referencing it.
     *  - **INVALID**: pack loading has been failed for some reason. it's unable to recover it.
     *
     *  @section recursive_loading Recursive Loading
     *  Packs depending on other packs is very common. Because autopack operates lazily, loading one pack may access
     *  another autopack, causing that autopack to also recursively enter the loading sequence. This checks for
     *  duplicate initialization or incomplete pipeline traversal.
     *
     *
     *  @section raii_section RAII
     *  autopack points to the _pack object defined in @ref pack. This object contains symbols loaded from the
     *  external `pack` file. Since autopack is responsible for pack loading through packLoading, it's also
     *  responsible for symbol and pack destruction. Using RAII, when the autopack object is destroyed, all symbols
     *  are removed first, then the packLoading object is also removed to perform operations like closing so files.
     *  See packLoading for details.
     *
     *
     *  @section thread is required to run an autopack
     *  when you try to load pack by accessing subs() of autopack instance, it processes loading
     *  and access to errReport when exception occurs during that procedure.
     *  basically, accessing to subs() of autopack happen when you eval your AST.
     *  which means, you're required to prepare a thread. so autopack will use
     *  the errReport instance in current thread::getEx().
     *
     *  @remark this doesn't mean that a thread is needed when running a packLoader
     *          that creates autopacks.
     */
    class _nout autopack: public pack, public packMakable {
        BY(CLASS(autopack, pack), VISIT())

    public:
        autopack(const manifest& manifest, const packLoadings& loadingsInHeap);
        ~autopack() override;

    public:
        using super::subs;
        scope& subs() override;

        state getState() const override;
        void setState(state new1) override;
        nbool isValid() const override;

        void rel() override;

    protected:
        void _invalidate() override;

    private:
        nbool parse(errReport& rpt, pack& pak) override;
        nbool expand(errReport& rpt, pack& pak) override;
        nbool verify(errReport& rpt, pack& pak) override;
        nbool link();
        void _rel();

    private:
        packLoadings _loadings;
        state _state;
    };
} // namespace by
