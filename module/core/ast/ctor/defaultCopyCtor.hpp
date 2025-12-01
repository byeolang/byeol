/// @file
#pragma once

#include "core/ast/ctor/baseCtor.hpp"

namespace by {

    class visitor;

    /// @ingroup core
    /// @brief Default copy constructor implementation
    /// @details Provides default copy constructor behavior for objects without explicit copy constructors.
    /// Automatically generated constructor that performs shallow copying of object state.
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
