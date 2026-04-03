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
        static std::string joinVectorString(const tucontainable<nStr, nStr*, nStr&>& container);

    public:
        static constexpr const nchar* FILE_EXTENSION = "byeol";
        static inline const std::string FILE_REGEX = std::string("[^\\@\\#\\$\\%\\[\\]\\+\\\\\\|\\<\\>\\=\\!\\^\\&\\*\\(\\)\\{\\}\\n\\s]+\\.") + util::FILE_EXTENSION;
    };
}
