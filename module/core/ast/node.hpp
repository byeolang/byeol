/// @file
#pragma once

#include "core/builtin/container/native/tnarr.hpp"
#include "core/frame/frameInteractable.hpp"
#include "core/type/ntype.hpp"
#include "core/builtin/container/native/tnmap.hpp"
#include "core/ast/tpriorities.hpp"
#include "core/ast/validable.hpp"

namespace by {

    class ases;
    class args;
    class visitor;
    class visitInfo;
    class baseObj;
    class src;
    class modifier;
    template <typename K, typename V, typename defaultContainer> class tnchain;

    typedef tnchain<std::string, node, tnmap<std::string, node, immutableTactic>> scope;

    template <typename T> class tmock;

    /** @ingroup core
     *  @brief Base class for all AST nodes in the byeol language
     *  @details The most fundamental class in the core module. Unlike traditional AST (Abstract Syntax Tree)
     *  implementations, byeol executes programs while maintaining the AST structure, making it more akin to a
     *  Program Execution Tree (PET). Provides comprehensive APIs for AST structure manipulation, node evaluation,
     *  child node traversal, and type conversion.
     *
     *  @section program_execution_tree Program Execution Tree
     *  Unlike typical programming languages, byeol executes programs while maintaining the AST structure intact.
     *  In most languages, the AST is merely an intermediate representation of grammatical structure, but in byeol,
     *  the AST serves as the final executable output. Because execution is considered from the start, the class
     *  provides not only tree-based program structure representation but also APIs for type conversion and node
     *  evaluation.
     *
     *  @section fundamental_class Fundamental Base Class
     *  Each node must be executable, so a node can be a function, an object, or an operator like `+`. The
     *  evaluation function is provided as `eval(const args&)` to execute and evaluate values.
     *
     *  @section side_funcs Side Functions
     *  As the core base class of the byeol project, node provides numerous side functions for various operations.
     *
     *  @section ast_traversal AST Traversal
     *  Due to AST characteristics, a node can contain objects of other node-derived classes. Like a DOM tree, it
     *  uses the composition pattern, and various functions are provided to traverse this tree. In byeol, this tree
     *  is called @ref scope and is map-based. Classes like @ref blockExpr contain statement blocks as arrays while
     *  also being nodes. Therefore, the overall AST structure is a mix of maps and arrays. The scope uses each
     *  node's name as a key, where the @ref parser defines appropriate names as keys when adding instances.
     *
     *  Common traversal functions: `operator[], sub(), subs(), in(), subAll()`
     *
     *  @code
     *      const node& root = getRoot();
     *      root.sub("name1");             // Find node named name1 in root (unknown if function or object)
     *      root.sub<func>("name1");       // Get symbol named name1 only if it's a function
     *
     *      // Find all nodes named name2 that take one int argument
     *      tnarr found = root.subAll("name2", args(narr(*new nInt())));
     *
     *      const scope& subs = root.subs(); // Can utilize all tbicontainable APIs
     *                                       // Supports for loops, iterators, lambda filtering, etc.
     *      subs.len();                      // Returns number of child nodes root has
     *  @endcode
     *
     *  @section duplicate_symbols Handling Duplicate Symbols
     *  Byeol doesn't allow duplicate symbols within the same scope, but this doesn't mean two pairs with the same
     *  key cannot exist. Functions with the same name but different parameter counts or types are different
     *  symbols. Simple string comparison cannot determine duplication, so the scope class is based on multimap
     *  rather than map.
     *
     *  @section eval_section Evaluation
     *  A node can be a function, object, or expression. `eval(const args&)` returns the execution result of such
     *  nodes. (Byeol has no class concept; objects replace this role. Objects can be executed like functions,
     *  equivalent to calling a constructor.) The @ref args object containing required arguments is passed to
     *  eval(). If the node's expected parameters differ in count or type from args, an empty @ref str may be
     *  returned. Some node functions take a name parameter along with `eval()`, which finds child nodes matching
     *  the node's name and passes args to evaluate them—essentially dispatching a message.
     *
     *  @section infer_section Type Inference
     *  `infer()` performs type inference. It doesn't return the exact runtime value but the type determined at
     *  verification stage. For example:
     *
     *  @code
     *      // In byeol language, `2 + 3.5` is represented as:
     *      FBOExpr e = FBOExpr(FBOExpr::SYMBOL_ADD, *new nInt(2), *nFlt(3.5));
     *
     *      str infered = e.infer();       // What type results from adding int 2 and flt 3.5?
     *      infered->cast<nFlt>() != nullptr; // true: answer is flt
     *  @endcode
     *
     *  This creates an expression like 2 + 3.5 by putting @ref nInt and @ref nFlt objects into @ref FBOExpr.
     *  Calling `infer()` yields nFlt due to type promotion (int + flt = flt). Type inference for an expression
     *  requires recursively calling type inference on child nodes, unlike `getType()` which returns immediately.
     *  The AST must be traversed to calculate type information. Note that type inference focuses on quickly
     *  determining types and doesn't compute values. In the example above, the nFlt value in `infered` contains
     *  the default value—use `eval()` for accurate values.
     *
     *  @section type_conversion Type Conversion
     *  Node provides `as()` for explicit type conversion and `is()` to check conversion possibility:
     *
     *  @code
     *      // The following byeol code translated to C++:
     *      //  foo(val int) void
     *      //      if val is flt
     *      //          doSomething(val as flt)
     *
     *      void foo(const nInt& val) {
     *          if(val.is<nFlt>()) {
     *              str isFlt = val.as<nFlt>();
     *              nflt converted = isFlt->get();
     *              doSomething(converted);
     *          }
     *      }
     *  @endcode
     *
     *  `as()` and `is()` provide various side functions, enabling generic functions:
     *
     *  @code
     *      str convertIfPossible(const node& it, const node& toThisType) {
     *          // getType() retrieves type information (C++ class type or user-defined byeol type)
     *          // See type and ntype classes for details
     *          if(!it.is(toThisType.getType())) return str();
     *          return it.as(toThisType.getType());
     *      }
     *  @endcode
     *
     *  This example is verbose for clarity. Real code uses @ref WHEN for brevity. Rewriting foo():
     *
     *  @code
     *      void foo(const nInt& val) {
     *          tstr<nFlt> converted = val OR.ret(); // early-return pattern + WHEN + OR macro
     *          doSomething(converted->get());
     *      }
     *  @endcode
     *
     *  @section managed_vs_native Managed vs Native Type Conversion
     *  The `as()` and `is()` functions handle type conversion in the byeol language environment. For example,
     *  @ref nInt is the C++ class representing byeol's `int`. It inherits from @ref node because int can be an AST
     *  instance in byeol. Calling is<nFlt>() on nInt is allowed, but this doesn't mean nInt converts to nFlt in
     *  C++ code. In C++, implicit conversion is generally only allowed when nInt is a parent class of nFlt.
     *
     *  To check if nFlt is a kind of node, use the meta module's type conversion via `cast()`. The meta module
     *  manages native environment types in C++ code. Thus, two type conversion systems exist: one for native (C++)
     *  and one for managed (byeol language):
     *
     *  @code
     *      // Simplified inheritance relationship of nFlt and nInt:
     *      class nFlt : public obj {};
     *      class nInt : public obj {};
     *      class obj : public node {};
     *
     *      nInt val1;
     *      nFlt val2;
     *
     *      // Native type conversion:
     *      nFlt* cast1 = dynamic_cast<nFlt>(val1);  // nullptr: disallowed conversion
     *      nFlt* cast2 = val1.cast<val2>();          // nullptr: same, using meta module conversion
     *      node* isNode = val1.cast<node>();         // != nullptr: upcasting allowed
     *      isNode->cast<nFlt>();                     // nullptr
     *      &val1 == isNode->cast<nInt>();            // true
     *
     *      // Managed type conversion:
     *      val1.is<nFlt>(); // true: byeol supports explicit int <-> flt conversion
     *  @endcode
     *
     *  The core module often implements the same concept separately for native and managed environments. Get
     *  familiar with this pattern. See @ref ases and @ref asable for type conversion flow details.
     *
     *  @section implicit_explicit Implicit vs Explicit Conversion
     *  Implicit conversion is provided via `impliAs()` and `impliIs()`. Rarely called directly by external code;
     *  mostly invoked by AST classes like @ref expr and @ref baseFunc. User-defined type conversions are naturally
     *  excluded from implicit conversion, hence the distinction.
     *
     *  @section visitable_class Visitable Class
     *  AST traversal occurs for various purposes, not just `eval()` but also for debugging output. To separate
     *  traversal methods from node processing, the visitor pattern must be applied to nodes. `accept()` is used by
     *  the @ref visitor class, and each class header requires the VISIT macro. See @ref visitor for details.
     *
     *  @section frame_interaction Frame Interaction
     *  @ref frame manages the currently executing scope and registered symbols. Since node forms the AST base, if a
     *  node is an object or function, it must register or unregister owned functions or properties with the frame.
     *  `inFrame()` and `outFrame()` handle this. See @ref frame for details.
     *
     *  @section message_priority Message Priority
     *  Byeol supports function overloading, so an object may have multiple functions with the same name but
     *  different parameters. Some functions match args exactly, while others may require type conversion. Node uses
     *  `prioritize()` to determine how well child nodes match given args. Derived node classes return EXACT_MATCH
     *  for precise matches and NO_MATCH otherwise. See @ref tprior and @ref priorType for detailed criteria and
     *  algorithms.
     */
    class _nout node: public instance, public frameInteractable {
        BY(ADT(node, instance))
        friend class coreInternal;
        friend class exprMaker; // for _setSrc
        template <typename T> friend class tmock;

