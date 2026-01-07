/// @file
#pragma once

#include "core/ast/ctor/baseCtor.hpp"

namespace by {

    class visitor;

    /// @ingroup core
    /// @brief Default copy constructor for managed environment
    /// @details Automatically added by the @ref parser when users don't define a copy constructor for byeol types
    /// in the managed environment. The object to be copied is passed through args, and this constructor performs
    /// shallow copying of the object's properties.
    ///
    /// @remark @ref scalar objects are immutable, so even shallow copying behaves like deep copying. See
    /// @ref immutableTactic for details.
    class _nout defaultCopyCtor: public baseCtor {
        BY(ME(defaultCopyCtor, baseCtor), INIT_META(defaultCopyCtor), CLONE(defaultCopyCtor), VISIT())

    public:
        defaultCopyCtor(const node& org);

    public:
        using super::eval;
        str eval(const args& a) override;

        const src& getSrc() const override;
    };
}
