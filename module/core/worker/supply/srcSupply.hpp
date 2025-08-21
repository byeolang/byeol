/// @file
#pragma once

#include "core/builtin/container/native/tnarr.hpp"
#include "core/type/ntype.hpp"

namespace by {

    class parser;
    class exprMaker;

    class _nout srcSupply: public instance {
        BY(ADT(srcSupply, instance))

    public:
        /// @return true if there is no more data to process.
        virtual void* onSupplySrc(parser& ps, void* scanner) const = 0;

    protected:
        void* _scanString(parser& ps, const nchar* src, void* scanner) const;
        exprMaker& _getMaker(parser& ps) const;
    };

    typedef tnarr<srcSupply> srcSupplies;
}