    public:
        me& operator[](const std::string& name);
        const me& operator[](const std::string& name) const BY_CONST_FUNC(operator[](name));
        me& operator[](const std::string* it);
        const me& operator[](const std::string* it) const BY_CONST_FUNC(operator[](it));
        me& operator[](const nchar* name);
        const me& operator[](const nchar* name) const BY_CONST_FUNC(operator[](name));

    public:
        nbool in(const node& elem) const;
        nbool in(const node* elem) const;

        virtual scope& subs() = 0;
        const scope& subs() const BY_CONST_FUNC(subs())
        /**
         * @brief Returns a tstr to a container of immediate sub-nodes (excluding inherited or chained ones).
         * @details This method provides direct access to the immediate children owned by this node,
         *          distinguishing them from children accessible via chained scopes or inheritance.
         * @return A tstr to a container holding the immediate sub-nodes.
         */
        virtual tstr<nbicontainer> mySubs() const;

        /**
         *  @param r type to be promoted to this type.
         *           if a type is null, it means that there is no type specified.
         *           it's complete different to 'void' type.
         *  @return null if it's not relative between l & r.
         */
        const node* promote(const node& it) const;
        const node* promote(const node* it) const BY_SIDE_FUNC(promote);

        template <typename T> T* sub(std::function<nbool(const std::string&, const T&)> l);
        template <typename T>
        const T* sub(std::function<nbool(const std::string&, const T&)> l) const BY_CONST_FUNC(sub<T>(l))
        template <typename T = me> T* sub();
        template <typename T = me> const T* sub() const BY_CONST_FUNC(sub<T>())
        template <typename T = me> T* sub(const std::string& name);
        template <typename T = me> T* sub(const std::string* it) BY_SIDE_FUNC(sub);
        template <typename T = me> const T* sub(const std::string& name) const BY_CONST_FUNC(sub<T>(name))
        template <typename T = me> const T* sub(const std::string* name) const BY_CONST_FUNC(sub<T>(name))
        template <typename T = me> T* sub(const std::string& name, const args& a);
        template <typename T = me>
        T* sub(const std::string* name, const args& a) BY_SIDE_FUNC(name, sub(*name, a), nullptr);
        template <typename T = me>
        const T* sub(const std::string& name, const args& a) const BY_CONST_FUNC(sub<T>(name, a))
        template <typename T = me>
        const T* sub(const std::string* name, const args& a) const BY_CONST_FUNC(sub<T>(name, a))

