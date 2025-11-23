/// @file
#pragma once

#include "stela/ast/stela.hpp"

namespace by {

    /// @ingroup stela
    /// @brief Value Stela node for storing primitive values
    /// @details Extends Stela to store and convert primitive values like strings,
    /// integers, floats, and booleans. Provides type conversion capabilities.
    class _nout valStela: public stela {
        BY(ME(valStela, stela), CLONE(me), INIT_META(valStela))

    public:
        valStela(const std::string& rawVal, const std::string& name = "");
        valStela(nbool val, const std::string& name = "");
        valStela(nint val, const std::string& name = "");
        valStela(nflt val, const std::string& name = "");
        valStela(const me& rhs, const std::string& name = "");

    public:
        const std::string& asStr() const override;
        nchar asChar() const override;
        nint asInt() const override;
        nbool asBool() const override;
        const type& getType() const override;

    private:
        static std::string toLower(std::string it);

    private:
        std::string _rawVal;
    };
}
