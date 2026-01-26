/// @file
#pragma once

#include "meta/common.hpp"

namespace by {

    /** @ingroup meta
     *  @brief Core class for runtime type information in byeol language
     *  @details The central class of the meta module. Provides fundamental type information APIs.
     *
     *  @section Basic type identification
     *  - isTemplate(): Returns whether this is a template class
     *  - isAbstract(): Returns whether this is an abstract class
     *  - getName(): Returns class name (demangled)
     *
     *  @section Class hierarchy information
     *  - getSupers(): Returns list of super classes, with direct parent at the end
     *  - getSubs(): Returns list of sub classes, with closest descendants first
     *  - isSuper(const type& rhs): Checks if this is a super class of rhs
     *  - isSub(const type& rhs): Checks if this is a sub class of rhs
     *
     *  Performance note: isSuper() and isSub() are more efficient than dynamic_cast.
     *  While dynamic_cast typically loops through vtables, the meta module uses a tier
     *  algorithm that compares tier values and char* addresses for O(1) type checking.
     *
     *  @section Instance creation
     *  - make(): Creates instance using default constructor. Returns nullptr if no
     *    default constructor exists.
     *
     *  @section Meta type information management
     *  - init(): Initializes type information
     *  - rel(): Releases type information
     *
     *  These are typically handled automatically via @ref BY_INIT_META macro and rarely
     *  need direct invocation.
     *
     *  @section How meta information is generated
     *  Type information like isTemplate(), isAbstract(), and getName() is filled by
     *  @ref ttypeBase<T> through metaprogramming. The purpose of type::init() is to
     *  construct the class hierarchy.
     *
     *  The hierarchy is built using the constraint that "all classes must define super
     *  as a typedef". With super defined for all classes, ttype<super>().init() can be
     *  called, enabling recursive class hierarchy construction:
     *
     *  @code
     *      nbool type::init() {
     *          if(_isInit) return false;
     *          _isInit = true; // Executes only once
     *
     *          type& super = (type&) getSuper();
     *          super.init(); // Recursively calls parent's init
     *                        // Eventually reaches adam, which has no parent
     *
     *          types& mySupers = getSupers();
     *          mySupers = super.getSupers();
     *          mySupers.push_back(&super);
     *      }
     *  @endcode
     *
     *  @section Automatic meta information generation
     *  Type objects are initialized via init() calls, but manually calling init() for
     *  every class would be inefficient. The @ref BY_INIT_META macro solves this by
     *  using @ref BY_INITIATOR to execute init() before main() via static object
     *  initialization with lambda functions.
     *
     *  The constraint is that each class declaration must include BY_INIT_META(MyClass).
     *  These meta DSL-style macros are executed through the BY macro convention, and
     *  core module adds additional meta DSL macros, so use BY(CLASS()) or BY(ADT())
     *  instead of calling BY_INIT_META directly.
     *
     *  @section Adding custom meta information
     *  While type provides substantial type information, language implementations like
     *  byeol may need additional information like parameters or return types. You might
     *  think to inherit from type, but since the user entry point must always be
     *  ttype<T>, and you cannot modify ttype<T> code from modules depending on meta,
     *  inheritance isn't possible. Instead, inject custom meta types.
     *
     *  The core code is in @ref ttypeBase<T>:
     *  @code
     *      template <typename T, typename S = typename tmetaTypeDef<T>::is>
     *      class ttypeBase: public S { }
     *
     *      template <typename T, nbool hasMeta = tifHasMetaTypeDef<T>::is>
     *      struct tmetaTypeDef { using is = type; };
     *
     *      template <typename T>
     *      struct tmetaTypeDef<T, true> { using is = typename T::metaType; };
     *  @endcode
     *
     *  tmetaTypeDef returns T::metaType if it exists, otherwise returns type. ttype
     *  inherits from ttypeBase, which inherits from tmetaTypeDef<T>::is. When calling
     *  ttype<T>, if class T defines `typedef metaType MyType`, the ttype<T> object
     *  will be based on MyType.
     *
     *  This feature is actually used in the core module to inject ntype. See @ref ntype
     *  for details.
     */
    class _nout type {
        BY_ME(type)

    public:
        virtual ~type() {}

