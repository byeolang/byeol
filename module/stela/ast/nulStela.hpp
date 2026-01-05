/// @file
#pragma once

#include "stela.hpp"

namespace by {

    /// @ingroup stela
    /// @brief Null stela node representing empty or undefined values
    /// @details When retrieving child objects from @ref stela, usually by name, if no child
    /// with that name exists, nulStela is returned instead.
    ///
    /// nulStela implements the null object pattern. When value conversion is requested on
    /// this object, it always returns the default value for that type.
    ///
    /// @remark Checking for nulStela
    /// To verify if a stela object is nulStela, check if isExist() or operator bool() returns
    /// false.
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
