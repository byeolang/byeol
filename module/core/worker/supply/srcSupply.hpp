/// @file
#pragma once

#include "core/builtin/container/native/tnarr.hpp"
#include "core/type/ntype.hpp"

namespace by {

    class parser;
    class exprMaker;
    class srcSupply;
    typedef tnarr<srcSupply> srcSupplies;

    /** @ingroup core
     *  @brief Source code supply interface
     *  @details Abstract base for supplying source code to the parser from various sources.
     */
    class _nout srcSupply: public instance {
        BY(ADT(srcSupply, instance))

    public:
        /**
         *  @return true if there is no more data to process.
         */
        virtual void* onSupplySrc(parser& ps, void* scanner) const = 0;

        /**
         * @brief if the given path points to a file, a single fileSupply will be returned.
         *        otherwise this will traverse all sub folders to find byeol src files
         *        and return each fileSupplies matching them.
         * @param path for single src file or directory including src files.
         */
        static srcSupplies makeSuppliesFrom(const std::string& path);

    protected:
        void* _scanString(parser& ps, const nchar* src, void* scanner) const;
    };
}
