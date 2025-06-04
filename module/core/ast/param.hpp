#pragma once

#include "core/common.hpp"
#include "core/type/ntype.hpp"
#include "core/ast/immutableTactic.hpp"

namespace nm {
    class _nout param: public instance {
        NM(CLASS(param, instance))

    public:
        param(const std::string& newName, const node& org);
        param(const std::string& newName, const node* org);

    public:
        nbool operator==(const me& rhs) const;
        nbool operator!=(const me& rhs) const;

    public:
        const std::string& getName() const;
        void setName(const std::string& new1);
        const node& getOrigin() const;
        void setOrigin(const node& newType);

        void onCloneDeep(const clonable& from) override;

    public:
        std::string _name;
        str _org;
    };
}
