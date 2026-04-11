/// @file
#pragma once

#include "core/frame/frameInteractable.hpp"
#include "core/ast/src/src.hpp"
#include "core/ast/statable.hpp"
#include "core/ast/modifier/modifier.hpp"
#include "core/ast/tmock.hpp"

namespace by {

    class baseFunc;
    typedef tnarr<baseFunc> funcs;
    class frame;
    class origin;
    class obj;
    class mgdType;
    template <typename T> class tbaseObjOrigin;

    /** @ingroup core
     *  @brief Base class for representing byeol objects
     *  @details The base class for representing objects in byeol. The core module uses both objects in the byeol
     * runtime environment written in byeol language (@ref obj) and objects in the native environment written using C++
     * code
     *  (@ref baseObj) without distinction. Both are @ref node and baseObj. To achieve this, obj inherits from
     *  baseObj, grouping them into the same class hierarchy. Therefore, baseObj only has functionality common to
     *  native and script objects. In other words, managed object obj always has more functionality than baseObj.
     *
     *  @section origin_object Origin Objects
     *  The original type defined by users in the byeol language is called an @ref origin object. Constructor calls
     *  like @ref ctor all create by copy-constructing origin objects.
     *
     *  @section override_getOrigin Override getOrigin() for baseObj
     *  To define a new baseObj in C++ code for use in byeol code, create a C++ class inheriting baseObj and override
     *  getOrigin() to return the appropriate baseObj origin object. For example:
     *
     *  @code
     *      // There's a C++ class nInt inheriting from baseObj that represents integers.
     *      // This nInt's origin is based on the nInt C++ class itself.
     *      // Being based on a C++ class makes it static and can be expressed as a singleton.
     *      const baseObj& nInt::getOrigin() const {
     *          // Using tbaseObjOrigin easily creates a tbaseObjOrigin object based on nInt class.
     *          static tbaseObjOrigin<me> org(tbridger<me>::ctor().ctor<me>().subs());
     *          const baseObj& supers = super::getOrigin();
     *          return &supers == this ? org : supers;
     *      }
     *  @endcode
     *
     *  @ref tbaseObjOrigin is a class template added for convenience. See that class for details. Using @ref tbridger
     *  allows exposing native functions as script functions in one simple line without additional code. @ref nStr
     *  has a good example.
     *
     *  @section baseobj_only_one_origin Unlike obj, baseObj's Origin Object Exists Only Once
     *  obj represents objects written in byeol language. Among these, origin objects written with the `def` keyword
     *  are represented as instances of the origin class. In contrast, baseObj cannot be used by itself; there exist
     *  C++ classes that inherit from it. Understand the difference well. From C++ code's perspective, baseObj's origin
     *  is static while obj's origin is dynamic. For example:
     *
     *  @code
     *      // Look again at the previously written baseObj origin object example. Note it's a static object.
     *      // Can we do the same for obj?
     *
     *      // Suppose a user wrote the following in byeol code:
     *      //      def MyObj
     *      //          foo() void
     *      //              print("hello")
     *      // The MyObj type is dynamic from C++'s perspective. So we can't create class MyObj at compile time.
     *      // MyObj itself in byeol is called an origin object and is represented as an instance of the `origin` class.
     *      origin org(typeMaker::make<obj>(name)); // name == "MyObj"
     *      // Created at runtime by the parser as above, so it can't be a singleton or static.
     *  @endcode
     */
    class _nout baseObj: public node, public statable {
        BY(ADT(baseObj, node))
        friend class coreInternal;

    protected:
        /**
         * @brief Default constructor for baseObj.
         * @details If no sub-objects are provided, internal sub-containers will be assigned to a dummy array.
         *          This is typically called in the constructor of derived classes.
         */
        explicit baseObj() = default;
        /**
         * @brief Constructs a baseObj from an existing origin object.
         * @param org A pointer to the origin baseObj.
         * @param isScripted A boolean indicating whether this baseObj is part of the Byeol runtime
         * environment.
         */
        explicit baseObj(const baseObj* org, nbool isScripted);

    public:
        using super::eval;
        str eval(const args& a) override;

        using super::subs;
        scope& subs() override;

        state getState() const override;
        void setState(state new1) override;

        using super::prioritize;
        priorType prioritize(const args& a) const override;

        virtual const baseObj& getOrigin() const;

        using super::inFrame;
        void inFrame(const bicontainable* args) const override;
        using super::outFrame;
        void outFrame() const override;

        const src& getSrc() const override;

        /**
         * @brief Retrieves a sub-pod represented as a node.
         * @details This method is used to access nested pod structures or components within this base object.
         * @return A const reference to the node representing the sub-pod.
         */
        virtual const node& getSubPod() const;

        virtual baseObj* make() const;

        const modifier& getModifier() const override;

    protected:
        str _onEvalSub(node& sub, const args& a) override;
        void _setSrc(const src& s) override;
        /**
         * @brief Enters this object's frame into the current execution stack.
         * @details This protected virtual method is a key part of the frame interaction mechanism,
         *          allowing the object's associated scope and symbols to become active in the
         *          current execution context.
         * @param fr The frame to interact with.
         * @param args Optional arguments to be pushed onto the frame.
         */
        virtual void _inFrame(frame& fr, const bicontainable* args) const;

        /**
         * @brief Sets the script type for this base object.
         * @details This method is used to associate a specific script type (`mgdType`) with this
         *          base object, allowing for dynamic type resolution in the Byeol language environment.
         * @param new1 The new `mgdType` to assign.
         */
        virtual void _setType(const mgdType& new1);

        virtual void _setModifier(const modifier& mod);

        /**
         * @brief Hook method called before the object's constructor is fully executed.
         * @details This protected virtual method provides a mechanism for derived classes or the
         *          runtime to inject logic or perform setup just before the main constructor body.
         * @return An `str` object representing a pre-constructor state or result.
         */
        virtual str _onBeforeCtor();

    public:
        inline static const std::string COMMON_NAME = "@common";
        inline static const std::string EXPAND_NAME = "@expand";

    protected:
        tstr<baseObj> _org;
    };

    typedef tmock<baseObj> mockObj;
} // namespace by
