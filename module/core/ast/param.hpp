/// @file
#pragma once

#include "core/common.hpp"
#include "core/type/ntype.hpp"
#include "core/ast/immutableTactic.hpp"

namespace by {
    /// @ingroup core
    /// @brief Function parameter definition
    /// @details Represents a single function parameter with name and type information.
    /// Used in function signatures to define parameter requirements and type checking.
    class _nout param: public instance {
        BY(CLASS(param, instance))

    public:
        param(const std::string& newName, const node& org);
        param(const std::string& newName, const node* org);

    public:
        nbool operator==(const me& rhs) const;
        nbool operator!=(const me& rhs) const;

    public:
        const std::string& getName() const;

        void setName(const std::string& new1);
        void setName(const std::string* it) BY_SIDE_FUNC(setName);
        void setName(const nchar* new1) BY_SIDE_FUNC(new1, setName(std::string(new1)), void());

        const node& getOrigin() const;
        void setOrigin(const node& newType);

        void onCloneDeep(const clonable& from) override;

    public:
        std::string _name;
        str _org;
    };
}
