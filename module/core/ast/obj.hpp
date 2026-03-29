/// @file
#pragma once

#include "core/ast/baseObj.hpp"
#include "core/ast/scope.hpp"
#include "core/ast/statable.hpp"

namespace by {

    class mgdType;
    class origin;
    class evalExpr;

    /** @ingroup core
     *  @brief Byeol runtime environment object
     *  @details The class representing objects in the byeol runtime environment. Extends baseObj's functionality to add
     *  scope handling and concepts for shares and owns needed only in the byeol runtime environment.
     *
     *  @section obj_as_type obj as a Type
     *  In C++, types are represented by classes, but byeol has no class concept. Since there's no distinction between
     *  objects and classes, `obj` itself is the type. For example, look at this C++ code:
     *
     *  @code
     *      class A {}; // In C++, class is the type.
     *      A* a = new A(); // Objects and classes are strictly distinguished.
     *
     *      // byeol code:
     *      //  def myObj
     *      //      name str
     *      //      foo() void: ...
     *      //
     *      //  myObj2 myObj
     *      //
     *      // The above code translated to C++:
     *      obj myObj = new obj(....);  // When created with `def` in byeol.
     *      obj* myObj2 = myObj.clone(); // When creating object with `myObj2 myObj` in byeol.
     *  @endcode
     *
     *  As shown, in the byeol runtime environment, objects are just obj created via two paths: `obj defined with def`
     * and `obj cloned from obj`. Since both are used as types, distinguishing them is meaningless.
     *
     *  @section shares_owns Shares and Owns
     *  For baseObj, since the base is a C++ native class, object creation also occurs through C++'s `new` and
     *  constructors, simultaneously creating member variables defined in that class. But what about writing byeol code?
     *  In the byeol runtime environment, obj representing objects can have properties like `name` in the example above.
     * Since defining objects in byeol runtime environment is cloning objects from @ref origin objects, calling clone()
     * on origin also copies properties and funcs. At this time, funcs only need one in the system and don't need to be
     * copied each time a func object is created. Conversely, properties like name must be copied with different values
     * per instance. To handle this efficiently, parts shared among objs of the same type are separated into shares, and
     *  parts that get copied are separated into owns. Therefore, when obj's clone() occurs, shares only gets references
     *  to shares from the original origin, and only the owns part performs clone.
     *
     *  @section immutable_type Immutable Types
     *  str, int, and other scalar types are all immutable types. This stems from byeol's calling strategy following
     *  `by object`, which operates like C-family languages like Java and C# commonly use: `references to objects are
     *  shallow copied, raw types are deep copied`. node is just node, and obj shouldn't know whether the node it must
     *  insert is an immutable type. Breaking this also breaks polymorphism. So whether to copy an incoming object or
     *  just point to a reference is determined through @ref immutableTactic.
     *
     *  See @ref immutableTactic or tscalar for details.
     */
    class _nout obj: public baseObj {
        BY(ME(obj, baseObj), INIT_META(obj), CLONE(obj), VISIT())

    public:
        typedef ntype metaType;

    public:
        /**
         * @brief Constructs an obj representing an "any" class.
         */
        explicit obj(); // this means 'any' class.
        /**
         * @brief Constructs an obj from a given scope, typically representing its initial set of children.
         * @param subs The scope containing the initial sub-nodes (children) for this object.
         */
        explicit obj(const scope& subs);
        /**
         * @brief Copy constructor for obj.
         * @param rhs The obj to copy from.
         */
        explicit obj(const me& rhs);

    protected:
        /**
         * @brief Protected constructor for obj, allowing explicit separation of shares and owns.
         * @details This constructor is used internally to initialize an obj with distinct shared
         *          and owned scopes, which is fundamental to the object model's efficiency.
         * @param shares The scope containing shared properties and functions.
         * @param owns The scope containing owned (instance-specific) properties.
         */
        explicit obj(const scope& shares, scope& owns);

    public:
        me& operator=(const me& rhs);

    public:
        using super::subs;
        scope& subs() override;

        tstr<nbicontainer> mySubs() const override;

        state getState() const override;
        void setState(state new1) override;

        scope& getShares();
        const scope& getShares() const BY_CONST_FUNC(getShares())
        scope::super& getOwns();
        const scope::super& getOwns() const BY_CONST_FUNC(getOwns())

        const ntype& getType() const override;

        /**
         * @brief Performs a deep clone operation, copying the contents from another clonable object.
         * @param from The clonable object to copy from.
         */
        void onCloneDeep(const clonable& from) override;

        /**
         * @brief Returns the node responsible for completing the object's construction or initialization calls.
         * @details In the Byeol language, object construction might involve specific nodes that
         *          handle the final steps or logic after the object is initially created.
         * @return A pointer to the node that manages constructor completion, or nullptr if none.
         */
        virtual node* getCallComplete();
        const node* getCallComplete() const BY_CONST_FUNC(getCallComplete())

    protected:
        void _inFrame(frame& fr, const bicontainable* args) const override;
        str _onBeforeCtor() override;

    private:
        me& _assign(const me& rhs);

    private:
        tstr<scope> _subs;
    };

#ifdef BY_BUILD_PLATFORM_IS_WINDOWS
    // f***ing annoying another MSVC bug here:
    //  first of all, I'm so sorry to start my slang. but I couldn't help spitting it out after
    //  I used plenty hours of heading to the ground.
    //
    //  I don't know why, but unless define below variable here, I'll observe that the
    //  member-variable
    //  '_subs' above was tried to be instantiated but failed.
    //  error message said that 'You've used undefined type "identifiertstr<scope>"'.
    //  however, MSVC definately knows about tstr<T> and scope types.
    //
    //  clang, gcc didn't complain about this.
    namespace {
        static const inline scope a3;
    }
#endif
} // namespace by
