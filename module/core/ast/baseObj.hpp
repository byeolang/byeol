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

    /// @ingroup core
    /// @brief Base class for representing byeol objects
    /// @details The base class for representing objects in byeol. The core module uses both objects in the managed
    /// environment written in byeol language (@ref obj) and objects in the native environment written using C++ code
    /// (@ref baseObj) without distinction. Both are @ref node and baseObj. To achieve this, obj inherits from
    /// baseObj, grouping them into the same class hierarchy. Therefore, baseObj only has functionality common to
    /// native and managed objects. In other words, managed object obj always has more functionality than baseObj.
    ///
    /// @section origin_object Origin Objects
    /// The original type defined by users in the byeol language is called an @ref origin object. Constructor calls
    /// like @ref ctor all create by copy-constructing origin objects.
    ///
    /// @section override_getOrigin Override getOrigin() for baseObj
    /// To define a new baseObj in C++ code for use in byeol code, create a C++ class inheriting baseObj and override
    /// getOrigin() to return the appropriate baseObj origin object. For example:
    ///
    /// @code
    ///     // There's a C++ class nInt inheriting from baseObj that represents integers.
    ///     // This nInt's origin is based on the nInt C++ class itself.
    ///     // Being based on a C++ class makes it static and can be expressed as a singleton.
    ///     const baseObj& nInt::getOrigin() const {
    ///         // Using tbaseObjOrigin easily creates a tbaseObjOrigin object based on nInt class.
    ///         static tbaseObjOrigin<me> org(tbridger<me>::ctor().ctor<me>().subs());
    ///         const baseObj& supers = super::getOrigin();
    ///         return &supers == this ? org : supers;
    ///     }
    /// @endcode
    ///
    /// @ref tbaseObjOrigin is a class template added for convenience. See that class for details. Using @ref tbridger
    /// allows exposing native functions as managed functions in one simple line without additional code. @ref nStr
    /// has a good example.
    ///
    /// @section baseobj_only_one_origin Unlike obj, baseObj's Origin Object Exists Only Once
    /// obj represents objects written in byeol language. Among these, origin objects written with the `def` keyword
    /// are represented as instances of the origin class. In contrast, baseObj cannot be used by itself; there exist
    /// C++ classes that inherit from it. Understand the difference well. From C++ code's perspective, baseObj's origin
    /// is static while obj's origin is dynamic. For example:
    ///
    /// @code
    ///     // Look again at the previously written baseObj origin object example. Note it's a static object.
    ///     // Can we do the same for obj?
    ///
    ///     // Suppose a user wrote the following in byeol code:
    ///     //      def MyObj
    ///     //          foo() void
    ///     //              print("hello")
    ///     // The MyObj type is dynamic from C++'s perspective. So we can't create class MyObj at compile time.
    ///     // MyObj itself in byeol is called an origin object and is represented as an instance of the `origin` class.
    ///     origin org(typeMaker::make<obj>(name)); // name == "MyObj"
    ///     // Created at runtime by the parser as above, so it can't be a singleton or static.
    /// @endcode
    class _nout baseObj: public node, public statable {
        BY(ADT(baseObj, node))
        friend class verifier;
        friend class obj;
        friend class defaultMakeCtor;
        friend class parser;        // _setOrigin()
        friend class genericOrigin; // from genericOrigin::_makeGeneric(), _setOrigin()
        friend class exprMaker;
        friend class defVarExpr; // for _setModifier
        template <typename T> friend class tbaseObjOrigin;

    protected:
        /// if you don't give any subs when construct an @ref baseObj, _subs will be assigned to dummy
        /// array. instance on ctor of derived class.
        explicit baseObj() = default;
        explicit baseObj(const baseObj* org, nbool);

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

        virtual const node& getSubPack() const;

        virtual baseObj* make() const;

        const modifier& getModifier() const override;

    protected:
        str _onEvalSub(node& sub, const args& a) override;
        void _setSrc(const src& s) override;
        virtual void _inFrame(frame& fr, const bicontainable* args) const;

        // update @ref origin pointer of an object.
        // to modify origin* is very dangerous. only permitted module should do this.
        void _setOrigin(const baseObj& newOrg);

        virtual void _setType(const mgdType& new1);

        virtual void _setModifier(const modifier& mod);

        virtual str _onBeforeCtor();

    public:
        inline static const std::string COMMON_NAME = "@common";
        inline static const std::string EXPAND_NAME = "@expand";

    private:
        tstr<baseObj> _org;
    };

    typedef tmock<baseObj> mockObj;
} // namespace by
