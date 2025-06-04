#pragma once

#include "core/ast/obj.hpp"
#include "core/builtin/container/native/tnarr.hpp"
#include "core/builtin/container/native/tnchain.hpp"
#include "core/loader/pack/packMakable.hpp"

namespace nm {

    class slot;
    struct manifest;
    typedef tnchain<std::string, slot> slotChain;

    class _nout packLoading: public instance, public packMakable {
        NM(ADT(packLoading, instance))

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
