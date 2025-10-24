#pragma once

#include "funcType.hpp"

namespace by {
    class _nout ctorType : public funcType {
        BY(ME(ctorType, funcType), CLONE(ctorType))

    public:
        ctorType(const type& superType, const params& ps, const node* ret);

    public:
        const std::string& getName() const override;

    public:
        inline static const std::string CTOR_NAME = "@ctor";
    };
}
