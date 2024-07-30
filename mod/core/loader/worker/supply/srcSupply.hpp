#pragma once

#include "../../../builtin/container/native/tnarr.hpp"
#include "../../../type/ntype.hpp"

namespace nm {

    class parser;
    class exprMaker;

    class _nout srcSupply: public instance {
        NM(ADT(srcSupply, instance))

    public:
        /// @return true if there is no more data to process.
        virtual void* onSupplySrc(parser& ps, void* scanner) const = 0;

    protected:
        void* _scanString(parser& ps, const nchar* src, void* scanner) const;
        exprMaker& _getMaker(parser& ps) const;
    };

    typedef tnarr<srcSupply> srcSupplies;
}
