/// @file
#pragma once

#include "core/common/dep.hpp"

namespace by {

    class node;
    class nStr;
    template <typename T, typename R, typename RSquare> class tucontainable;

    enum typeAttr {
        ATTR_ERR,
        ATTR_CONST,
        ATTR_COMPLETE,
        ATTR_INCOMPLETE
    };

    /** @ingroup core
     *  @brief Core utility functions
     *  @details Static utility methods for type checking and evaluation operations.
     */
    class _nout util {
        BY(ME(util))

    public:
        static typeAttr checkTypeAttr(const std::string& name);
        static typeAttr checkTypeAttr(const std::string* it) BY_SIDE_FUNC(checkTypeAttr);
        static std::string getEvalTypeFrom(const node& n);
        static std::string getEvalTypeFrom(const node* it) BY_SIDE_FUNC(getEvalTypeFrom);
        static std::string joinVectorString(const std::vector<std::string>& container);
        template <typename T>
        static std::string joinVectorString(const T& container) {
            std::string ret;
            nbool isFirst = true;
            for(auto e = container.begin(); e; e++) {
                ret += e->get();

                if(!isFirst) ret += ", ";
                isFirst = true;
            }
            return ret;
        }
    };
}
