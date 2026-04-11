/// @file
#pragma once

#include "core/ast/obj.hpp"
#include "core/builtin/container/native/tnarr.hpp"
#include "core/builtin/container/native/tnchain.hpp"
#include "core/loader/pod/podMakable.hpp"

namespace by {

    class pod;
    struct manifest;
    typedef tnchain<std::string, pod> podChain;
    class errReport;
    class pod;

    /** @ingroup core
     *  @brief Base class for pod loading strategies
     *  @details Abstract base class that defines the interface for loading different types of pods.
     *  Manages file paths and provides the foundation for specialized pod loading implementations.
     */
    class _nout podLoading: public instance, public podMakable {
        BY(ADT(podLoading, instance))

        typedef nbool (me::*podEvent)(const podChain&);

    public:
        void addPath(const std::string& filePath);
        void addPath(const std::vector<std::string> paths);

        virtual void rel();

        virtual const std::string& getName() const = 0;

    protected:
        const std::vector<std::string>& _getPaths() const;
        virtual void _tryDump(const errReport& rpt, const pod& pak, ncnt prevCnt) const;

    private:
        std::vector<std::string> _paths;
    };

    typedef std::vector<podLoading*> podLoadings;
}
