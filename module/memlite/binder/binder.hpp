/// @file
#pragma once

#include "memlite/binder/tbindable.hpp"

struct binderTest;

namespace by {

    class bindTacticable;
    class life;

    /** @ingroup memlite
     *  @brief Generalized binding class for managing @ref instance lifecycles
     *  @details Can bind any object inheriting from @ref instance. Uses reference counting
     *  to properly destroy objects at appropriate times. Similar to std::weak_ptr and
     *  std::shared_ptr, with @ref tweak handling weak pointers and @ref tstr handling
     *  strong pointers.
     *
     *  @section Usage
     *  Basic usage with bind(), isBind(), and get():
     *  @code
     *      class A : public instance {}; // Inherits instance, so bindable
     *      A* a = new A();
     *
     *      {
     *          tstr<A> strBinder;
     *          strBinder.bind(a);
     *          // a's life count becomes 1
     *
     *          strBinder.isBind(); // true
     *          a == strBinder.get(); // true
     *      } // strBinder destroys, life count becomes 0, a automatically destroyed
     *
     *      *a; // Error: using destroyed object
     *  @endcode
     *
     *  More concise real-world usage:
     *  @code
     *      class shell : public instance {
     *      public:
     *          int age;
     *      };
     *
     *      tstr<shell> foo() {
     *          tstr<shell> ptr(new shell()); // Create and bind simultaneously
     *          ptr->age = 57; // Supports operator->
     *
     *          tweak<shell> weak = ptr; // Binders can be compatible
     *          callShell(*weak); // Supports operator* too
     *
     *          return ptr; // Returns by value, count maintained
     *                      // shell object created with new won't be destroyed
     *      }
     *  @endcode
     *
     *  @section Why not use shared_ptr?
     *  Several advantages over shared_ptr:
     *
     *  1. Reference counting block attached to instance itself
     *     - shared_ptr creates a "Control block" on the heap for reference counting.
     *       This makes the following dangerous:
     *     @code
     *         Foo* raw = new Foo();
     *         shared_ptr<Foo> foo1(raw);
     *         shared_ptr<Foo> foo2(raw); // Separate control blocks -> double delete
     *     @endcode
     *     - memlite uses @ref life class for reference counting, assigned per instance
     *       by @ref watcher. No double deletion problem.
     *
     *  2. Provides ADT (Abstract Data Type)
     *     - tstr and tweak share same binder base, enabling generic logic:
     *     @code
     *         void me::rel(binder& me) { // Works for both tstr and tweak
     *             WHEN(!me.isBind()) .ret();
     *             life* l = me._getBindTag();
     *             if(l) l->_onStrong(-1);
     *         }
     *     @endcode
     *
     *  3. Dynamic type checking
     *     - binder is ADT and not even a class template. bind() parameter is instance type.
     *       tstr<A> allows bind(new B()); without compile error. bind() uses meta module
     *       for dynamic type checking, binding only when types match.
     *
     *  @remark Abstract nature
     *  binder is abstract, so cannot create objects. Only meaningful for generic logic
     *  using already created tstr or tweak binders.
     *
     *  @section Custom memory pool
     *  Ultimate goal of memlite is lightweight C++ memory management for byeol managed
     *  environment. Requires GC and additional memory management, implying custom memory
     *  pool and instance lifecycle management. All instance allocation starts with
     *  @ref instancer.
     *
     *  @section Performance improvements
     *  shared_ptr's algorithm stores reference counting info on heap. Using faster custom
     *  memory pool instead of heap and optimizing binding speed provides performance gains.
     *  Binding is one of the hotspots consuming most performance in byeol.
     *
     *  @section Additional information
     *  shared_ptr creates objects on heap for reference counting. memlite uses @ref watcher
     *  to lend empty @ref life objects from pre-allocated memory, using them as reference
     *  counting space for instances. If GC features are added later, instances may require
     *  additional lifecycle information. Unlike shared_ptr, managing each instance's
     *  lifecycle information internally allows appropriate handling of such requirements.
     */
    class _nout binder: public typeProvidable, public tbindable<instance> {
        BY(ME(binder, instance), INIT_META(me))

        friend class weakTactic;
        friend class strTactic;
        friend class life;          // for _get()
        friend struct ::binderTest; // for UT

    public:
        binder(const type& type, bindTacticable& tactic);
        binder(const me& rhs);
        virtual ~binder();

