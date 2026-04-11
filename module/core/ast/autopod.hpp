/// @file
#pragma once

#include "core/loader/pod/podLoading.hpp"
#include "core/loader/pod/podMakable.hpp"
#include "core/ast/statable.hpp"
#include "core/ast/scope.hpp"
#include "core/ast/pod.hpp"

namespace by {

    class visitor;

    /** @ingroup core
     *  @brief Automatic pod loading pod
     *  @details Inherits from @ref pod, so see the pod class first if unfamiliar with it.
     *
     *  The byeol language lazily and dynamically loads pods. autopod implements this feature. When
     *  @ref podLoader finds a pod file, it creates appropriate @ref podLoading objects and places them in
     *  autopod. Later, when accessing autopod to retrieve contained symbols, podLoading operates lazily to load
     *  symbols from the file.
     *
     *  See podLoader and podLoading for details.
     *
     *  @section multiple_podloadings Multiple podLoadings
     *  podLoading can be retrieved from the native environment (dll or so files) or from the byeol runtime environment
     *  (.byeol files). Or both may exist in a single pod. Therefore, autopod doesn't hold just one podLoading but
     *  handles them as an array.
     *
     *  @section dynamic_verification_dependencies Dynamic Verification and Dependency Issues
     *  We cannot simply assume all pods are verified and flawless and just load them. Sometimes pods need to be
     *  verified again to ensure correctness, so some autopods must parse or verify before loading symbols. The
     *  problem is that pods very frequently depend on other pods. If a dependent pod fails verification, that fact
     *  must propagate so all dependent pods also become unusable. This is solved by an algorithm controlling the
     *  four states described below.
     *
     *  @section autopod_state autopod State
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
     *  - **pod creation**: podLoader creates pod objects and adds them to the system. At this time, dependencies
     *                       between pods are also recorded.
     *  - **RELEASED**: Initial state, occupies no memory. Most unused pods belong here.
     *  - **PARSED**: When autopod is accessed, code is parsed for actual use. If parsing is unnecessary (optimized
     *                pod distribution or native pod), skips directly to LINKED state.
     *  - **EXPANDED**: after parsing resolve dependencies between symbols and expressions.
     *  - **VERIFIED**: After expanding, code integrity is verified. If verification fails, isValid is set to false.
     *  - **LINKED**: If verification failed, propagates that failure to all dependents referencing it.
     *  - **INVALID**: pod loading has been failed for some reason. it's unable to recover it.
     *
     *  @section recursive_loading Recursive Loading
     *  pods depending on other pods is very common. Because autopod operates lazily, loading one pod may access
     *  another autopod, causing that autopod to also recursively enter the loading sequence. This checks for
     *  duplicate initialization or incomplete pipeline traversal.
     *
     *
     *  @section raii_section RAII
     *  autopod points to the _pod object defined in @ref pod. This object contains symbols loaded from the
     *  external `pod` file. Since autopod is responsible for pod loading through podLoading, it's also
     *  responsible for symbol and pod destruction. Using RAII, when the autopod object is destroyed, all symbols
     *  are removed first, then the podLoading object is also removed to perform operations like closing so files.
     *  See podLoading for details.
     *
     *
     *  @section thread is required to run an autopod
     *  when you try to load pod by accessing subs() of autopod instance, it processes loading
     *  and access to errReport when exception occurs during that procedure.
     *  basically, accessing to subs() of autopod happen when you eval your AST.
     *  which means, you're required to prepare a thread. so autopod will use
     *  the errReport instance in current thread::getEx().
     *
     *  @remark this doesn't mean that a thread is needed when running a podLoader
     *          that creates autopods.
     */
    class _nout autopod: public pod, public podMakable {
        BY(CLASS(autopod, pod), VISIT())

    public:
        autopod(const manifest& manifest, const podLoadings& loadingsInHeap);
        ~autopod() override;

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
        nbool parse(errReport& rpt, pod& pak) override;
        nbool expand(errReport& rpt, pod& pak) override;
        nbool verify(errReport& rpt, pod& pak) override;
        nbool link();
        void _rel();

    private:
        podLoadings _loadings;
        state _state;
    };
} // namespace by