        template <typename T> tnarr<T, strTactic> subAll(std::function<nbool(const std::string&, const T&)> l) const;
        template <typename T = me> tnarr<T, strTactic> subAll() const;
        template <typename T = me> tpriorities<T> subAll(const std::string& name) const;
        template <typename T = me>
        tpriorities<T> subAll(const std::string* it) const BY_SIDE_FUNC(subAll<T>)


            /**
             *  finds subnodes of this node, allowing implicit conversion based on name and
             *  arguments.
             *  @param a if you pass this as a nullptr, the argument will be ignored and all names
             *  that are the same
             *           will be considered to meet the condition.
             *  @warning if you pass the argument as `args()`, it will be considered to meet the
             *           condition only if there are no parameters and the names are the same, so
             *           the two have completely different meanings.
             */
            template <typename T = me>
            tpriorities<T> subAll(const std::string& name, const args* a) const;
        template <typename T = me> tpriorities<T> subAll(const std::string& name, const args& a) const;
        template <typename T = me>
        tpriorities<T> subAll(const std::string* name, const args* a) const
            BY_SIDE_FUNC(name, subAll<T>(*name, a), tpriorities<T>());
        template <typename T = me>
        tpriorities<T> subAll(const std::string* name, const args& a) const
            BY_SIDE_FUNC(name, subAll<T>(*name, a), tpriorities<T>());