    public:
        /**
         *  @brief Dereference operator to access bound instance
         *  @details This follows the same policy as tmay and stl.
         *  that is, if the binder does not bind any instances and tries to dereference them with
         *  `get()` or `operator*()`, it will behave as UB.
         *  this is likely to crash.
         */
        instance* operator->();
        const instance* operator->() const BY_CONST_FUNC(operator->())
        instance& operator*();
        const instance& operator*() const BY_CONST_FUNC(operator*())
        me& operator=(const me& rhs);

    public:
        //  binder:
        id getItsId() const;
        //  tbindable:
        nbool isBind() const override;
        void rel() override;
        using tbindable::canBind;

        /**
         * @brief Checks if given type can be bound to this binder
         * @return true if type matches or is subtype of binder's type, false otherwise
         * @note Uses meta module for dynamic type checking
         */
        nbool canBind(const type& it) const override;
        using tbindable::bind;

        /**
         * @brief Binds instance to this binder with reference counting
         * @return true on success, false if type mismatch or binding fails
         * @note Increases reference count. Type must match via canBind() check.
         */
        nbool bind(const instance& it) override;

        instance* get();
        const instance* get() const BY_CONST_FUNC(get())

        template <typename E> E* get() { return get()->template cast<E>(); }
        template <typename E> const E* get() const BY_CONST_FUNC(get<E>())

        //  typeProvidable:
        const type& getType() const override;

        using typeProvidable::cast;
        void* cast(const type& to) override;

    protected:
        /**
         * @brief Assigns binder from another binder
         * @return true on success, false on failure
         * @note Handles reference counting via tactic when copying bindings
         */
        nbool _assign(const binder& rhs);

        /**
         * @brief Checks if two typeProvidable objects are same instance
         * @return true if same object, false otherwise
         * @note Overrides typeProvidable for binder-specific equality
         */
        nbool _onSame(const typeProvidable& rhs) const override;

        /**
         * @brief Retrieves life object associated with bound instance
         * @return Pointer to life object, or nullptr if not bound
         * @note Internal accessor for reference counting operations
         */
        life* _getBindTag() const;

    protected:
        id _itsId; // id for binded one
        const type* _type;
        bindTacticable* _tactic;
    };

    // extension for OR macro:
    template <typename F> tmedium<instance> operator|(binder& t, F&& f) {
        f(t);
        // this returns null-reference but take it easy.
        // it'll never be used.
        return t.get();
    }

    template <typename F> tmedium<instance> operator|(const binder& t, F&& f) {
        f(t);
        // this returns null-reference but take it easy.
        // it'll never be used.
        return t.get();
    }

    // extension for typeTrait:
    template <> struct typeTrait<binder> {
        typedef binder Org;
        typedef binder& Ref;
        typedef binder* Ptr;

        static nbool isNul(const binder& it) { return !it.isBind(); }

        static constexpr nbool is_ptr = false;
        static constexpr nbool is_ref = false;
        static constexpr nbool is_like_ptr = true;
    };

    template <> struct typeTrait<binder*> {
        typedef binder Org;
        typedef binder& Ref;
        typedef binder* Ptr;

        static binder* ret() { return nullptr; }

        static nbool isNul(const binder* it) { return !it || !it->isBind(); }

        static constexpr nbool is_ptr = true;
        static constexpr nbool is_ref = false;
        static constexpr nbool is_like_ptr = is_ptr;
    };

    template <> struct typeTrait<binder&> {
        typedef binder Org;
        typedef binder& Ref;
        typedef binder* Ptr;

        static nbool isNul(const binder& it) { return it.isBind(); }

        static constexpr nbool is_ptr = false;
        static constexpr nbool is_ref = true;
        static constexpr nbool is_like_ptr = true;
    };

    // extension for TO macro:
    template <typename T, typename F> auto operator->*(binder* t, F&& f) {
        return t ? f(**t) : typeTrait<std::decay_t<decltype(f(**t))>>::ret();
    }

    template <typename T, typename F> auto operator->*(const binder* t, F&& f) {
        return t ? f(**t) : typeTrait<std::decay_t<decltype(f(**t))>>::ret();
    }

    template <typename T, typename F> auto operator->*(binder& t, F&& f) {
        return t ? f(*t) : typeTrait<std::decay_t<decltype(f(*t))>>::ret();
    }
} // namespace by
