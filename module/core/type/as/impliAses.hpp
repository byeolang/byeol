#pragma once

#include "core/type/as/ases.hpp"

namespace by {

    class _nout impliAses: public ases {
        NM(CLASS(impliAses, ases))

    public:
        impliAses();
        impliAses(const std::initializer_list<aser*>& args);
    };
}