        bool canEval(const args& a) const;
        virtual priorType prioritize(const args& a) const = 0;
        priorType prioritize(const args* it) const BY_SIDE_FUNC(it, prioritize(*it), NO_MATCH);

        virtual str eval(const args& a) = 0;
        str eval(const args* it) BY_SIDE_FUNC(eval);
        str eval(const std::string& name, const args& a);
        str eval(const std::string& name, const args* a) BY_SIDE_FUNC(a, eval(name, a), str());
        str eval(const std::string* name, const args& a) BY_SIDE_FUNC(name, eval(*name, a), str());
        str eval(const std::string* name, const args* a) BY_SIDE_FUNC(name&& a, eval(*name, *a), str());
        str eval(const std::string& name);
        str eval(const std::string* it) BY_SIDE_FUNC(eval);
        str eval();

        /**
         *  Release all held resources and prepare for termination.
         *  @remark Some classes won't be able to reinitialize after rel() is called.
         */
        virtual void rel() {}

        template <typename T> nbool is() const { return is(ttype<T>::get()); }

        nbool is(const typeProvidable& to) const;
        nbool is(const typeProvidable* it) const BY_SIDE_FUNC(is);
        nbool is(const type& to) const;
        nbool is(const type* it) const BY_SIDE_FUNC(is);

        template <typename T> tstr<T> as() const { return as(ttype<T>::get()); }

        str as(const typeProvidable& to) const;
        str as(const typeProvidable* it) const BY_SIDE_FUNC(as);
        str as(const type& to) const;
        str as(const type* it) const BY_SIDE_FUNC(as);

        template <typename T> nbool isImpli() const { return isImpli(ttype<T>::get()); }

        virtual nbool isImpli(const type& to) const;
        nbool isImpli(const typeProvidable& to) const;
        nbool isImpli(const typeProvidable* it) const BY_SIDE_FUNC(isImpli);

        template <typename T> tstr<T> asImpli() const { return asImpli(ttype<T>::get()); }

        virtual str asImpli(const type& to) const;
        str asImpli(const typeProvidable& to) const;
        str asImpli(const typeProvidable* it) const BY_SIDE_FUNC(asImpli);

        virtual const src& getSrc() const;

        /**
         *  returns whether this instance is complete or not.
         *  if some instance is incomplete, it means that it's not accessible on byeolang source
         *  codes.
         */
        virtual nbool isComplete() const;

        /**
         *  getType() returns what it is. opposite to it, this returns what this class will
         *  represents after evaluation.
         *
         *  for example, the @ref expr class has derived from this node class. and if an user call the
         *  funcs to get type of it, class 'ntype' of 'expr' will be returned.
         *  but if that user call the 'infer()' then the 'expr' object evaluate its terms and
         *  returns it as a node from the output. it could be an integer if it was @ref addExpr and all
         *  terms are constructed with integers.
         *
         *  and also this func requires frames. means that you have to interact frame first before
         *  call this func.
         *  @remark some derived class of node may require to call this function first by @ref verifier.
         *          if you call this function without calling @ref verifier previously will damage then
         *          process.
         */
        virtual str infer() const;

        virtual void accept(const visitInfo& i, visitor& v);

        using frameInteractable::inFrame;
        void inFrame(const bicontainable* args) const override;
        using frameInteractable::outFrame;
        void outFrame() const override;

        virtual const modifier& getModifier() const;

    protected:
        /**
         * @brief Protected virtual method for evaluating a sub-node.
         * @details This method is a crucial part of the evaluation mechanism, allowing derived
         *          classes to define how a specific sub-node is evaluated within the context
         *          of the current node.
         * @param sub The sub-node to be evaluated.
         * @param a The arguments for evaluation.
         * @return The result of the evaluation as a `str` object.
         */
        virtual str _onEvalSub(node& sub, const args& a);
        virtual void _setSrc(const src& s);
        void _setSrc(const src* it) BY_SIDE_FUNC(_setSrc);
    };

    extern template class tnmap<std::string, node>;
    typedef tnmap<std::string, node> nmap;
} // namespace by
