/// @file
#pragma once

#include "core/type/as/ases.hpp"

namespace by {

    /** @ingroup core
     *  @brief Implicit type conversion collection
     *  @details Collection of implicit type converters for automatic type conversion.
     */
    class _nout impliAses: public ases {
        BY(CLASS(impliAses, ases))

    public:
        impliAses();
        impliAses(const std::initializer_list<aser*>& args);
    };
}
