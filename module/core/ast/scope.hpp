/// @file
#pragma once

#include "core/builtin/container/native/tnchain.inl"

namespace by {
    /// @ingroup core
    /// @brief Symbol scope container
    /// @details scope is declared based on @ref tnchain. See tnchain first if unfamiliar.
    ///
    /// @section scope_concept Scope Concept in Byeol
    /// scope is classified into 5 types based on who created it: local scope (created by @ref blockExpr), func
    /// scope (created by @ref baseFunc), obj scope (created by @ref baseObj), file scope, and pack scope. (Note:
    /// pack is the same as @ref obj derived from baseObj code-wise, but byeol conceptually treats pack scope
    /// separately.)
    ///
    /// @section scope_usage How Scope is Used
    /// scope is `add()`ed to @ref frames owned by the @ref thread object, allowing @ref node objects in the
    /// program's AST to access symbols registered in the scope. Here we only explain each scope conceptually. See
    /// @ref frameInteract or @ref frames for how scopes are actually created, registered, and released.
    ///
    /// @section local_scope Local Scope
    /// Local scope is created when a block statement executes within a function. When the block statement ends, the
    /// scope disappears and instances referenced in that scope lose 1 reference count. For optimization, the local
    /// scope creation itself isn't done directly by @ref blockExpr; instead, the side executing blockExpr has the
    /// @ref frameInteractable blockExpr create it.
    ///
    /// @section func_scope Func Scope
    /// Scope storing symbols owned by func. Usually func's sub nodes are used directly as func scope. Typically
    /// includes @ref nestedFunc owned by func or @ref obj or static variables defined within func. Note these aren't
    /// created anew each time func is created. Since this scope is unique sub nodes owned by a unique func object in
    /// the system, it also exists uniquely.
    ///
    /// @section obj_scope Obj Scope
    /// Similar to func scope, the scope containing funcs or variables owned by a specific object is called obj scope.
    /// obj has separately distinguished shares and owns, and inherited symbols are also included in this obj scope.
    /// See @ref obj for details.
    ///
    /// @section file_scope File Scope
    /// Literally the scope accessible only within that source code unit. In byeol code, symbols defined above `pack`
    /// all belong to file scope.
    ///
    /// @remark file scope is always made to chain pack scope by the @ref parser. See @ref tnchain for chaining.
    ///
    /// @section pack_scope Pack Scope
    /// Scope where symbols defined directly under pack are located. Functions defined in pack have their me as
    /// `pack`, and defined @ref origin objects have their obj scope chain pack scope. See @ref frames for why this
    /// chaining happens. Unlike file scope, pack scope can be accessed externally if dependencies are defined in
    /// @ref manifest.
    typedef tnchain<std::string, node, tnmap<std::string, node, immutableTactic>> scope;
    extern template class tnchain<std::string, node, tnmap<std::string, node, immutableTactic>>;
}
