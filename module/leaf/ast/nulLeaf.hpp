/// @file
#pragma once

#include "leaf.hpp"

namespace by {

    /// @ingroup leaf
    /// @brief Null leaf node representing empty or undefined values
    /// @details Singleton leaf node that represents null/undefined values in the configuration.
    /// Provides default implementations that return empty or false values.
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
