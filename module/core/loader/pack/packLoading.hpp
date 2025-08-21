/// @file
#pragma once

#include "core/ast/obj.hpp"
#include "core/builtin/container/native/tnarr.hpp"
#include "core/builtin/container/native/tnchain.hpp"
#include "core/loader/pack/packMakable.hpp"

namespace by {

    class slot;
    struct manifest;
    typedef tnchain<std::string, slot> slotChain;

    /// @ingroup core
    /// @brief Base class for pack loading strategies
    /// @details Abstract base class that defines the interface for loading different types of packs.
    /// Manages file paths and provides the foundation for specialized pack loading implementations.
    class _nout packLoading: public instance, public packMakable {
        BY(ADT(packLoading, instance))

        typedef nbool (me::*packEvent)(const slotChain&);

    public:
        void addPath(const std::string& filePath);
        void addPath(const std::vector<std::string> paths);

        virtual void rel();

        virtual const std::string& getName() const = 0;

    protected:
        const std::vector<std::string>& _getPaths() const;

    private:
        std::vector<std::string> _paths;
    };

    typedef std::vector<packLoading*> packLoadings;
}
