/// @file
#pragma once

#include "core/type/mgdType.hpp"
#include "core/ast/obj.hpp"

namespace by {

    class src;
    class evalExpr;

    /** @ingroup core
     *  @brief User-defined origin type
     *  @details The original type defined by users in the byeol language is called an origin object. Constructor
     *  calls like @ref ctor all create by copy-constructing origin objects.
     *
     *  @remark Never carelessly use metatype on origin.
     *  origin objects inherit from @ref obj and are written assuming they'll be used as obj type except at creation
     *  time. Therefore, code like this is somewhat risky:
     *
     *  @code
     *      origin* new1 = new origin(...); // Referencing as origin* at creation is fine but..
     *      new1->getType();                 // Without understanding origin class characteristics,
     *                                       // using the `meta` module to get type info like this isn't recommended.
     *                                       // You might end up writing like below.
     *
     *      new1->cast<origin>();            // This code returns nullptr.
     *  @endcode
     *
     *  @section why_cast_origin_returns_null Why cast<origin>() Returns nullptr
     *  `getType()` doesn't just apply to script object origin types defined in byeol. It also provides types for
     *  native environment C++ classes like @ref node and @ref instance. For example, suppose a user writes this byeol
     *  code:
     *
     *  @code
     *      def MyObj
     *          foo() void
     *              print("hello")
     *
     *      def YourObj
     *          boo() void
     *              doSomething()
     *  @endcode
     *
     *  `MyObj` is part of the AST, so in C++ it's represented as an instance of @ref obj. If you call `getType()` on
     *  the obj instance representing `MyObj` without additional code, what happens? Naturally `ttype<obj>()` is
     *  returned. The problem is calling on the obj instance representing `YourObj` also returns the same
     *  ttype<obj>(). Result: @ref starter executing the program thinks YourObj and MyObj are the same type.
     *
     *  To solve this, the C++ obj class has a @ref mgdType variable as a member. mgdType is type information for byeol
     *  types in the byeol runtime environment. See mgdType for details. The key point is this mgdType is structured to
     *  extend the existing native type meta information tree, so it's represented as inheriting from ttype<obj>.
     *  Thus, calling getType() on the C++ MyObj object yields mgdType, and this type can be distinguished from
     *  YourObj while having parent class `obj` type.
     *
     *  If we didn't do this but instead separated getType() and getMgdType(), all functions using type would need to
     *  distinguish whether the received node* is a native or script object, and if managed, call getMgdType() to
     *  handle it.
     *
     *  Sharp readers may now understand why cast<origin>() shouldn't be used. Without any processing, origin class's
     *  getType() would try to return ttype<origin>(). But as in the example above, the obj instance created to
     *  represent byeol code's MyObj must return mgdType. And naturally ttype<origin> has no inheritance relationship
     *  with the just-created mgdType instance, so calling cast<origin> fails the internal type check.
     *
     *  Therefore, the key is that starting from C++'s `obj` class, providing metatype by existing C++ class switches
     *  to providing metatype by C++ instance via mgdType. So don't carelessly try to use the meta module on origin
     *  objects. origin class's getType() returns a type with no relation to C++'s origin type. But this mgdType is
     *  created by the parser to have `ttype<obj>` as parent class. So type conversion to obj type or `cast<obj>()`
     *  etc. are no problem.
     *
     *  @code
     *      origin* makeOrigin() {
     *          mgdType t = typeMaker::make<obj>("MyObj"); // mgdType usually created via typeMaker class.
     *                                                     // t has parent `obj` and name MyObj.
     *          return new origin(t);                      // Unless necessary, return type should be obj* not origin*.
     *                                                     // But for origin-only functions like setCallComplete(),
     *                                                     // return as origin* type.
     *      }
     *
     *      void main() {
     *          auto* org = makeOrigin();
     *          org->setCallComplete(....);
     *
     *          tstr<obj> bind1(org);  // Success: obj is origin's parent class.
     *          str bind2(org);        // Success
     *
     *          obj* cast = bind2->cast<obj>();  // Success.
     *          cast->getType().isSub<obj>();    // true
     *      }
     *  @endcode
     *
     *  @remark Since origin itself is premised on being used as obj type, almost all public functions except some are
     *  identical to obj type's API. No need to specifically use origin type.
     *
     *  @remark The call itself has no C++ compiler restrictions. Type conversion is safely possible with
     *  dynamic_cast, and if you understand the above constraints well, you can freely call getType() from origin and
     *  use cast, isSub, etc. In summary, understand the intent and use it.
     */
    class _nout origin final: public obj {
        BY(ME(origin, obj), INIT_META(origin))
        friend class coreInternal;

    public:
        explicit origin(const mgdType& newType);
        explicit origin(const mgdType& newType, node* subpod);
        explicit origin(const me& rhs);

    public:
        me& operator=(const me& rhs);

    public:
        using super::subs;
        scope& subs() override;

        state getState() const override;
        void setState(state new1) override;
        const ntype& getType() const override;
        const node& getSubPod() const override;
        const src& getSrc() const override;
        void onCloneDeep(const clonable& from) override;
        baseObj* make() const override;

        using super::getCallComplete;
        node* getCallComplete() override;

        void setCallComplete(const node& new1);
        void setCallComplete(const node* it) BY_SIDE_FUNC(setCallComplete);

        nbool isComplete() const override;
        clonable* clone() const override;
        clonable* cloneDeep() const override;

    private:
        void _setType(const mgdType& new1) override;
        void _setSrc(const src& s) override;
        void _setModifier(const modifier& mod) override;
        me& _assign(const me& rhs);
        void _runCallComplete();

    private:
        mgdType _type; // TODO: memory leak
        str _subpod;
        tstr<src> _src;
        tstr<modifier> _mod;
        str _callComplete;
        state _state;
    };
} // namespace by