    public:
        virtual nbool operator==(const me& rhs) const;
        nbool operator!=(const me& rhs) const;

    public:
        virtual nbool isTemplate() const = 0;
        virtual nbool isAbstract() const = 0;
        virtual const std::string& getName() const;

        /**
         * @brief  create an instance to be refered this type.
         * @remark available when the type defines a ctor without any params.
         * @return return an address of new instance, however, if ctor without any params
         *         isn't defined, then returns null.
         */
        virtual void* make() const = 0;

        template <typename T> T* makeAs() const {
            // c++ is not allow for covariant against void*.
            // that's is why I make another variant func of already made make().
            return (T*) make();
        }

        virtual ncnt size() const = 0;

        /**
         * @brief Initializes type metadata and constructs class hierarchy
         * @return true on success, false if already initialized
         * @note Recursively initializes super class hierarchy. Automatically called via
         *       BY_INIT_META macro before main(). Sets up super/sub relationships.
         */
        virtual nbool init();

        virtual nbool rel();
        virtual const type& getSuper() const = 0;
        virtual const nbool& isInit() const = 0;

        /**
         * @brief Returns all most derived classes (leaf nodes) from this class
         * @return Vector of type pointers representing all leaf classes in hierarchy
         */
        const types& getLeafs() const;

        const types& getSubs() const;
        const types& getSupers() const;

        /**
         * @brief Checks if this type is a super class of the given type
         * @return true if this is a super class of it, false otherwise
         * @note Uses O(1) tier algorithm comparing tier values, much faster than dynamic_cast
         *       which typically loops through vtables.
         */
        virtual nbool isSuper(const type& it) const;
        nbool isSuper(const type* it) const BY_SIDE_FUNC(isSuper);
        template <typename T> nbool isSuper() const;

        /**
         * @brief Checks if this type is a sub class of the given type
         * @return true if this is a sub class of it, false otherwise
         * @note Uses O(1) tier algorithm. Implemented as it.isSuper(*this).
         */
        nbool isSub(const type& it) const;
        nbool isSub(const type* it) const BY_SIDE_FUNC(isSub);
        template <typename T> nbool isSub() const;
        const type& getStatic() const BY_CONST_FUNC(_getStatic())

        /**
         * @brief Get meta type name for efficient type checking
         * @details This returns metaTypename.
         * metaTypename can be used like 'dynamic_cast<yourType>'.
         * as you may know, c++'s dynamic_cast is slow. because normally compilers tries to loop
         * in order to figure out which type is fit to given your type parameter.
         * 'meta' library, however, uses 'tier' algorithm and it's O(1), so it's faster.
         *
         * you can use your own metaType to represent more data on type class.
         * for instance, 'core' module uses 'ntype' custom type class.
         * but in that case, when you compare custom type class, you must compare extended data to
         * 'rhs' variable to base type class, 'type'.
         *
         * so how can you know that 'type' is actually instance of your derived custom type class
         * in 'tier' algorithm? please don't think about 'dynamic_cast'. it'll vanish our
         * effectiveness to use 'tier' algorithm. that's why I make 'getMetaTypeName()' func.
         *
         * @return static literal c-style string for meta type name.
         *         so you are able to use c-style casting if address of 'rhs' variables's
         *         getMetaTypeName() isn just same to yours.
         * @code
         * yourType& a = ....;
         * type& rhs = ....;
         * if(a.getMetaTypeName() != rhs.getMetaTypeName()) return;
         * yourType& rhsCasted = (yourType&) rhs;
         *
         * ...now you can do something on yourType's data...
         * @endcode
         */
        virtual const nchar* getMetaTypeName() const;

    protected:
        //  type:
        virtual types& _getSubs() = 0;
        virtual types& _getSupers() = 0;
        virtual type& _getStatic() const = 0;
        void _setInit(nbool newState);
        virtual void _onAddSubClass(const me& subClass);
        virtual types** _onGetLeafs() const = 0;
        void _setLeafs(types* newLeafs) const;
        virtual const std::string& _getNativeName() const;

    private:
        nbool _logInitOk(nbool res);
        void _findLeafs(const type& cls, types& tray) const;
    };
} // namespace by
