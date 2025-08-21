/// @file
#pragma once

#include "leaf.hpp"

namespace by {

    class _nout nulLeaf: public leaf {
        BY(ME(nulLeaf, leaf), CLONE(me), INIT_META(nulLeaf))

    public:
        nulLeaf();

    public:
        const std::string& asStr() const override;
        nchar asChar() const override;
        nint asInt() const override;
        nbool asBool() const override;
        const type& getType() const override;
        nbool isExist() const override;

    public:
        static me& get();
    };
}
