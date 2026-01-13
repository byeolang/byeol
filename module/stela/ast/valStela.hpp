/// @file
#pragma once

#include "stela/ast/stela.hpp"

namespace by {

    /** @ingroup stela
     *  @brief Value stela node for storing primitive values
     *  @details Unlike @ref nulStela, this @ref stela holds scalar types like int, float,
     *  string. Internally stores value as a base string, and can convert to appropriate
     *  types on type conversion requests like `asInt()`.
     *
     *  @remark Exception on type conversion failure
     *  If type conversion fails, an exception is thrown, so be careful.
     */
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
