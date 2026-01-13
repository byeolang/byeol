/// @file
#pragma once

#include "core/common.hpp"

namespace by {

    /** @ingroup core
     *  @brief Immutable type binding strategy
     *  @details str, int, and other scalar types are all immutable types. This stems from byeol's calling strategy
     *  following `by object`, which operates like C-family languages like Java and C# commonly use: `references to
     *  objects are shallow copied, raw types are deep copied`.
     *
     *  However, the part responsible for this shouldn't be objects owning containers like @ref node or @ref obj, but
     *  the container classes themselves like @ref tnmap.
     *
     *  From obj's perspective, node is just node, and it shouldn't need to know whether the new node to insert into
     *  its owned container is an immutable type. Breaking this also breaks polymorphism. So whether to copy an
     *  incoming object or just point to a reference is determined through immutableTactic.
     *
     *  @section algorithm_section Algorithm
     *  The determination is simply made using the `isImmutable()` function defined in @ref ntype. Scalar type
     *  implementations like @ref nInt and @ref nStr export their types with isImmutable() returning true. This is
     *  implemented through the meta module's type extension feature. See @ref type for detailed type extension
     *  functionality.
     *
     *  If determined to be an immutable type, `clone()` is called instead of directly inserting the argument.
     */
    class _nout immutableTactic: public strTactic {
        BY(ME(immutableTactic, strTactic))

    public:
        using super::bind;
        nbool bind(binder& me, const instance& it) override;

    public:
        static me singleton;
    };
}
