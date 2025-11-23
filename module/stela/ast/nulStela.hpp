/// @file
#pragma once

#include "stela.hpp"

namespace by {

    /// @ingroup stela
    /// @brief Null stela node representing empty or undefined values
    /// @details Singleton stela node that represents null/undefined values in the configuration.
    /// Provides default implementations that return empty or false values.
    class _nout nulStela: public stela {
        BY(ME(nulStela, stela), CLONE(me), INIT_META(nulStela))

    public:
        nulStela();

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
