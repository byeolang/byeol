/// @file
#pragma once

#include "core/common/dep.hpp"

namespace by {

    class node;

    enum typeAttr {
        ATTR_ERR,
        ATTR_CONST,
        ATTR_COMPLETE,
        ATTR_INCOMPLETE
    };

    class _nout util {
        BY(ME(util))

    public:
        static typeAttr checkTypeAttr(const std::string& name);
        static typeAttr checkTypeAttr(const std::string* it) BY_SIDE_FUNC(checkTypeAttr);
        static std::string getEvalTypeFrom(const node& n);
        static std::string getEvalTypeFrom(const node* it) BY_SIDE_FUNC(getEvalTypeFrom);
    };
}
