/// @file
#pragma once

#include "core/ast/ctor/baseCtor.hpp"
#include "core/ast/args.hpp"

namespace by {

    class visitor;

    /// @ingroup core
    /// @brief Default constructor implementation
    /// @details Provides default constructor behavior for objects without explicit constructors.
    /// Automatically generated constructor that performs basic object initialization.
    class _nout defaultCtor: public baseCtor {
        BY(ME(defaultCtor, baseCtor), CLONE(defaultCtor), VISIT())

    public:
        defaultCtor(const node& org);

    public:
        using super::run;
        str run(const args& a) override;
    };
}
