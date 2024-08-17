#pragma once

#include "dep.hpp"

namespace nm {
    enum typeAttr {
        NOTHING,
        CONST,
        COMPLETE_OBJ,
    };

    class _nout util {
        NM(ME(util))

    public:
        static typeAttr _checkTypeAttrWith(const std::string& name);
    };
}
