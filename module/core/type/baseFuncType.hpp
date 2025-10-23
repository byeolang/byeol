#pragma once

#include "core/type/ntype.hpp"

namespace by {
    class _nout baseFuncType : public ntype {
    public:
        const std::string& getName() const override;

    protected:
        const ases& _getAses() const override;

    public:
        static constexpr const nchar* TYPE_NAME = "func";
    };
}
