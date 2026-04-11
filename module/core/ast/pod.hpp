/// @file
#pragma once

#include "core/builtin/err/errReport.hpp"
#include "core/ast/manifest.hpp"
#include "core/ast/origin.hpp"

namespace by {

    class visitor;

    /** @ingroup core
     *  @brief Pod
     *  @details The byeol language distributes libraries in units called `pod`, a type of compressed file. A pod
     *  file contains a top-level `@ref obj` object, @ref manifest, and a list of dependent pods. (In byeol, to use
     *  another pod in your code, you must declare the dependency relationship in the manifest.)
     *
     *  A pod is the result created from a pod file; it doesn't handle loading the pod. See @ref podLoading or
     *  @ref podLoader for pod loading details.
     */
    class _nout pod: public obj {
        BY(CLASS(pod, obj), VISIT())

    public:
        pod(const manifest& mani);

    public:
        manifest& getManifest();
        const manifest& getManifest() const;
        virtual nbool isValid() const;
        void rel() override;

        /**
         * @brief Adds a dependent pod to this slot.
         * @param dependent The pod that depends on this one.
         */
        void addDependent(me& dependent);
        void addDependent(me* it) BY_SIDE_FUNC(addDependent);

        const tnarr<me>& getDependents() const;

    protected:
        /**
         * @brief Protected virtual method to invalidate the pod.
         * @details This method is typically called internally to mark the pod as invalid,
         *          making it unusable for further operations.
         */
        virtual void _invalidate();

    private:
        void _rel();

    private:
        manifest _manifest;
        tnarr<me> _dependents;
    };

    typedef tnmap<std::string, pod> pods;
    typedef tnchain<std::string, pod> podChain;
    typedef tnbicontainer<std::string, pod> podContainer;
} // namespace by
