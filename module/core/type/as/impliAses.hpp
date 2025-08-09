#pragma once

#include "core/type/as/ases.hpp"

namespace by {

    class _nout impliAses: public ases {
        BY(CLASS(impliAses, ases))

    public:
        impliAses();
        impliAses(const std::initializer_list<aser*>& args);
    };
}
