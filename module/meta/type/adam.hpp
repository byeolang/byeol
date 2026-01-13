/// @file
#pragma once

#include "meta/common.hpp"

namespace by {
    /** @ingroup meta
     *  @brief Root base class for the type hierarchy
     *  @details Meta types in the @ref type class hierarchy that have no parent class are
     *  difficult to handle generically. For example, to write a function that operates on all
     *  meta types, you would need a way to iterate over all of them.
     *
     *  To solve this, when constructing meta information, any class without a parent class
     *  is assigned adam as its parent. This allows accessing all types through adam.
     *
     *  @section Usage
     *  Example of iterating over all types:
     *  @code
     *      // Without adam, how would you get all types?
     *      // You'd have to manually collect them:
     *      vector<type> how_to_get_all_meta_types() {
     *          vector<type> ret;
     *          ret.push_back(ttype<A>());
     *          ret.push_back(ttype<B>());
     *          // ... exhaustively list all types
     *          return ret;
     *      }
     *
     *      // With adam, you can simply iterate:
     *      for(const auto& t : ttype<adam>().getSubs())
     *          cout << t.getName() << "\n";
     *  @endcode
     */
    class _nout adam {};
}
